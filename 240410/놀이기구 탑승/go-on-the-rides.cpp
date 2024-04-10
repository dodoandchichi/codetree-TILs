#include <iostream>
#include <tuple>
#include <climits>

#define MAX_N 21
#define DIR_NUM 4
#define Seat tuple<int, int, int, int>
#define EMPTY make_tuple(-1, -1, INT_MIN, INT_MIN)
using namespace std;

int n, std_num;
int grid[MAX_N][MAX_N];
int student[MAX_N * MAX_N];
int likes[MAX_N * MAX_N][MAX_N * MAX_N];

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

Seat FindBestSeat(int x, int y, int num) {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    int like_num = -1, empty_num = -1;

    for (int i = 0; i < DIR_NUM; i++) {
        int new_x = x + dx[i];
        int new_y = y + dy[i];
        if (InRange(new_x, new_y)) {
            for (int j = 0; j < 4; j++) {
                if (grid[new_x][new_y] == likes[num][j]) like_num++;
            }

            if (grid[new_x][new_y] == 0) empty_num++;
        }
    }
    return make_tuple(like_num, empty_num, -x, -y);
}

void Simulate() {
    for (int num = 1; num <= std_num; num++) {
        Seat best_seat = EMPTY;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) {
                    Seat cur_seat = FindBestSeat(i, j, student[num]);
                    if (cur_seat > best_seat) best_seat = cur_seat;
                }
            }
        }

        int x, y;
        tie(ignore, ignore, x, y) = best_seat;
        grid[-x][-y] = student[num];

        //for (int i = 0; i < n; i++) {
        //    for (int j = 0; j < n; j++) {
        //        cout << grid[i][j] << " ";
        //    }
        //    cout << "\n";
        //}
        //cout << "\n";
    }
}

int GetScore() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };
    int score = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int num = grid[i][j];
            int cnt = 0;
            for (int dir = 0; dir < DIR_NUM; dir++) {
                int new_x = i + dx[dir];
                int new_y = j + dy[dir];
                for (int k = 0; k < 4; k++) {
                    if (InRange(new_x, new_y) && grid[new_x][new_y] == likes[num][k]) {
                        cnt++;
                    }
                }
            }
            if (cnt == 1) score += 1;
            else if (cnt == 2) score += 10;
            else if (cnt == 3) score += 100;
            else if (cnt == 4) score += 1000;
        }
    }
    return score;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n;
    std_num = n * n;
    for (int i = 1; i <= std_num; i++) {
        int num;
        cin >> num;
        student[i] = num;
        for (int j = 0; j < 4; j++) {
            int like;
            cin >> like;
            likes[num][j] = like;
        }
    }
    Simulate();
    int ans = GetScore();
    cout << ans;
    return 0;
}


/*
    쉬움 한번에 통과
*/