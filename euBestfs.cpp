#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

#define SIZE 3

const vector<vector<int>> TARGET = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0} // 0 is the blank tile
};

int dx[] = {-1, 1, 0, 0}; // Up, Down
int dy[] = {0, 0, -1, 1}; // Left, Right

struct PuzzleNode {
    vector<vector<int>> board;
    int blankX, blankY;
    int depth;
    int heuristic;
    PuzzleNode* parent;

    PuzzleNode(vector<vector<int>> b, int x, int y, int d, PuzzleNode* p)
        : board(b), blankX(x), blankY(y), depth(d), parent(p) {
        heuristic = 0;
    }
};

int computeHeuristic(const vector<vector<int>>& current) {
    int mismatch = 0;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            if (current[i][j] != 0 && current[i][j] != TARGET[i][j])
                mismatch++;
    return mismatch;
}

bool withinBounds(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

void showBoard(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int val : row)
            cout << (val == 0 ? "_ " : to_string(val) + " ");
        cout << "\n";
    }
    cout << "------\n";
}

struct CompareNodes {
    bool operator()(PuzzleNode* a, PuzzleNode* b) {
        return a->heuristic > b->heuristic;
    }
};

void traceSolution(PuzzleNode* node) {
    if (!node) return;
    traceSolution(node->parent);
    showBoard(node->board);
}

bool isSolvable(const vector<vector<int>>& puzzle) {
    vector<int> flat;
    for (const auto& row : puzzle)
        for (int val : row)
            if (val != 0)
                flat.push_back(val);

    int inversions = 0;
    for (size_t i = 0; i < flat.size(); ++i)
        for (size_t j = i + 1; j < flat.size(); ++j)
            if (flat[i] > flat[j])
                inversions++;

    return inversions % 2 == 0;
}

void solvePuzzle(vector<vector<int>> start, int blankX, int blankY) {
    if (!isSolvable(start)) {
        cout << "This puzzle configuration is unsolvable.\n";
        return;
    }

    priority_queue<PuzzleNode*, vector<PuzzleNode*>, CompareNodes> openSet;
    PuzzleNode* root = new PuzzleNode(start, blankX, blankY, 0, nullptr);
    root->heuristic = computeHeuristic(start);
    openSet.push(root);

    unordered_set<string> visited;

    while (!openSet.empty()) {
        PuzzleNode* current = openSet.top();
        openSet.pop();

        string stateKey;
        for (const auto& row : current->board)
            for (int val : row)
                stateKey += to_string(val);

        if (visited.count(stateKey)) continue;
        visited.insert(stateKey);

        if (current->heuristic == 0) {
            cout << "Solution Path:\n";
            traceSolution(current);
            return;
        }

        for (int dir = 0; dir < 4; ++dir) {
            int newX = current->blankX + dx[dir];
            int newY = current->blankY + dy[dir];

            if (withinBounds(newX, newY)) {
                vector<vector<int>> nextBoard = current->board;
                swap(nextBoard[current->blankX][current->blankY], nextBoard[newX][newY]);

                string nextKey;
                for (const auto& row : nextBoard)
                    for (int val : row)
                        nextKey += to_string(val);

                if (!visited.count(nextKey)) {
                    PuzzleNode* child = new PuzzleNode(nextBoard, newX, newY, current->depth + 1, current);
                    child->heuristic = computeHeuristic(nextBoard);
                    openSet.push(child);
                }
            }
        }
    }

    cout << "No solution could be found.\n";
}

int main() {
    vector<vector<int>> initialBoard = {
        {1, 2, 3},
        {4, 0, 6},
        {7, 5, 8}
    };

    int blankRow = 1, blankCol = 1;

    solvePuzzle(initialBoard, blankRow, blankCol);

    return 0;
}