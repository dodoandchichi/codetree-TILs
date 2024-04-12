#include <iostream>
#include <tuple>
#include <vector>

#define MAX_N 4
#define MAX_T 26
#define DIR_NUM 8
#define P_DIR_NUM 4
#define MAX_DECAY 2
using namespace std;

int n = 4, m, t;
int mon[MAX_T][MAX_N][MAX_N][DIR_NUM];
int temp[MAX_T][MAX_N][MAX_N][DIR_NUM];
int copy_mon[MAX_T][MAX_N][MAX_N][DIR_NUM];
int dead[MAX_N][MAX_N][MAX_DECAY+1];
int turn;

pair<int, int> pack;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

bool CanMonGo(int x, int y) {
    return InRange(x, y) && !dead[x][y][0] && !dead[x][y][1] && pack != make_pair(x, y);
}

void Print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int cnt = 0;
            for (int k = 0; k < DIR_NUM; k++) {
                if (mon[turn][i][j][k])
                    cnt += mon[turn][i][j][k];
            }
            cout << cnt << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Mon_Move(int x, int y, int dir, int cnt) {
    int dx[DIR_NUM] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    int dy[DIR_NUM] = { 0, -1, -1, -1, 0, 1, 1, 1 };

    int new_x = x + dx[dir];
    int new_y = y + dy[dir];

    for (int i = 0; i < DIR_NUM; i++) {
        if (!CanMonGo(new_x, new_y)) {
            dir = (dir + 1) % 8;
            new_x = x + dx[dir];
            new_y = y + dy[dir];
        }   
        else break;
    }

    new_x = x + dx[dir];
    new_y = y + dy[dir];
    if (CanMonGo(new_x, new_y)) copy_mon[turn][new_x][new_y][dir] += cnt;
    else copy_mon[turn][x][y][dir] += cnt;

}

void Duplicate() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < DIR_NUM; k++) {
                temp[turn][i][j][k] = mon[turn][i][j][k];
            }
        }
    }
    cout << "temp\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int cnt = 0;
            for (int k = 0; k < DIR_NUM; k++) {
                if (temp[turn][i][j][k]) cnt+= temp[turn][i][j][k];
            }
            cout << cnt << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Mon_MoveAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < DIR_NUM; k++) {
                copy_mon[turn][i][j][k] = 0;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < DIR_NUM; k++) {
                if (mon[turn][i][j][k]) {
                    Mon_Move(i, j, k, mon[turn][i][j][k]);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < DIR_NUM; k++) {
                mon[turn][i][j][k] = copy_mon[turn][i][j][k];
            }
        }
    }
}

int FindKillNum(int dir1, int dir2, int dir3) {
    int kill = 0;
    int dir[] = { dir1, dir2, dir3 };
    int dx[P_DIR_NUM] = { -1, 0, 1, 0 }, dy[P_DIR_NUM] = { 0, -1, 0, 1 };
    bool visit = false;
    int cur_x, cur_y;
    tie(cur_x, cur_y) = pack;

    vector<pair<int, int>> v_pos;
    for (int i = 0; i < 3; i++) {
        cur_x = cur_x + dx[dir[i]];
        cur_y = cur_y + dy[dir[i]];
        if (!InRange(cur_x, cur_y)) return -1;
        
        for (int j = 0; j < v_pos.size(); j++) {
            if (v_pos[j] == make_pair(cur_x, cur_y)) visit = true;
        }

        if (!visit) {
            v_pos.push_back(make_pair(cur_x, cur_y));

            for (int dir = 0; dir < DIR_NUM; dir++) {
                if (mon[turn][cur_x][cur_y][dir]) kill += mon[turn][cur_x][cur_y][dir];
            }
        }
    }
    cout << dir1 << " " << dir2 << " " << dir3 << "\n";
    cout << "kill: " << kill << "\n";
    return kill;
}

void GoKill(int dir1, int dir2, int dir3) {
    int dir[] = { dir1, dir2, dir3 };
    int dx[P_DIR_NUM] = { -1, 0, 1, 0 }, dy[P_DIR_NUM] = { 0, -1, 0, 1 };
    cout << "gokill\n";
    cout << dir1 << " " << dir2 << " " << dir3 << "\n";
    int cur_x, cur_y;
    tie(cur_x, cur_y) = pack;

    for (int i = 0; i < 3; i++) {
        cur_x = cur_x + dx[dir[i]];
        cur_y = cur_y + dy[dir[i]];

        int cnt = 0;
        for (int j = 0; j < DIR_NUM; j++) {
            if (mon[turn][cur_x][cur_y][j]) {
                cnt++;
                mon[turn][cur_x][cur_y][j] = 0;
            }
        }
        dead[cur_x][cur_y][MAX_DECAY] = cnt;
    }
    pack = make_pair(cur_x, cur_y);
}

void Pack_Move() {
    int best_kill = -1;
    tuple<int, int, int> best_route;

    for (int i = 0; i < P_DIR_NUM; i++) {
        for (int j = 0; j < P_DIR_NUM; j++) {
            for (int k = 0; k < P_DIR_NUM; k++) {
                int cur_kill = FindKillNum(i, j, k);
                if (best_kill < cur_kill) {
                    best_kill = cur_kill;
                    best_route = make_tuple(i, j, k);
                }
            }
        }
    }

    int dir1, dir2, dir3;
    tie(dir1, dir2, dir3) = best_route;
    GoKill(dir1, dir2, dir3);
}

void Dead_Decay() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < MAX_DECAY; k++) {
                dead[i][j][k] = dead[i][j][k + 1];
            }
            dead[i][j][MAX_DECAY] = 0;
        }
    }
}

void Breed() {
    cout << "temp\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int cnt = 0;
            for (int k = 0; k < DIR_NUM; k++) {
                if (temp[turn][i][j][k]) cnt += temp[turn][i][j][k];
            }
            cout << cnt << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    cout << "mon\n";
    Print();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < DIR_NUM; k++) {
                mon[turn+1][i][j][k] = temp[turn][i][j][k] + mon[turn][i][j][k];
            }
        }
    }

    cout << "next mon\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int cnt = 0;
            for (int k = 0; k < DIR_NUM; k++) {
                if (mon[turn+1][i][j][k]) cnt += mon[turn+1][i][j][k];
            }
            cout << cnt << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Simulate() {
    Duplicate();
    Print();
    Mon_MoveAll();
    Print();
    Pack_Move();
    Print();
    Dead_Decay();
    Breed();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> m >> t;

    int r, c, d;
    cin >> r >> c;
    pack = make_pair(r - 1, c - 1);
    for (int i = 0; i < m; i++) {
        cin >> r >> c >> d;
        mon[0][r - 1][c - 1][d - 1]++;
    }

    
    for(int i=0; i<t; i++) {
        turn = i;
        Simulate();
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < DIR_NUM; k++) {
                ans += mon[t][i][j][k];
            }
        }
    }

    cout << "fianl\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int cnt = 0;
            for (int k = 0; k < DIR_NUM; k++) {
                if (mon[t][i][j][k]) cnt += mon[t][i][j][k];
            }
            cout << cnt << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    cout << ans;
    return 0;
}