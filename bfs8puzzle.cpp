#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

// Generates all valid moves from current puzzle state
vector<vector<vector<int>>> getNextStates(const vector<vector<int>> &state) {
    int blankRow = -1, blankCol = -1;

    // Locate the blank tile (represented by 0)
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (state[row][col] == 0) {
                blankRow = row;
                blankCol = col;
                break;
            }
        }
        if (blankRow != -1) break;
    }

    vector<pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    vector<vector<vector<int>>> successors;

    for (const auto& dir : directions) {
        int x = blankRow + dir.first;
        int y = blankCol + dir.second;

        if (x >= 0 && x < 3 && y >= 0 && y < 3) {
            auto newState = state;
            swap(newState[blankRow][blankCol], newState[x][y]);
            successors.emplace_back(newState);
        }
    }

    return successors;
}

// Converts 2D puzzle state to a string key
string toString(const vector<vector<int>> &board) {
    string s;
    for (const auto& row : board) {
        for (int num : row) {
            s += char('0' + num);
        }
    }
    return s;
}

// Breadth-First Search to solve the puzzle
void solvePuzzle(const vector<vector<int>> &initialState) {
    const vector<vector<int>> targetState = {{1,2,3},{4,5,6},{7,8,0}};
    queue<vector<vector<int>>> q;
    unordered_set<string> visited;

    q.push(initialState);
    visited.insert(toString(initialState));

    while (!q.empty()) {
        auto currentState = q.front();
        q.pop();

        if (currentState == targetState) {
            cout << "Success" << endl;
            return;
        }

        for (const auto& next : getNextStates(currentState)) {
            string hashKey = toString(next);
            if (visited.find(hashKey) == visited.end()) {
                visited.insert(hashKey);
                q.push(next);
            }
        }
    }

    cout << "No solution" << endl;
}

int main() {
    vector<vector<int>> inputBoard = {
        {8,1,2},
        {0,4,3},
        {7,6,5}
    };

    solvePuzzle(inputBoard);
    return 0;
}
