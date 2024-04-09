#include <iostream>

#define MAX_N 51
#define MAX_M 51
#define DIR_NUM 4
using namespace std;

int n, m, t;
int grid[MAX_N][MAX_M];
int copy_grid[MAX_N][MAX_M];

int dx[DIR_NUM] = { -1, 0, 1, 0 };
int dy[DIR_NUM] = { 0, 1, 0, -1 };

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < m;
}

void AddDust(int x, int y) {
    for (int i = 0; i < DIR_NUM; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (InRange(new_x, new_y) && grid[new_x][new_y] != -1) {
            copy_grid[new_x][new_y] += grid[x][y] / 5;
            copy_grid[x][y] -= grid[x][y] / 5;
        }
    }
}

void AddDustAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            copy_grid[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] > 0) AddDust(i, j);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            grid[i][j] += copy_grid[i][j];
        }
    }
}

void CCW_Wind(int x, int y) {
    for (int row = x; row > 0; row--) {
        grid[row][y] = grid[row - 1][y];
    }

    for (int col = y; col < m-1; col++) {
        grid[0][col] = grid[0][col+1];
    }

    for (int row = 0; row < x; row++) {
        grid[row][m-1] = grid[row + 1][m-1];
    }

    for (int col = m-1; col > y; col--) {
        grid[x][col] = grid[x][col - 1];
    }

    grid[x][y] = -1;
    grid[x][y + 1] = 0;
}

void CW_Wind(int x, int y) {
    for (int row = x; row < n - 1; row++) {
        grid[row][y] = grid[row + 1][y];
    }

    for (int col = y; col < m - 1; col++) {
        grid[n-1][col] = grid[n-1][col + 1];
    }

    for (int row = n-1; row > x; row--) {
        grid[row][m-1] = grid[row - 1][m-1];
    }

    for (int col = m - 1; col > y; col--) {
        grid[x][col] = grid[x][col - 1];
    }

    grid[x][y] = -1;
    grid[x][y + 1] = 0;
}

void FindWind() {
    int upper_wind = true;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == -1 && upper_wind) {
                upper_wind = false;
                CCW_Wind(i, j);
            }
            else if (grid[i][j] == -1) {
                CW_Wind(i, j);
            }
        }
    }
}

void Simulate() {
    AddDustAll();
    /*cout << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    FindWind();
    /*cout << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> t;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    while (t--) {
        Simulate();
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if(grid[i][j] > 0) ans += grid[i][j];
        }
    }
    cout << ans;
    return 0;
}