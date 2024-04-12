#include <iostream>
#include <tuple>

#define MAX_N 21
#define DIR_NUM 4
#define EMPTY make_tuple(-1, -1, -1)
using namespace std;

int n, m, k, c;
int tree[MAX_N][MAX_N];
int copy_tree[MAX_N][MAX_N];
bool wall[MAX_N][MAX_N];
int drug[MAX_N][MAX_N];
int ans;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << tree[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Grow(int x, int y) {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    int cnt = 0;
    for (int i = 0; i < DIR_NUM; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (InRange(new_x, new_y) && tree[new_x][new_y] > 0) {
            cnt++;
        }
    }
    copy_tree[x][y] = tree[x][y] + cnt;
}

void GrowAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_tree[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(tree[i][j] > 0) Grow(i, j);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tree[i][j] = copy_tree[i][j];
        }
    }
}

void Breed(int x, int y) {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    int cnt = 0;
    copy_tree[x][y] = tree[x][y];

    for (int i = 0; i < DIR_NUM; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (InRange(new_x, new_y) && tree[new_x][new_y] == 0 && drug[new_x][new_y] == 0 && !wall[new_x][new_y]) {
            cnt++;
        }
    }
    
    for (int i = 0; i < DIR_NUM; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (InRange(new_x, new_y) && tree[new_x][new_y] == 0 && drug[new_x][new_y] == 0 && !wall[new_x][new_y]) {
            copy_tree[new_x][new_y] += (copy_tree[x][y] / cnt);
        }
    }
}

void BreedAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_tree[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(tree[i][j] > 0) Breed(i, j);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tree[i][j] = copy_tree[i][j];
        }
    }
}

tuple<int, int, int> Drug(int x, int y) {
    int kill = 0;
    int dx[DIR_NUM] = { -1, -1, 1, 1 }, dy[DIR_NUM] = { -1, 1, -1, 1 };
    kill += tree[x][y];

    bool canGo[4] = { true, true, true, true };

    for (int dir = 0; dir < DIR_NUM; dir++) {
        for (int i = 1; i <= k; i++) {
            int new_x = x + dx[dir] * i;
            int new_y = y + dy[dir] * i;
            if (InRange(new_x, new_y) && canGo[dir]) {
                if (tree[new_x][new_y] > 0)
                    kill += tree[new_x][new_y];
                else if(tree[new_x][new_y] == 0 || wall[new_x][new_y]) canGo[dir] = false;
            }
        }
    }

    return make_tuple(kill, -x, -y);
}

void Kill(int x, int y) {
    int dx[DIR_NUM] = { -1, -1, 1, 1 }, dy[DIR_NUM] = { -1, 1, -1, 1 };
    ans += tree[x][y];
    tree[x][y] = 0;
    drug[x][y] = c + 1;
    bool canGo[4] = { true, true, true, true };

    for (int dir = 0; dir < DIR_NUM; dir++) {
        for (int i = 1; i <= k; i++) {
            int new_x = x + dx[dir] * i;
            int new_y = y + dy[dir] * i;
            if (InRange(new_x, new_y) && canGo[dir]) {
                drug[new_x][new_y] = c + 1;
                if (tree[new_x][new_y] > 0) {
                    ans += tree[new_x][new_y];
                    tree[new_x][new_y] = 0;
                }
                else if (tree[new_x][new_y] == 0 || wall[new_x][new_y]) canGo[dir] = false;
            }
        }
    }
}

void FindMax() {
    tuple<int, int, int> best_kill = EMPTY;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tree[i][j] > 0) {
                tuple<int, int, int> cur_kill = Drug(i, j); // 나무가없어도 뿌려야될까?
                if (cur_kill > best_kill) best_kill = cur_kill;
            }
        }
    }

    int x, y;
    tie(ignore, x, y) = best_kill;
    Kill(-x, -y);
}

void DecDrug() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(drug[i][j] > 0) drug[i][j]--;
        }
    }
}

void Simulate() {
    GrowAll();
    /*cout << "After grow\n";
    Print();*/
    BreedAll();
    /*cout << "After Breed\n";
    Print();*/
    FindMax();
    DecDrug();
    /*cout << "After drug\n";
    Print();*/
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k >> c;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> tree[i][j];
            if (tree[i][j] == -1) wall[i][j] = true;
        }
    }

    while (m--) {
        Simulate();
    }
    cout << ans;
    return 0;
}