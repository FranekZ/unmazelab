#include<iostream>
#include<vector>
#include <stack>
#include <unordered_map>
#include <sstream>
#include <chrono>
#include<algorithm>

using namespace std;

class Tremaux
{
    static string hashXY(int x, int y)
    {
        return to_string(x) + "," + to_string(y);
    }

    static vector<pair<int, int>> findNeighbors(
        const pair<int, int>& position,
        const vector<vector<int>>& verticalWalls,
        const vector<vector<int>>& horizontalWalls,
        int rows,
        int cols)
    {
        vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
        vector<pair<int, int>> neighbors;

        int x = position.first;
        int y = position.second;

        for (const pair<int, int>& direction : directions) {
            int nx = x + direction.first;
            int ny = y + direction.second;

            if (nx >= 0 && ny >= 0 && nx < rows && ny < cols) {

                if (direction.first == 0 && direction.second == 1 && ny > 0 && verticalWalls[x][y] == 1)
                {
                    continue;
                }
                if (direction.first == 0 && direction.second == -1 && y > 0 && verticalWalls[x][y - 1] == 1)
                {
                    continue;
                }
                if (direction.first == 1 && direction.second == 0 && nx > 0 && horizontalWalls[x][y] == 1)
                {
                    continue;
                }
                if (direction.first == -1 && direction.second == 0 && x > 0 && horizontalWalls[x - 1][y] == 1)
                {
                    continue;
                }

                neighbors.push_back({nx, ny});
            }
        }

        return neighbors;
    }

public:
    static vector<pair<int, int>> findPathTremaux(
        const vector<vector<int>>& verticalWalls,
        const vector<vector<int>>& horizontalWalls,
        int rows,
        int cols,
        int startX,
        int startY,
        int endX,
        int endY)
    {
        stack<pair<int, int>> positionStack;
        unordered_map<string, int> visitedCount;
        pair<int, int> position = {startX, startY};

        while (!(position.first == endX && position.second == endY))
        {
            string posKey = hashXY(position.first, position.second);
            visitedCount[posKey]++;

            vector<pair<int, int>> neighbors = findNeighbors(position, verticalWalls, horizontalWalls, rows, cols);

            vector<pair<int, int>> newNeighbors;
            for (const auto& neighbor : neighbors)
            {
                if (visitedCount[hashXY(neighbor.first, neighbor.second)] == 0)
                {
                    newNeighbors.push_back(neighbor);
                }
            }

            if (!newNeighbors.empty())
            {
                positionStack.push(position);
                position = newNeighbors[0];
            }
            else
            {
                if (!positionStack.empty())
                {
                    position = positionStack.top();
                    positionStack.pop();
                }
                else
                {
                    return {};
                }
            }
        }

        vector<pair<int, int>> path;
        while (!positionStack.empty())
        {
            path.push_back(positionStack.top());
            positionStack.pop();
        }
        reverse(path.begin(), path.end());
        path.push_back(position);
        return path;
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
            path = Tremaux::findPathTremaux(verticalWalls, horizontalWalls, rows, cols, startX, startY, endX, endY);
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
