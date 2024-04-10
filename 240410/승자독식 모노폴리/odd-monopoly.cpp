#include <iostream>
#include <tuple>

#define MAX_N 21
#define DIR_NUM 4
#define EMPTY make_pair(-1, -1)
using namespace std;

int n, m, k;
int grid[MAX_N][MAX_N];
pair<int, int> player[MAX_N][MAX_N]; // 번호, 방향
pair<int, int> copy_player[MAX_N][MAX_N]; // 번호, 방향
pair<int, int> contract[MAX_N][MAX_N]; // 번호, 유효기간
int next_dir[MAX_N * MAX_N][DIR_NUM][DIR_NUM];

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;

}
void Update(pair<int, int> player1, pair<int, int> player2, int x, int y) {
    int num1, dir1;
    tie(num1, dir1) = player1;

    int num2, dir2;
    tie(num2, dir2) = player2;
    //cout << num1 << " " << num2 << "\n";
    if (player1 > player2) {
        copy_player[x][y] = make_pair(num2, dir2);
        /*cout << "after update1\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << copy_player[i][j].first << " ";
            }
            cout << "\n";
        }
        cout << "\n";*/
    }
    else {
        copy_player[x][y] = make_pair(num1, dir1);
        /*cout << "after update2\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << copy_player[i][j].first << " ";
            }
            cout << "\n";
        }
        cout << "\n";*/
    }
}

void Move(int x, int y) {
    int dx[DIR_NUM] = { -1, 1, 0, 0 }, dy[DIR_NUM] = { 0, 0, -1, 1 };

    int num, cur_dir;
    tie(num, cur_dir) = player[x][y];

    for (int i = 0; i < DIR_NUM; i++) {
        int dir = next_dir[num][cur_dir][i];
        //cout << "num: " << num << "dir: " << dir << "\n";
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];
        if (InRange(new_x, new_y) && contract[new_x][new_y] == EMPTY) {
            //cout << "check " << num << "\n";
            player[x][y] = make_pair(num, dir);
            if (copy_player[new_x][new_y] != EMPTY)
                Update(copy_player[new_x][new_y], player[x][y], new_x, new_y);
            else copy_player[new_x][new_y] = make_pair(num, dir);
            return;
        }
    }

    for (int i = 0; i < DIR_NUM; i++) {
        int dir = next_dir[num][cur_dir][i];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];
        if (InRange(new_x, new_y) && contract[new_x][new_y].first == num) {
            player[x][y] = make_pair(num, dir);
            if (copy_player[new_x][new_y] != EMPTY)
                Update(copy_player[new_x][new_y], player[x][y], new_x, new_y);
            else copy_player[new_x][new_y] = make_pair(num, dir);
            return;
        }
    }
}

void MoveAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_player[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (player[i][j] != EMPTY) {
                Move(i, j);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            player[i][j] = copy_player[i][j];
        }
    }
}   

void DecContract() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (contract[i][j] != EMPTY) {
                int num, period;
                tie(num, period) = contract[i][j];
                period--;
                if (period == 0) contract[i][j] = EMPTY;
                else contract[i][j] = make_pair(num, period);
            }
        }
    }
}

void AddContract() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (player[i][j] != EMPTY) {
                int num;
                tie(num, ignore) = player[i][j];
                contract[i][j] = make_pair(num, k);
            }
        }
    }
}

void Simulate() {
    MoveAll();
    /*cout << "after move\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << player[i][j].first << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    DecContract();
    AddContract();
}

bool End() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (player[i][j] != EMPTY) {
                int num;
                tie(num, ignore) = player[i][j];

                if (num != 1) return false;
            }
        }
    }
    return true;
}


int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
            player[i][j] = EMPTY;
            contract[i][j] = EMPTY;
        }
    }

    for (int num = 1; num <= m; num++) {
        int dir;
        cin >> dir;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == num) {
                    player[i][j] = make_pair(num, dir - 1);
                    contract[i][j] = make_pair(num, k);
                }
            }
        }
    }

    for (int num = 1; num <= m; num++) {
        for (int i = 0; i < DIR_NUM; i++) {
            for (int j = 0; j < DIR_NUM; j++) {
                cin >> next_dir[num][i][j];
                next_dir[num][i][j]--;
            }
        }
    }
    int turn = 1;
    while (1) {
        Simulate();
        if (End() || turn >= 1000) break;
        turn++;
    }
    if (turn == 1000) cout << "-1";
    else cout << turn;
    return 0;
}


/*
    히든 테케는 한번에 통과되었지만, 자꾸 EMPTY랑 0을 착각하는 실수,,,
    조심해야겠다
*/