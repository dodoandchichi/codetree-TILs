#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#define MAX_N 51
#define DIR_NUM 4
using namespace std;

int n, m;
int grid[MAX_N][MAX_N];
vector<pair<int, int>> hos;
vector<pair<int, int>> selected_hos;
queue<pair<int, int>> q;
bool visited[MAX_N][MAX_N];
int step[MAX_N][MAX_N];
int ans = INT_MAX;
bool virus;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

void BFS() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 };
    int dy[DIR_NUM] = { 0, 1, 0, -1 };

    while (!q.empty()) {
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for (int i = 0; i < DIR_NUM; i++) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if (InRange(new_x, new_y) && !visited[new_x][new_y] && grid[new_x][new_y] != 1) {
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
                step[new_x][new_y] = min(step[new_x][new_y], step[x][y] + 1);
            }
        }
    }
}

void Init_visit() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[i][j] = false;
        }
    }
}

void Init_step() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            step[i][j] = 10001;
        }
    }
}

void print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << step[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int Calc() {
    Init_step();
    int minTime = INT_MIN;
    for (int i = 0; i < selected_hos.size(); i++) {
        Init_visit();
        q.push(selected_hos[i]);
        visited[selected_hos[i].first][selected_hos[i].second] = true;
        step[selected_hos[i].first][selected_hos[i].second] = 0;
        BFS();
        //print();
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 0 && step[i][j] == 10001) return INT_MAX;
            else if (grid[i][j] == 0) {
                minTime = max(step[i][j], minTime);
            }
        }
    }
    return minTime;
}

void FindMin(int idx, int cnt) {
    if (selected_hos.size() == m) {
        ans = min(ans, Calc());
        return;
    }

    if (idx == hos.size()) return;

    selected_hos.push_back(hos[idx]);
    FindMin(idx + 1, cnt + 1);
    selected_hos.pop_back();

    FindMin(idx + 1, cnt);
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
            if (grid[i][j] == 2) {
                hos.push_back(make_pair(i, j));
            }
            else if (grid[i][j] == 0) virus = true;
        }
    }
    FindMin(0, 0);
    if (!virus) cout << "0";
    else if (ans == INT_MAX) cout << "-1";
    else cout << ans;
    return 0;
}