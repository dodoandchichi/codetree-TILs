#include <iostream>
#include <queue>

#define MAX_N 30
#define DIR_NUM 4
using namespace std;

int n;
int grid[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
int group_n;
int group_cnt[MAX_N * MAX_N + 1], group_num[MAX_N * MAX_N + 1][MAX_N * MAX_N + 1];
queue<pair<int, int>> q;
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

void BFS(int target) {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    
    while (!q.empty()) {
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for (int i = 0; i < DIR_NUM; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (InRange(new_x, new_y) && !visited[new_x][new_y] && grid[new_x][new_y] == target) {
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
                group_cnt[group_n]++;
                group_num[new_x][new_y] = group_n;
            }
        }
    }
}

void MakeGroup() {
    group_n = 0;
    Init();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (visited[i][j]) continue;

            group_n++;
            group_cnt[group_n] = 1;
            group_num[i][j] = group_n;
            visited[i][j] = true;
            q.push(make_pair(i, j));
            BFS(grid[i][j]);
        }
    }
}

void GetScore() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    int score = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int dir = 0; dir < DIR_NUM; dir++) {
                int new_x = i + dx[dir];
                int new_y = j + dy[dir];
                if (InRange(new_x, new_y) && (grid[i][j] != grid[new_x][new_y])) {
                    int g1 = group_num[i][j], g2 = group_num[new_x][new_y];
                    // cout << g1 << " " << g2 << "\n";
                    int a1 = group_cnt[g1], a2 = group_cnt[g2];
                    score += (a1 + a2) * grid[i][j] * grid[new_x][new_y];
                }
            }
        }
    }
    // cout << score / 2 << "\n";
    ans += score / 2;
}

void RotateSquare(int sx, int sy, int square_size) {
    for (int x = sx; x < sx + square_size; x++) {
        for (int y = sy; y < sy + square_size; y++) {
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = square_size - ox - 1;
            temp[rx + sx][ry + sy] = grid[x][y];
        }
    }
}

void Rotate() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == n / 2) temp[j][i] = grid[i][j];

            if (i == n / 2) temp[n-j-1][i] = grid[i][j];
        }
    }

    RotateSquare(0, 0, n / 2);
    RotateSquare(0, n/2+1, n / 2);
    RotateSquare(n/2+1, 0, n / 2);
    RotateSquare(n/2+1, n/2+1, n / 2);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = temp[i][j];
        }
    }
}

void Simulate() {
    MakeGroup();
    GetScore();
    Rotate();
   /* cout << "after rotate\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    for (int i = 0; i <= 3; i++) {
        Simulate();
    }
    cout << ans;
    return 0;
}

/*
    회전도 하다보니 한번에 구현 후 히든테케까지 성공하였음
*/