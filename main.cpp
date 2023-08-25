/*
The goal of this task is to develop an algorithm to solve a warehouse (reach a goal) without knowing
the warehouse as a whole from the get-go, you will need to explore the warehouse step by step.

You have to include "warehouse.h".

Your code goes in the *initialize* and *solve* functions. Read the instructions there carefully.

Compile your code using any c++ compiler and pass level filename as an argument, for example:
g++ main.cpp -o main.exe
main.exe warehouse1.txt
*/

#include "warehouse.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <cmath>

struct Node {
    Point position;
    int gCost;
    int fCost;

    bool operator>(const Node& other) const {
        return fCost > other.fCost;
    }
};

std::unordered_map<Point,Point> keepTrack;
std::unordered_map<Point,int> gCost;
std::unordered_map<Point,int> fCost;

int hcost(const Point& a, const Point& b) {
    return abs(a.row-b.row)+abs(a.col-b.col);
}

void reconstructPath(Point current) {
    while (keepTrack.find(current) != keepTrack.end()) {
        current = keepTrack[current];
        moveRobot(current.col-getRobotPos().col, current.row-getRobotPos().row);
        printAround();
    }
}

void aStar(const Point& start, const Point& goal) {

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    openSet.push({start, 0, hcost(start, goal)});
    gCost[start] = 0;
    fCost[start] = hcost(start, goal);

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.position.row == goal.row && current.position.col == goal.col) {
            reconstructPath(current.position);
            return;
        }

        for (int down = -1; down <= 1; ++down) {
            for (int right = -1; right <= 1; ++right) {
                if (down == 0 && right == 0) continue;

                Point neighbor = {current.position.row + down, current.position.col + right};
                if (!isWalkable(right, down) || gCost.find(neighbor) != gCost.end()) continue;

                int tentativeGCost = current.gCost + 1; // i assumed uniform edge costt

                if (gCost.find(neighbor) == gCost.end() || tentativeGCost < gCost[neighbor]) {
                    keepTrack[neighbor] = current.position;
                    gCost[neighbor] = tentativeGCost;
                    fCost[neighbor] = tentativeGCost + hcost(neighbor, goal);
                    openSet.push({neighbor, tentativeGCost, fCost[neighbor]});
                }
            }
        }
    }
}




void initialize() {
    cout << "Initial Warehouse State:" << endl;
    printAround();
}

void solve() {
    Point start = getRobotPos();
    Point itemPostion = getItemPos();
    Point goal = getGoalPos();

    keepTrack.clear();
    gCost.clear();
    fCost.clear();
//to pick
    aStar(start, itemPostion);
    //sort of error handling or at least a user can recieve a message :)
    bool pickCheck= pickItem();
    if (pickCheck==0){
        cout<<"There is no item in the Warehouse to pick!";
    }
    
    keepTrack.clear();
    gCost.clear();
    fCost.clear();
//to go to target
    aStar(itemPostion, goal);
}

int main() {
    //reading warehouse grid as an input because there an issue reading from file.txt
    for (int row = 0; row < __privates::mapSize; row++) {
        string line;
        cin >> line;
        for (int col = 0; col < __privates::mapSize; col++) {
            if (line[col] == '>')
            {
                __privates::robotPos.row = row;
                __privates::robotPos.col = col;
            }
            else if (line[col] == 'D')
            {
                __privates::goalPos.row = row;
                __privates::goalPos.col = col;
            }
            else if (line[col] == 'P')
            {
                __privates::pickupPos.row = row;
                __privates::pickupPos.col = col;
            }
            __privates::map[row][col] = (line[col] != '#');
        }
    }

    initialize();

    while (true) {
        if (hasReachedGoal()) {
            cout << "Reached Goal!" << endl;
            break;
        }

        solve();
    }

    return 0;
}



