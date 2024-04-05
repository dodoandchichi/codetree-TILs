#include <iostream>
#include <vector>
#include <tuple>

#define MAX_N 4
#define DIR_NUM 8
#define P_DIR_NUM 4
using namespace std;

int n = 4;
int m, t, r, c;
pair<int, int> pack;
vector<int> mon[MAX_N][MAX_N];
vector<int> copy_mon[MAX_N][MAX_N];
int dead[MAX_N][MAX_N];
vector<int> egg[MAX_N][MAX_N];

int dx[DIR_NUM] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[DIR_NUM] = {0, -1, -1, -1, 0, 1, 1, 1};
int p_dx[P_DIR_NUM] = {-1, 0, 1, 0};
int p_dy[P_DIR_NUM] = {0, -1, 0, 1};

void Init(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            egg[i][j].clear();
        }
    }
}

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Breed(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(mon[i][j].size() >= 1){
                for(int k=0; k<(int)mon[i][j].size(); k++){
                    int d = mon[i][j][k];
                    egg[i][j].push_back(d);
                }
            }
        }
    }
}

void Mon_Move(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int k=0; k<(int)mon[i][j].size(); k++){
                bool CanMove = false;
                int dir= mon[i][j][k];

                for(int num=0; num<DIR_NUM; num++){
                    int new_x = i + dx[dir];
                    int new_y = j + dy[dir];

                    if(!InRange(new_x, new_y) || pack == make_pair(new_x, new_y) || dead[new_x][new_y] > 0){
                        dir = (dir + 1) % 8;
                    }
                    else{
                        CanMove = true;
                        copy_mon[new_x][new_y].push_back(dir);
                        break;
                    }
                }
                if(!CanMove) copy_mon[i][j].push_back(dir);
            }
        }
    }
}

void CopyToMon(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            mon[i][j] = copy_mon[i][j];
            copy_mon[i][j].clear();
        }
    }
}

int GetKilledNum(int dir1, int dir2, int dir3) {
    int dirs[] = {dir1, dir2, dir3};
    int x, y;
    tie(x, y) = pack;
    int killed_num = 0;

    vector<pair<int, int>> v_pos;
    bool visit = false;

    for(int i = 0; i < 3; i++) {
        int nx = x + p_dx[dirs[i]], ny = y + p_dy[dirs[i]];
        if(!InRange(nx, ny))
            return -1;

        for(int j=0; j<(int)v_pos.size(); j++){
            if(v_pos[j] == make_pair(nx, ny)) visit = true; 
        }       
        if(!visit){
            killed_num += mon[nx][ny].size();
            v_pos.push_back(make_pair(nx, ny));   
        }
        x = nx; y = ny;
    }
    return killed_num;
}

void DoKill(tuple<int, int, int> best_route) {
    int dir1, dir2, dir3;
    tie(dir1, dir2, dir3) = best_route;
    int dirs[] = {dir1, dir2, dir3};
    int x, y;
    tie(x, y) = pack;

    for(int i = 0; i < 3; i++) {
        int nx = x + p_dx[dirs[i]], ny = y + p_dy[dirs[i]];
        dead[nx][ny] = 3;
        mon[nx][ny].clear();
        x = nx; y = ny;
    }
    pack = make_pair(x, y);
}

void Pack_Move() {
    int max_cnt = -1;
    tuple<int, int, int> best_route;

    for(int i = 0; i < P_DIR_NUM; i++)
        for(int j = 0; j < P_DIR_NUM; j++)
            for(int k = 0; k < P_DIR_NUM; k++) {
                int m_cnt = GetKilledNum(i, j, k);
                if(m_cnt > max_cnt) {
                    max_cnt = m_cnt;
                    best_route = make_tuple(i, j, k);
                }
            }

    DoKill(best_route);
}

void DeadDie(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(dead[i][j] > 0) dead[i][j]--;
        }
    }
}

void Duplicate(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(egg[i][j].size() >= 1){
                for(int k=0; k<(int)egg[i][j].size(); k++){
                    int d = egg[i][j][k];
                    mon[i][j].push_back(d);
                }
            }
        }
    }
}

void Simulate(){
    Init();
    Breed();
    int x, y;
    tie(x, y) = pack;
    Mon_Move();
    CopyToMon();
    Pack_Move();
    DeadDie();
    Duplicate();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> m >> t;
    cin >> r >> c;
    pack = make_pair(r-1, c-1);
    for(int i=0; i<m; i++){
        int x, y, d;
        cin >> x >> y >> d;
        mon[x-1][y-1].push_back(d-1);
    }
    
    while(t--){
        Simulate();
    }
    int ans = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            ans += mon[i][j].size();
        }
    }
    cout << ans;
    return 0;
}