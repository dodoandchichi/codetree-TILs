#include <iostream>

#define MAX_N 51
#define MAX_M 51
using namespace std;

int n, m, q;
int grid[MAX_N][MAX_M];
int temp[MAX_N][MAX_N];
int x, d, k;
bool removed[MAX_N][MAX_M];
bool normalize;

void Rotate(int num) {
    for (int col = 0; col < m; col++) {
        if (d == 1) {
            temp[num][col] = grid[num][(col+k) % m];
        }
        else if (d == 0) {
            temp[num][col] = grid[num][(col - k + m) % m];
        }
    }

    for (int col = 0; col < m; col++) {
        grid[num][col] = temp[num][col];
    }
}

void Remove() {
    normalize = false;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            removed[i][j] = false;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == grid[i][j + 1] && j != m-1 && grid[i][j]) {
                removed[i][j] = true;
                removed[i][j + 1] = true;
            }

            if (grid[i][0] == grid[i][m - 1] && grid[i][0]) {
                removed[i][0] = true, removed[i][m - 1] = true;
            }

            if (grid[i][j] == grid[i + 1][j] && i != n-1 && grid[i][j]) {
                removed[i][j] = true;
                removed[i + 1][j] = true;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (removed[i][j]) {
                normalize = true;
                grid[i][j] = 0;
            }
        }
    }
}

void Normalize() {
    int sum = 0, cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] > 0) {
                sum += grid[i][j];
                cnt++;
            }
        }
    }

    if (cnt > 0) {
        int average = sum / cnt;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] > average)
                    grid[i][j]--;
                else if (grid[i][j] < average && grid[i][j])
                    grid[i][j]++;
            }
        }
    }
}

void Print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}
void Simulate() {
    for (int i = 0; i < n; i++) {
        if ((i + 1) % x == 0) Rotate(i);
    }

    //cout << "After Roate\n";
    //Print();
    Remove();
    //cout << "After Remove\n";
    //Print();
    if(!normalize) Normalize();
    //cout << "After Normal\n";
    //Print();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> q;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    while (q--) {
        cin >> x >> d >> k;
        Simulate();
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] > 0) ans += grid[i][j];
        }
    }
    cout << ans;
    return 0;
}