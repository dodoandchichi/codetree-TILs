#include <iostream>

#define MAX_N 15
#define DIR_NUM 8
using namespace std;

int n, m;
int tree[MAX_N][MAX_N];
int copy_tree[MAX_N][MAX_N];
int food[MAX_N][MAX_N];
int copy_food[MAX_N][MAX_N];
int d, p;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Move(int x, int y) {
    int dx[DIR_NUM] = { 0, -1, -1, -1, 0, 1, 1, 1 };
    int dy[DIR_NUM] = { 1, 1, 0, -1, -1, -1, 0, 1 };

    int new_x = (x + dx[d - 1] * p + n) % n;
    int new_y = (y + dy[d - 1] * p + n) % n;

    copy_food[new_x][new_y] = 1;
}

void MoveFoodAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_food[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (food[i][j] == 1) Move(i, j);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            food[i][j] = copy_food[i][j];
        }
    }
}

void Grow(int x, int y){
    int dx[4] = { -1, -1, 1, 1 };
    int dy[4] = { -1, 1, -1, 1 };
    int cnt = 0;

    for (int i = 0; i < 4; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (InRange(new_x, new_y) && tree[new_x][new_y]) {
            cnt++;
        }
    }

    copy_tree[x][y] += cnt;
}

void GrowAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_tree[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (food[i][j] == 1) tree[i][j]++;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (food[i][j] == 1) Grow(i, j);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tree[i][j] += copy_tree[i][j];
        }
    }
}

void CutFood() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!food[i][j] && tree[i][j] >= 2) {
                tree[i][j] -= 2;
                food[i][j] = 1;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (copy_food[i][j]) food[i][j] = 0;
        }
    }
}
    
void Simulate() {
    MoveFoodAll();
    GrowAll();
    CutFood();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> tree[i][j];
        }
    }

    food[n - 2][0] = 1, food[n - 1][0] = 1;
    food[n - 2][1] = 1, food[n - 1][1] = 1;

    while (m--) {
        cin >> d >> p;
        Simulate();
    }
    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ans += tree[i][j];
        }
    }
    cout << ans;
    return 0;
}


/*
    검토를 잘해서 한번에 히든 통과
    문제를 잘 읽자
*/