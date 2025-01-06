#include<iostream>
#include<vector>
#include<queue>
#include<unordered_map>
#include<string>
#include<utility>
#include<algorithm>
#include<cmath>
#include<chrono>
#include<stdexcept>

using namespace std;

class AStar
{
    struct Node
    {
        int x;
        int y;
        int priority;

        Node(int x, int y, int priority) : x(x), y(y), priority(priority) {}
    };

    static string hashXY(int x, int y)
    {
        return to_string(x) + "," + to_string(y);
    }


    struct ComparePriority
    {
        bool operator()(const Node& a, const Node& b) {
            return a.priority > b.priority;
        }
    };

public:
    static vector<pair<int, int>> findPathAStar (
        const vector<vector<int>>& verticalWalls,
        const vector<vector<int>>& horizontalWalls,
        int rows,
        int cols,
        int startX,
        int startY,
        int endX,
        int endY)
    {
        priority_queue<Node, vector<Node>, ComparePriority> openSet;
        openSet.emplace(startX, startY, 0);

        unordered_map<string, int> costs;
        costs[hashXY(startX, startY)] = 0;

        unordered_map<string, pair<int, int>> predecessors;
        predecessors[hashXY(startX, startY)] = {-1, -1};

        vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

        while(!openSet.empty())
        {
            Node current = openSet.top();
            openSet.pop();

            if(current.x == endX && current.y == endY) {
                vector<pair<int, int>> path;
                pair<int, int> position = pair<int, int>(current.x, current.y);

                while (position.first != -1 && position.second != -1) {
                    path.push_back({position.first, position.second});
                    position = predecessors[hashXY(position.first, position.second)];
                }

                reverse(path.begin(), path.end());
                return path;
            }

            for(const pair<int, int>& direction : directions)
            {
                int neighborX = current.x + direction.first;
                int neighborY = current.y + direction.second;

                if (neighborX < 0 || neighborY < 0 || neighborX >= rows || neighborY >= cols)
                {
                    continue;
                }

                if (direction.first == 0 && direction.second == 1 && neighborY > 0 && verticalWalls[current.x][current.y] == 1)
                {
                    continue;
                }
                if (direction.first == 0 && direction.second == -1 && current.y > 0 && verticalWalls[current.x][current.y - 1] == 1)
                {
                    continue;
                }
                if (direction.first == 1 && direction.second == 0 && neighborX > 0 && horizontalWalls[current.x][current.y] == 1)
                {
                    continue;
                }
                if (direction.first == -1 && direction.second == 0 && current.x > 0 && horizontalWalls[current.x - 1][current.y] == 1)
                {
                    continue;
                }

                int newCost = costs[hashXY(current.x, current.y)] + 1;

                if (!costs.count(hashXY(neighborX, neighborY)) || newCost < costs[hashXY(neighborX, neighborY)])
                {
                    costs[hashXY(neighborX, neighborY)] = newCost;
                    int heuristic = abs(neighborX - endX) + abs(neighborY - endY);
                    int priority = newCost + heuristic;

                    openSet.emplace(neighborX, neighborY, priority);
                    predecessors[hashXY(neighborX, neighborY)] = pair<int, int>(current.x, current.y);
                }
            }
        }

        return {};
    }
};

int main()
{
    try
    {
        int rows, cols, startX, startY, endX, endY;
        cin >> rows >> cols >> startX >> startY >> endX >> endY;

        vector<vector<int>> verticalWalls(rows, vector<int>(cols - 1));
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols - 1; ++j)
            {
                cin >> verticalWalls[i][j];
            }
        }

        vector<vector<int>> horizontalWalls(rows - 1, vector<int>(cols));
        for (int i = 0; i < rows - 1; ++i)
        {
            for (int j = 0; j < cols; ++j) {
                cin >> horizontalWalls[i][j];
            }
        }

        vector<pair<int, int>> path;
        vector<long long> times;
        long long lastDurationMicroseconds = 0;

        for(int i = 0; i < 20; ++i)
        {
            auto startTime = chrono::high_resolution_clock::now();
            path = AStar::findPathAStar(verticalWalls, horizontalWalls, rows, cols, startX, startY, endX, endY);
            auto endTime = chrono::high_resolution_clock::now();

            lastDurationMicroseconds = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
            times.push_back(lastDurationMicroseconds);
        }

        long long avgTime = 0;
        for(int i = 10; i < 20; ++i)
        {
            avgTime += times[i];
        }
        avgTime /= 10;

        if (!path.empty())
        {
            cout << avgTime << endl;

            for (pair<int, int> cell : path) {
                cout << cell.first << endl;
                cout << cell.second << endl;
            }
        }
    }
    catch(const exception& e)
    {
        throw runtime_error(e.what());
    }

    return 0;
}
