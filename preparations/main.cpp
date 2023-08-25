/*
The goal of this task is to develop an algorithm to solve a warehouse (reach a goal) without knowing
the warehouse as a whole from the get-go, you will need to explore the warehouse step by step.

You have to include "warehouse.h".

Your code goes in the *initialize* and *solve* functions. Read the instructions there carefully.

Compile your code using any c++ compiler and pass level filename as an argument, for example:
g++ main.cpp -o main.exe
main.exe warehouse1.txt
*/

//----------------------------------------first try without using the API----------------------------------//

#include "warehouse.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <cmath>

class Node {
public:
    Point position;
    Node* cameFrom;
    int gCost;
    int fCost;

    Node(Point pos) : position(pos), cameFrom(nullptr), gCost(INT_MAX), fCost(INT_MAX) {}
    
    bool operator<(const Node& other) const {
        return fCost < other.fCost;
    }
};

// Manhattan distance heuristic elcost from one point to another
int heuristic(const Point& a, const Point& b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}

void aStar(const Point& start, const Point& goal) {

    Node startNode(start);
    Node goalNode(goal);
    vector<Node> openSet;
    unordered_set<Node*> closeSet;

    openSet.push_back(startNode);

    while (!openSet.empty()) {
        auto currentIt = min_element(openSet.begin(), openSet.end());
        Node current = *currentIt;
        openSet.erase(currentIt);

        if (current.position.row == goalNode.position.row && current.position.col == goalNode.position.col) {
            //kda elPath found, reconstruct w return elpath
            vector<Node> path;
            Node* pathNode = &current;
            while (pathNode != nullptr) {
                path.push_back(*pathNode);
                pathNode = pathNode->cameFrom;
            }
            reverse(path.begin(), path.end());
            return;
        }

        closeSet.insert(&current);

        // Generatingg neighbors and calculating tentative scores
        vector<Point> neighbors;
        for (const Point& neighborPos : neighbors) {
            Node neighbor(neighborPos);

            if (closeSet.find(&neighbor) != closeSet.end()) {
                continue;
            }

            int tentativeGScore = current.gCost + 1; //uniformmm edge cost

            if (find(openSet.begin(), openSet.end(), neighbor) == openSet.end() || tentativeGScore < neighbor.gScore) {
                neighbor.cameFrom = &current;
                neighbor.gCost = tentativeGScore;
                neighbor.fCost = neighbor.gCost + heuristic(neighbor.position, goalNode.position);

                if (find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                    openSet.push_back(neighbor);
                }
            }
        }
    }
    return;
}
void initialize() {
    cout << "Initial Warehouse State:" << endl;
    printAround();
}

void solve() {
    Point start = getRobotPos();
    Point item = getItemPos();
    Point goal = getGoalPos();

    cameFrom.clear();
    gCost.clear();
    fCost.clear();

    aStar(start, item);
    pickItem();

    cameFrom.clear();
    gCost.clear();
    fCost.clear();

    aStar(item, goal);
}

int main() {
    //reading warehouse grid as an input
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




