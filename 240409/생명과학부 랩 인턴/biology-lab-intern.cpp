#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>

#define DIR_NUM 4
#define MAX_N 101
#define MAX_M 101
using namespace std;

int n, m, k;
vector<tuple<int, int, int>> mold[MAX_N][MAX_M];
vector<tuple<int, int, int>> copy_mold[MAX_N][MAX_M];
int ans;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < m;
}

void Catch(int col) {
    for (int i = 0; i < n; i++) {
        if (mold[i][col].size() != 0) {
            int b;
            tie(b, ignore, ignore) = mold[i][col][0];
            ans += b;
            mold[i][col].clear();
            return;
        }
    }
}

void Move(int x, int y) {
    int dx[DIR_NUM] = { -1, 1, 0, 0 }, dy[DIR_NUM] = { 0, 0, 1, -1 };

    for (int i = 0; i < (int)mold[x][y].size(); i++) {
        int b, dir, s;
        tie(b, dir, s) = mold[x][y][i];

        for (int j = 1; j <= s; j++) {
            int new_x = x + dx[dir];
            int new_y = y + dy[dir];
            if (!InRange(new_x, new_y)) {
                dir = (dir < 2) ? (1 - dir) : (5 - dir);
            }
            x = x + dx[dir];
            y = y + dy[dir];
        }
        copy_mold[x][y].push_back(make_tuple(b, dir, s));
    }
}

void MoveAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            copy_mold[i][j].clear();
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mold[i][j].size() != 0) {
                Move(i, j);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mold[i][j] = copy_mold[i][j];
        }
    }
}

void Eat() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (mold[i][j].size() > 1) {
                sort(mold[i][j].rbegin(), mold[i][j].rend());
                while (mold[i][j].size() != 1) mold[i][j].pop_back();
            }
        }
    }
}

void Simulate() {
    for (int i = 0; i < m; i++) {
        Catch(i);
        /*cout << "After catch\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << mold[i][j].size() << " ";
            }
            cout << "\n";
        }
        cout << "\n";*/
        MoveAll();
        /*cout << "After Move\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << mold[i][j].size() << " ";
            }
            cout << "\n";
        }
        cout << "\n";*/
        Eat();
        /*cout << "After Eat\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << mold[i][j].size() << " ";
            }
            cout << "\n";
        }
        cout << "\n";*/
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for (int i = 0; i < k; i++) {
        int x, y, s, d, b;
        cin >> x >> y >> s >> d >> b;
        mold[x-1][y-1].push_back(make_tuple(b, d - 1, s));
    }
    Simulate();
    cout << ans;
    return 0;
}