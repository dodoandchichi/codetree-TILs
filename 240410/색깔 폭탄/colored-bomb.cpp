#include <iostream>
#include <tuple>
#include <queue>

#define MAX_N 21
#define DIR_NUM 4
#define Bundle tuple<int, int, int, int>
#define EMPTY_BUNDLE make_tuple(-1, -1, -1, -1)
#define EMPTY -2
using namespace std;

int n, m;
int bomb[MAX_N][MAX_N];
int copy_bomb[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];
queue<pair<int, int>> q;
bool visited[MAX_N][MAX_N];
int ans;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Init() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[i][j] = false;
        }
    }
}

void BFS(int color) {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    
    while (!q.empty()) {
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for (int i = 0; i < DIR_NUM; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (InRange(new_x, new_y) && !visited[new_x][new_y] && (bomb[new_x][new_y] == color 
                || bomb[new_x][new_y] == 0)) {
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
            }
        }
    }
}

Bundle FindBestBundle(int x, int y) {
    int red_cnt = 0, bomb_cnt = 0;

    Init();
    q.push(make_pair(x, y));
    visited[x][y] = true;
    BFS(bomb[x][y]);

    pair<int, int> standard = make_pair(-1, -1);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (visited[i][j]) {
                bomb_cnt++;
                if (bomb[i][j] == 0) red_cnt++;
                else {
                    if (make_pair(i, -j) > standard) standard = make_pair(i, -j);
                }
            }
        }
    }
    int sx, sy;
    tie(sx, sy) = standard;
    return make_tuple(bomb_cnt, -red_cnt, sx, sy);
}

void Remove(int x, int y) {
    Init();
    q.push(make_pair(x, y));
    visited[x][y] = true;
    BFS(bomb[x][y]);

    int cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (visited[i][j]) {
                bomb[i][j] = EMPTY;
                cnt++;
            }
        }
    }
    ans += cnt * cnt;
}

void Rotate() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_bomb[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_bomb[n-j-1][i] = bomb[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            bomb[i][j] = copy_bomb[i][j];
        }
    }
}

void Gravity() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i][j] = EMPTY;
        }
    }

    for (int j = 0; j < n; j++) {
        int temp_idx = n - 1;
        for (int i = n - 1; i >= 0; i--) {
            if (bomb[i][j] >= 0) {
                temp[temp_idx--][j] = bomb[i][j];
            }
            else if (bomb[i][j] == -1) {
                temp[i][j] = bomb[i][j];
                temp_idx = i - 1;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            bomb[i][j] = temp[i][j];
        }
    }
}

void Print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << bomb[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Simulate() {
    while (1) {
        Bundle best_bundle = EMPTY_BUNDLE;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (bomb[i][j] >= 0) {
                    Bundle cur_bundle = FindBestBundle(i, j);
                    if (best_bundle < cur_bundle) best_bundle = cur_bundle;
                }
            }
        }

        int bomb_cnt, red_cnt, x, y;
        tie(bomb_cnt, red_cnt, x, y) = best_bundle;
        
        if (bomb_cnt < 2 || bomb_cnt == -red_cnt) break;

        Remove(x, -y);
        //cout << "After Remove\n";
        //Print();
        Gravity();
        //cout << "After Gravity\n";
        //Print();
        Rotate();
        //cout << "After Rotate\n";
        //Print();
        Gravity();
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> bomb[i][j];
        }
    }
    Simulate();
    cout << ans;
    return 0;
}


/*
    이것도 한번만에 통과,,,, 좋다
*/