#include <iostream>
#include <vector>
#include <tuple>

#define MAX_N 51
#define DIR_NUM 8

using namespace std;

int n, m, k;
vector<tuple<int, int, int>> atom[MAX_N][MAX_N];
vector<tuple<int, int, int>> copy_atom[MAX_N][MAX_N];

void Move(int x, int y) {
    int dx[DIR_NUM] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    int dy[DIR_NUM] = { 0, 1, 1, 1, 0, -1, -1, -1 };

    int m, s, d;
    tie(m, s, d) = atom[x][y][0];


    int new_x = (x + dx[d] * s + n) % n;
    int new_y = (y + dy[d] * s + n) % n;
    copy_atom[new_x][new_y].push_back(make_tuple(m, s, d));
}

void MoveAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_atom[i][j].clear();
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (atom[i][j].size() > 0) {
                Move(i, j);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            atom[i][j] = copy_atom[i][j];
        }
    }
}

void Merge(int x, int y) {
    bool diag = false, line = false;
    int sum_m = 0, sum_s = 0, cnt = 0;
    for (int i = 0; i < (int)atom[x][y].size(); i++) {
        int m, s, d;
        tie(m, s, d) = atom[x][y][i];
        sum_m += m, sum_s += s, cnt += 1;
        if (d % 2 == 0) line = true;
        else diag = true;
    }

    int new_m = sum_m / 5;
    int new_s = sum_s / cnt;
    if (diag && line && new_m > 0) {
        for (int i = 0; i < 4; i++) {
            copy_atom[x][y].push_back(make_tuple(new_m, new_s, i+1));
        }
    }
    else if (new_m > 0) {
        for (int i = 0; i < 4; i++) {
            copy_atom[x][y].push_back(make_tuple(new_m, new_s, i));
        }
    }
}

void MergeAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_atom[i][j].clear();
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (atom[i][j].size() > 1) {
                Merge(i, j);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            atom[i][j] = copy_atom[i][j];
        }
    }
}

void Simulate() {
    MoveAll();
    /*cout << "After move\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << atom[i][j].size() << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    MergeAll();
    /*cout << "After merge\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << atom[i][j].size() << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for (int i = 0; i < m; i++) {
        int x, y, m, s, d;
        cin >> x >> y >> m >> s >> d;
        atom[x - 1][y - 1].push_back(make_tuple(m, s, d));
    }

    while (k--) {
        Simulate();
    }
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (atom[i][j].size() > 0) {
                for (int k = 0; k < (int)atom[i][j].size(); k++) {
                    int m;
                    tie(m, ignore, ignore) = atom[i][j][k];
                    ans += m;
                }
            }
        }
    }
    cout << ans;
    return 0;
}