#include<iostream>
#include<vector>
#include<utility>
#include <climits>
#include<chrono>

using namespace std;

class DFS {
public:
    static vector<pair<int, int>> findShortestPath(
        const vector<vector<int>>& verticalWalls,
        const vector<vector<int>>& horizontalWalls,
        int rows,
        int cols,
        int startX,
        int startY,
        int endX,
        int endY)
    {
        vector<pair<int, int>> shortestPath;
        vector<vector<bool>> visited(rows, vector<bool>(cols, false));
        vector<pair<int, int>> currentPath;
        int minSteps = INT_MAX;

        dfs(verticalWalls, horizontalWalls, rows, cols, startX, startY, endX, endY, visited, currentPath, shortestPath, minSteps, 0);

        return shortestPath;
    }

private:
    static void dfs(
        const vector<vector<int>>& verticalWalls,
        const vector<vector<int>>& horizontalWalls,
        int rows,
        int cols,
        int x,
        int y,
        int endX,
        int endY,
        vector<vector<bool>>& visited,
        vector<pair<int, int>>& currentPath,
        vector<pair<int, int>>& shortestPath,
        int minSteps,
        int steps)
    {
        if (x < 0 || y < 0 || x >= rows || y >= cols || visited[x][y])
        {
            return;
        }

        if (x == endX && y == endY)
        {
            if (steps < minSteps)
            {
                minSteps = steps;
                shortestPath = currentPath;
                shortestPath.push_back({x, y});
            }
            return;
        }

        visited[x][y] = true;
        currentPath.push_back({x, y});

        if (y < cols - 1 && verticalWalls[x][y] == 0)
        {
            dfs(verticalWalls, horizontalWalls, rows, cols, x, y + 1, endX, endY, visited, currentPath, shortestPath, minSteps, steps + 1);
        }
        if (y > 0 && verticalWalls[x][y - 1] == 0)
        {
            dfs(verticalWalls, horizontalWalls, rows, cols, x, y - 1, endX, endY, visited, currentPath, shortestPath, minSteps, steps + 1);
        }
        if (x < rows - 1 && horizontalWalls[x][y] == 0)
        {
            dfs(verticalWalls, horizontalWalls, rows, cols, x + 1, y, endX, endY, visited, currentPath, shortestPath, minSteps, steps + 1);
        }
        if (x > 0 && horizontalWalls[x - 1][y] == 0)
        {
            dfs(verticalWalls, horizontalWalls, rows, cols, x - 1, y, endX, endY, visited, currentPath, shortestPath, minSteps, steps + 1);
        }

        visited[x][y] = false;
        currentPath.pop_back();
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
            path = DFS::findShortestPath(verticalWalls, horizontalWalls, rows, cols, startX, startY, endX, endY);
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
