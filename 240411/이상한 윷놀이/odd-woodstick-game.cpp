#include <iostream>
#include <vector>
#include <tuple>

#define MAX_N 13
#define DIR_NUM 4
#define BLUE 2
#define RED 1
using namespace std;

int n, k;
int grid[MAX_N][MAX_N];
vector<pair<int, int>> pieces[MAX_N][MAX_N]; // 번호, 방향
int dx[DIR_NUM] = { 0, 0, -1, 1 };
int dy[DIR_NUM] = { 1, -1, 0, 0 };

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

tuple<int, int, int> FindPieces(int num) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (pieces[i][j].size() > 0) {
                for (int k = 0; k < (int)pieces[i][j].size(); k++) {
                    if (pieces[i][j][k].first == num)
                        return make_tuple(i, j, pieces[i][j][k].second);
                }
            }
        }
    }
}

vector<pair<int, int>> PopPieces(int x, int y, int num) {
    vector<pair<int, int>> copy_pieces;

    for (int i = 0; i < (int)pieces[x][y].size(); i++) {
        if (pieces[x][y][i].first == num) {
            copy_pieces.assign(pieces[x][y].begin() + i, pieces[x][y].end());
            pieces[x][y].erase(pieces[x][y].begin() + i, pieces[x][y].end());
            break;
        }
    }
    return copy_pieces;
}

void Move(int x, int y, vector<pair<int, int>> copy_pieces, bool reversed) {
    if (reversed) reverse(copy_pieces.begin(), copy_pieces.end());

    pieces[x][y].insert(pieces[x][y].end(), copy_pieces.begin(), copy_pieces.end());
}

bool Simulate() {
    for (int num = 1; num <= k; num++) {
        bool reversed = false;
        int x, y, dir;
        tie(x, y, dir) = FindPieces(num);

        int new_x = x + dx[dir];
        int new_y = y + dy[dir];

        if (!InRange(new_x, new_y) || grid[new_x][new_y] == BLUE) {
            dir = (dir < 2) ? (1 - dir) : (5 - dir);
            new_x = x + dx[dir];
            new_y = y + dy[dir];
            if (!InRange(new_x, new_y) || grid[new_x][new_y] == BLUE) {
                new_x = x, new_y = y;
            }
            else if (grid[new_x][new_y] == RED) {
                reversed = true;
            }
        }
        else if (grid[new_x][new_y] == RED) reversed = true;

        vector<pair<int, int>> copy_pieces = PopPieces(x, y, num);

        copy_pieces[0] = make_pair(num, dir);

        Move(new_x, new_y, copy_pieces, reversed);

        if (pieces[new_x][new_y].size() >= 4) return true;

        /*for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << pieces[i][j].size() << " ";
            }
            cout << "\n";
        }
        cout << "\n";*/
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    for (int i = 1; i <= k; i++) {
        int x, y, d;
        cin >> x >> y >> d;
        pieces[x - 1][y - 1].push_back(make_pair(i, d - 1));
    }

    int turn = 1;
    while (1) {
        bool end = Simulate();
        if (end || turn > 1000) break;
        turn++;
    }
    if (turn > 1000) cout << "-1";
    else cout << turn;
    return 0;
}