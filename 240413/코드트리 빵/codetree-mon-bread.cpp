#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <climits>

#define MAX_N 15
#define MAX_M 31
#define DIR_NUM 4
#define EMPTY make_pair(-1, -1)
using namespace std;

int n, m;
int grid[MAX_N][MAX_N];
vector<pair<int, int>> base;
pair<int, int> cnv_list[MAX_M];
pair<int, int> guest[MAX_M];
bool visited[MAX_N][MAX_N];
int step[MAX_N][MAX_N];
queue<pair<int, int>> q;

void Init() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[i][j] = false;
            step[i][j] = 0;
        }
    }
}

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

void BFS() {
    int dx[DIR_NUM] = { -1, 0, 0, 1 }, dy[DIR_NUM] = { 0, -1, 1, 0 };

    while (!q.empty()) {
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for (int i = 0; i < DIR_NUM; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (InRange(new_x, new_y) && !visited[new_x][new_y] && grid[new_x][new_y] != 2) {
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
                step[new_x][new_y] = step[x][y] + 1;
            }
        }
    }
}

void Move() {
    int dx[DIR_NUM] = { -1, 0, 0, 1 }, dy[DIR_NUM] = { 0, -1, 1, 0 };

    for (int i = 1; i <= m; i++) {
        if (guest[i] == EMPTY || guest[i] == cnv_list[i]) continue;

        Init();
        visited[cnv_list[i].first][cnv_list[i].second] = true;
        q.push(cnv_list[i]);
        BFS();
        int x, y;
        tie(x, y) = guest[i];
        int minDist = INT_MAX;
        int min_x, min_y;

        for (int i = 0; i < DIR_NUM; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (InRange(new_x, new_y) && visited[new_x][new_y] && step[new_x][new_y] < minDist) {
                minDist = step[new_x][new_y];
                min_x = new_x;
                min_y = new_y;
            }
        }
        guest[i] = make_pair(min_x, min_y);

       /* cout << "guest locate: " << i << "\n";
        cout << min_x << " " << min_y << "\n";*/
    }

    for (int i = 1; i <= m; i++) {
        if (guest[i] == cnv_list[i]) {
            int x, y;
            tie(x, y) = cnv_list[i];
            grid[x][y] = 2;
        }
    }
}

bool NeedUpdate(pair<int, int> close_base, pair<int, int> cur_base) {
    if (close_base == EMPTY) return true;

    int x1, y1;
    tie(x1, y1) = close_base;

    int x2, y2;
    tie(x2, y2) = cur_base;

    return make_tuple(step[x1][y1], x1, y1) > make_tuple(step[x2][y2], x2, y2);
}

void GoBase(int t) {
    Init();
    visited[cnv_list[t].first][cnv_list[t].second] = true;
    q.push(cnv_list[t]);
    BFS();
    /*cout << "who? " << t << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << step[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    pair<int, int> close_base = EMPTY;

    for (int i = 0; i < (int)base.size(); i++) {
        pair<int, int> cur_base = base[i];
        int x, y;
        tie(x, y) = base[i];
        if (NeedUpdate(close_base, cur_base) && visited[x][y]) {
            close_base = cur_base;
        }
    }

    int x, y;
    tie(x, y) = close_base;
    // cout << x << " " << y << t << "\n";
    grid[x][y] = 2;
    guest[t] = make_pair(x, y);
}

void Simulate(int t) {
    Move();
    GoBase(t);
    /*for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
}

bool End() {
    for (int i = 1; i <= m; i++) {
        if (guest[i] != cnv_list[i]) return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 1) base.push_back(make_pair(i, j));
        }
    }

    for (int i = 1; i <= m; i++) {
        int x, y;
        cin >> x >> y;
        cnv_list[i] = make_pair(x - 1, y - 1);
        guest[i] = EMPTY;
    }

    int turn = 1;
    while (1) {
        Simulate(turn);
        if (End()) break;
        turn++;
    }
    cout << turn;
    return 0;
}