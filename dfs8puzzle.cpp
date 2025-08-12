#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

using namespace std;


string flattenBoard(const vector<vector<int>> &matrix) {
    string s;
    for (const auto &row : matrix) {
        for (auto num : row) {
            s += (char)('0' + num);
        }
    }
    return s;
}


vector<vector<vector<int>>> generateMoves(const vector<vector<int>> &state) {
    vector<vector<vector<int>>> moves;
    int blankR = -1, blankC = -1;

   
    for (int i = 0; i < 3 && blankR == -1; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (state[i][j] == 0) {
                blankR = i;
                blankC = j;
                break;
            }
        }
    }

  
    vector<pair<int, int>> deltas = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    for (const auto& [dr, dc] : deltas) {
        int r = blankR + dr;
        int c = blankC + dc;

        if (r >= 0 && r < 3 && c >= 0 && c < 3) {
            auto newState = state;
            swap(newState[blankR][blankC], newState[r][c]);
            moves.push_back(newState);
        }
    }

    return moves;
}


void depthFirstSolver(const vector<vector<int>> &initialBoard) {
    const vector<vector<int>> finalBoard = {{1,2,3}, {4,5,6}, {7,8,0}};
    stack<vector<vector<int>>> dfsStack;
    unordered_set<string> visited;

    dfsStack.push(initialBoard);

    while (!dfsStack.empty()) {
        auto currentBoard = dfsStack.top();
        dfsStack.pop();

        string key = flattenBoard(currentBoard);
        if (visited.find(key) != visited.end()) continue;
        visited.insert(key);

        if (currentBoard == finalBoard) {
            cout << "Success" << endl;
            return;
        }

        auto successors = generateMoves(currentBoard);
        for (auto it = successors.rbegin(); it != successors.rend(); ++it) { 
            string childKey = flattenBoard(*it);
            if (visited.find(childKey) == visited.end()) {
                dfsStack.push(*it);
            }
        }
    }

    cout << "No solution" << endl;
}

int main() {
    vector<vector<int>> puzzle = {
        {1, 2, 3},
        {4, 5, 6},
        {8, 0, 7}
    };

    depthFirstSolver(puzzle);
    return 0;
}
