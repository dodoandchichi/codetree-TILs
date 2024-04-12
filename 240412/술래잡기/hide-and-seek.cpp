#include <iostream>
#include <vector>
#include <tuple>

#define MAX_N 100
#define DIR_NUM 4
using namespace std;

int n, m, h, k;
vector<int> hiders[MAX_N][MAX_N];
vector<int> copy_hiders[MAX_N][MAX_N];
bool tree[MAX_N][MAX_N];
int seeker_next[MAX_N][MAX_N];
int seeker_rev[MAX_N][MAX_N];
int ans;
pair<int, int> seeker;
bool forward_face = true;
int t;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

void SearchSpiral() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };

    int cur_x = n / 2, cur_y = n / 2;
    int move_dir = 0, move_cnt = 1;

    while (cur_x || cur_y) {
        for (int i = 0; i < move_cnt; i++) {
            seeker_next[cur_x][cur_y] = move_dir;
            cur_x += dx[move_dir], cur_y += dy[move_dir];
            seeker_rev[cur_x][cur_y] = (move_dir + 2) % 4;

            if (!cur_y && !cur_x) break;
        }

        move_dir = (move_dir + 1) % 4;
        if (move_dir == 0 || move_dir == 2) move_cnt++;
    }
}

void HiderMove(int x, int y, int dir) {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };

    int new_x = x + dx[dir];
    int new_y = y + dy[dir];
    if (!InRange(new_x, new_y)) {
        dir = (dir + 2) % 4;
    }
    new_x = x + dx[dir];
    new_y = y + dy[dir];

    if (seeker == make_pair(new_x, new_y)) {
        copy_hiders[x][y].push_back(dir);
    }
    else {
        copy_hiders[new_x][new_y].push_back(dir);
    }
}

int Dist(int x, int y) {
    int seeker_x, seeker_y;
    tie(seeker_x, seeker_y) = seeker;

    return abs(seeker_x - x) + abs(seeker_y - y);
}

void HiderMoveAll() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copy_hiders[i][j].clear();
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (hiders[i][j].size() > 0) {
                for (int k = 0; k < (int)hiders[i][j].size(); k++) {
                    if (Dist(i, j) <= 3) 
                        HiderMove(i, j, hiders[i][j][k]);              
                    else 
                        copy_hiders[i][j].push_back(hiders[i][j][k]);
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            hiders[i][j] = copy_hiders[i][j];
        }
    }
}

void SeekerMove() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };

    int x, y;
    tie(x, y) = seeker;

    if (forward_face) {
        int dir = seeker_next[x][y];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];
        seeker = make_pair(new_x, new_y);
        // cout << new_x << " " << new_y << " " << dir << "\n";
        if (new_x == 0 && new_y == 0) forward_face = false;
    }
    else {
        int dir = seeker_rev[x][y];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];
        seeker = make_pair(new_x, new_y);
        if (new_x == n / 2 && new_y == n / 2) forward_face = true;
    }
}

void Catch() {
    int dx[DIR_NUM] = { -1, 0, 1, 0 }, dy[DIR_NUM] = { 0, 1, 0, -1 };

    int x, y, dir;
    tie(x, y) = seeker;
    if (forward_face) {
        dir = seeker_next[x][y];
    }
    else {
        dir = seeker_rev[x][y];
    }
    //cout << "catch dir " << dir << "\n";
    for (int i = 0; i < 3; i++) {
        int new_x = x + dx[dir] * i;
        int new_y = y + dy[dir] * i;
        if (InRange(new_x, new_y) && !tree[new_x][new_y]) {
            ans += t * hiders[new_x][new_y].size();
            hiders[new_x][new_y].clear();
        }
    }
}

void Simulate() {
    HiderMoveAll();
    /*for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << hiders[i][j].size() << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    SeekerMove();
    Catch();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> h >> k;
    for (int i = 0; i < m; i++) {
        int x, y, d;
        cin >> x >> y >> d;
        hiders[x - 1][y - 1].push_back(d);
    }

    while (h--) {
        int x, y;
        cin >> x >> y;
        tree[x - 1][y - 1] = true;
    }
    SearchSpiral();
    seeker = make_pair(n / 2, n / 2);
    while (k--) {
        t++;
        Simulate();
    }
    cout << ans;
    return 0;
}