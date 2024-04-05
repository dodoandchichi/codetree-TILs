#include <iostream>
#include <vector>
#include <tuple>

#define MAX_T 26
#define MAX_N 4
#define MAX_DECAY 2
#define DIR_NUM 8
#define P_DIR_NUM 4 
using namespace std;

int n = 4;
int mon[MAX_T][MAX_N][MAX_N][DIR_NUM];
int dead[MAX_N][MAX_N][MAX_DECAY+1];
int m, t, r, c;
int px, py;
int turn = 1;

int dx[DIR_NUM] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[DIR_NUM] = {0, -1, -1, -1, 0, 1, 1, 1};
int p_dx[P_DIR_NUM] = {-1, 0, 1, 0};
int p_dy[P_DIR_NUM] = {0, -1, 0, 1};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

bool CanGo(int x, int y){
    return InRange(x, y) && dead[x][y][0] == 0 && dead[x][y][1] == 0
    && !(px == x && py == y); 
}

tuple<int, int, int> NextPos(int x, int y, int dir){
    for(int i=0; i<DIR_NUM; i++){
        int n_dir = (dir + i) % DIR_NUM;
        int new_x = x + dx[n_dir];
        int new_y = y + dy[n_dir];
        if(CanGo(new_x, new_y)){
            return make_tuple(new_x, new_y, n_dir);
        }
    }
    return make_tuple(x, y, dir);
}

void Mon_Move(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<DIR_NUM; k++){
                tuple<int, int, int> next_pos = NextPos(i, j, k);
                int x, y, dir;
                tie(x, y, dir) = next_pos;
                mon[turn][x][y][dir] += mon[turn-1][i][j][k];
            }
        }
    }
}

int getKill(int dir1, int dir2, int dir3){
    int dir[3] = {dir1, dir2, dir3};
    int x = px, y = py;
    vector<pair<int, int>> v_pos;
    bool again = false;
    int cnt = 0;

    for(int i=0; i<3; i++){
        int new_x = x + p_dx[dir[i]];
        int new_y = y + p_dy[dir[i]];

        if(!InRange(new_x, new_y)) return -1;

        for(int j=0; j<(int)v_pos.size(); j++){
            if(v_pos[j] == make_pair(new_x, new_y)) again = true;
        }

        if(!again){
            for(int k=0; k<DIR_NUM; k++){
                cnt += mon[turn][new_x][new_y][k];
            }
            v_pos.push_back(make_pair(new_x, new_y));
        }
        x = new_x, y = new_y;
    }
    return cnt;
}

void EatMon(tuple<int, int, int> best_route){
    int dir1, dir2, dir3;
    tie(dir1, dir2, dir3) = best_route;

    int dir[3] = {dir1, dir2, dir3};

    for(int i=0; i<3; i++){
        int new_x = px + p_dx[dir[i]];
        int new_y = py + p_dy[dir[i]];

        for(int j=0; j<DIR_NUM; j++){
            dead[new_x][new_y][MAX_DECAY] += mon[turn][new_x][new_y][j];
            mon[turn][new_x][new_y][j] = 0;
        }
        px = new_x, py = new_y;
    }
}

void Pack_Move(){
    int max_cnt = -1;
    tuple<int, int, int> best_route;

    for(int i=0; i<P_DIR_NUM; i++){
        for(int j=0; j<P_DIR_NUM; j++){
            for(int k=0; k<P_DIR_NUM; k++){
                int kill_cnt = getKill(i, j, k);

                if(kill_cnt > max_cnt){
                    max_cnt = kill_cnt;
                    best_route = make_tuple(i, j, k);
                }
            }
        }
    }

    EatMon(best_route);
}

void Mon_Decay(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<MAX_DECAY; k++){
                dead[i][j][k] = dead[i][j][k+1];
            }
            dead[i][j][MAX_DECAY] = 0;
        }
    }
}

void Mon_Add(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<DIR_NUM; k++){
                mon[turn][i][j][k] += mon[turn-1][i][j][k];
            }
        }
    }
}

void Simulate(){
    Mon_Move();
    Pack_Move();
    Mon_Decay();
    Mon_Add();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> m >> t;
    cin >> r >> c;
    px = r-1, py = c-1;

    for(int i=0; i<m; i++){
        int x, y, d;
        cin >> x >> y >> d;
        mon[0][x-1][y-1][d-1] += 1;
    } 

    int cnt = 0;
    while(turn <= t){
        Simulate();
        turn++;
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<DIR_NUM; k++){
                cnt += mon[t][i][j][k];
            }
        }
    }
    cout << cnt;
    return 0;
}