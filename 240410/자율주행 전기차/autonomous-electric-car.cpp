#include <iostream>
#include <queue>
#include <tuple>

#define MAX_N 21
#define DIR_NUM 4
#define EMPTY make_pair(-1, -1)
using namespace std;

int n, m, c;
int grid[MAX_N][MAX_N];
pair<int, int> car;
pair<int, int> guest[MAX_N * MAX_N];
pair<int, int> goal[MAX_N * MAX_N];
bool visited[MAX_N][MAX_N];
int step[MAX_N][MAX_N];
queue<pair<int, int>> q;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Init() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[i][j] = false;
            step[i][j] = 0;
        }
    }
}

void BFS() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };

    Init();
    q.push(car);
    visited[car.first][car.second] = true;

    while (!q.empty()) {
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for (int i = 0; i < DIR_NUM; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (InRange(new_x, new_y) && !visited[new_x][new_y] && grid[new_x][new_y] != 1) {
                visited[new_x][new_y] = 1;
                step[new_x][new_y] = step[x][y] + 1;
                q.push(make_pair(new_x, new_y));
            }
        }
    }
}

bool NeedUpdate(pair<int, int> close_guest, pair<int, int> cur_guest) {
    if (close_guest == EMPTY) return false;

    int close_x, close_y;
    tie(close_x, close_y) = close_guest;

    int cur_x, cur_y;
    tie(cur_x, cur_y) = cur_guest;

    return make_tuple(step[close_x][close_y], close_x, close_y) <
        make_tuple(step[cur_x][cur_y], cur_x, cur_y);
}

void Simulate() {
    for (int num = 0; num < m; num++) {
        BFS();
        pair<int, int> close_guest = EMPTY;
        int guest_idx = -1;

        for (int i = 0; i < m; i++) {
            pair<int, int> cur_guest = guest[i];
            if (!NeedUpdate(close_guest, cur_guest) && visited[guest[i].first][guest[i].second]) {
                close_guest = cur_guest;
                guest_idx = i;
            }
        }
        
        int sx, sy;
        tie(sx, sy) = close_guest;

        if (step[sx][sy] > c || close_guest == EMPTY) break;

        car = make_pair(sx, sy);
        guest[guest_idx] = EMPTY;
        c -= step[sx][sy];
        BFS();

        int ex, ey;
        tie(ex, ey) = goal[guest_idx];
        if (step[ex][ey] > c || !visited[ex][ey]) break;

        car = make_pair(ex, ey);
        c += step[ex][ey];
        goal[guest_idx] = EMPTY;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> c;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }
    int car_x, car_y;
    cin >> car_x >> car_y;
    car = make_pair(car_x - 1, car_y - 1);

    for (int i = 0; i < m; i++) {
        int xs, ys, xe, ye;
        cin >> xs >> ys >> xe >> ye;
        guest[i] = make_pair(xs - 1, ys - 1);
        goal[i] = make_pair(xe - 1, ye - 1);
    }

    Simulate();

    for (int i = 0; i < m; i++) {
        if (guest[i] != EMPTY || goal[i] != EMPTY) c = -1;
    }
    cout << c;
    return 0;
}


/*
    이미 태운 손님은 없애고, 아예 벽으로막혀 못가는 손님 고려하고,
    태웠는데 도착지에 못도착한 경우 등등 visited로 잘 따질 것

    난 히든테케 한번에 통과함
*/