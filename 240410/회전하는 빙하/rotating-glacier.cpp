#include <iostream>
#include <queue>
#include <algorithm>

#define MAX_N 128
#define DIR_NUM 4
using namespace std;

int n, cycle, grid_size;
int grid[MAX_N][MAX_N];
int copy_grid[MAX_N][MAX_N];
int level;
queue<pair<int, int>> q;
bool visited[MAX_N][MAX_N];

bool InRange(int x, int y) {
    return 0 <= x && x < grid_size && 0 <= y && y < grid_size;
}

void RotateSquare(int x, int y, int square_size, int dir) {
    int dx[DIR_NUM] = { 0, 1, -1, 0 }, dy[DIR_NUM] = { 1, 0, 0, -1 };

    for (int row = x; row < x + square_size; row++) {
        for (int col = y; col < y + square_size; col++) {
            int rx = row + dx[dir] * square_size;
            int ry = col + dy[dir] * square_size;
            copy_grid[rx][ry] = grid[row][col];
        }
    }
}

void Rotate() {
    int box_size = 1 << level;
    int half_size = box_size / 2;

    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            copy_grid[i][j] = 0;
        }
    }

    for (int i = 0; i < grid_size; i+=box_size) {
        for (int j = 0; j < grid_size; j+=box_size) {
            RotateSquare(i, j, half_size, 0);
            RotateSquare(i, j + half_size, half_size, 1);
            RotateSquare(i + half_size, j, half_size, 2);
            RotateSquare(i + half_size, j + half_size, half_size, 3);
        }
    }

    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            grid[i][j] = copy_grid[i][j];
        }
    }
}

void Init() {
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            visited[i][j] = false;
        }
    }
}

void Melt(int x, int y) {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    int cnt = 0;

    for (int i = 0; i < DIR_NUM; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (InRange(new_x, new_y) && grid[new_x][new_y]) {
            cnt++;
        }
    }

    if (cnt < 3) visited[x][y] = true;
}

void MeltAll() {
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (grid[i][j] > 0) Melt(i, j);
        }
    }

    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (visited[i][j]) grid[i][j]--;
        }
    }
}

void Simulate() {
    if(level > 0) Rotate();
    // cout << "After Rotate\n";
    // for (int i = 0; i < grid_size; i++) {
    //     for (int j = 0; j < grid_size; j++) {
    //         cout << grid[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";
    MeltAll();
    // cout << "After Melt\n";
    // for (int i = 0; i < grid_size; i++) {
    //     for (int j = 0; j < grid_size; j++) {
    //         cout << grid[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";
    Init();
}

int BFS() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    int ice = 1;

    while (!q.empty()) {
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for (int i = 0; i < DIR_NUM; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (InRange(new_x, new_y) && !visited[new_x][new_y] && grid[new_x][new_y] > 0) {
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
                ice++;
            }
        }
    }
    return ice;
}
int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> cycle;
    grid_size = (1 << n);
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            cin >> grid[i][j];
        }
    }

    while (cycle--) {
        cin >> level;
        Simulate();
    }

    int ice_count = 0;
    int ice_house = 0;
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            if (grid[i][j]) ice_count += grid[i][j];

            if (visited[i][j] || grid[i][j] == 0) continue;

            visited[i][j] = true;
            q.push(make_pair(i, j));
            ice_house = max(ice_house, BFS());
        }
    }
    cout << ice_count << "\n" << ice_house;
    return 0;
}

/*
    회전하는 것 다시 보기
*/