#include <iostream>
#include <queue>
#include <algorithm>
#include <tuple>
#include <climits>

#define MAX_N 11
#define DIR_NUM 4
using namespace std;

int n, m, k;
pair<int, int> tower[MAX_N][MAX_N];
pair<int, int> attacker;
pair<int, int> subattacker;
queue<pair<int, int>> q;
bool visited[MAX_N][MAX_N];
bool relate[MAX_N][MAX_N];
int back_x[MAX_N][MAX_N];
int back_y[MAX_N][MAX_N];
bool CanLaser;
int turn = 1;

void Init(){
    CanLaser = false;
    while(!q.empty()) q.pop();
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            visited[i][j] = false;
            relate[i][j] = false;
        }
    }
}

bool NeedUpdate(tuple<int, int, int, int> best_tower, tuple<int, int, int, int> new_tower){
    if(best_tower == make_tuple(INT_MAX, INT_MAX, 0, 0)) return true;

    int best_str, best_exp, best_row, best_col;
    tie(best_str, best_exp, best_row, best_col) = best_tower;

    int new_str, new_exp, new_row, new_col;
    tie(new_str, new_exp, new_row, new_col) = new_tower;

    return make_tuple(-best_str, best_exp, best_row+best_col, best_col) <
           make_tuple(-new_str, new_exp, new_row+new_col, new_col);
}

void Choose_Attack(){
    tuple<int, int, int, int> best_tower = make_tuple(INT_MAX, INT_MAX, 0, 0);

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            int str, exp;
            tie(str, exp) = tower[i][j];
            if(str == 0) continue;
        
            tuple<int, int, int, int> new_tower = make_tuple(str, exp, i, j);
            if(NeedUpdate(best_tower, new_tower)) {
                best_tower = new_tower;
            }
        }
    }

    int x, y, str, exp;
    tie(ignore, ignore, x, y) = best_tower;
    tie(str, exp) = tower[x][y];
    tower[x][y] = make_pair(str+n+m, turn);
    attacker = make_pair(x, y);
    relate[x][y] = true;
}

bool NeedUpdate2(tuple<int, int, int, int> best_tower, tuple<int, int, int, int> new_tower){
    if(best_tower == make_tuple(INT_MIN, INT_MIN, 0, 0)) return true;

    int best_str, best_exp, best_row, best_col;
    tie(best_str, best_exp, best_row, best_col) = best_tower;

    int new_str, new_exp, new_row, new_col;
    tie(new_str, new_exp, new_row, new_col) = new_tower;

    return make_tuple(best_str, -best_exp, -(best_row+best_col), -best_col) <
           make_tuple(new_str, -new_exp, -(new_row+new_col), -new_col);
}

void Attack(){
    tuple<int, int, int, int> best_tower = make_tuple(INT_MIN, INT_MIN, 0, 0);

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            int str, exp;
            tie(str, exp) = tower[i][j];
            if(str == 0 || (attacker == make_pair(i, j))) continue;
            tuple<int, int, int, int> new_tower = make_tuple(str, exp, i, j);
            if(NeedUpdate2(best_tower, new_tower)) best_tower = new_tower;
        }
    }

    int x, y, str, exp;
    tie(ignore, ignore, x, y) = best_tower;
    subattacker = make_pair(x, y);
    relate[x][y] = true;
}

void BFS(){
    int dx[DIR_NUM] = {0, 1, 0, -1}, dy[DIR_NUM] = {1, 0, -1, 0};

    while(!q.empty()){
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();
        if(subattacker == make_pair(x, y)) {
            CanLaser = true;
            break;
        }
        for(int i=0; i<DIR_NUM; i++){
            int new_x = (x + dx[i] + n) % n;
            int new_y = (y + dy[i] + m) % m;

            if(tower[new_x][new_y].first != 0 && !visited[new_x][new_y]){
                back_x[new_x][new_y] = x;
                back_y[new_x][new_y] = y;
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
            }
        }
    }
}

void LaserAttack(){
    int sx, sy, ex, ey;
    tie(ex, ey) = subattacker;
    tie(sx, sy) = attacker;

    int damage, str, exp;
    tie(damage, ignore) = tower[sx][sy];
    tie(str, exp) = tower[ex][ey];

    tower[ex][ey] = make_pair(max(0,str-damage), exp);

    int new_x = ex, new_y = ey;

    while(1){
        int x = back_x[new_x][new_y];
        int y = back_y[new_x][new_y];
        if(x == sx && y == sy) break;
        tie(str, exp) = tower[x][y];
        tower[x][y] = make_pair(max(0,str-damage/2), exp);
        relate[x][y] = true;
        new_x = x, new_y = y;
    }
}

void PortAttack(){
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, 1, -1, -1, 0, 1};

    int sx, sy, ex, ey;
    tie(ex, ey) = subattacker;
    tie(sx, sy) = attacker;

    int damage, str, exp;
    tie(damage, ignore) = tower[sx][sy];
    tie(str, exp) = tower[ex][ey];

    tower[ex][ey] = make_pair(max(0,str-damage), exp);

    for(int i=0; i<8; i++){
        int x = (ex + dx[i] + n) % n;
        int y = (ey + dy[i] + m) % m;

        if(tower[x][y].first > 0 && attacker != make_pair(x, y)){
            int str, exp;
            tie(str, exp) = tower[x][y];
            tower[x][y] = make_pair(max(0, str-damage/2), exp);
            relate[x][y] = true;
        }
    }
}

void Heal(){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(tower[i][j].first > 0 && !relate[i][j]){
                int str, exp;
                tie(str, exp) = tower[i][j];
                tower[i][j] = make_pair(str+1, exp);
            }
        }
    }
}

void Simulate(){
    Init();
    Choose_Attack();
    Attack();
    int x, y, ex, ey;
    tie(ex, ey) = subattacker;
    tie(x, y) = attacker;
    q.push(make_pair(x, y));
    visited[x][y] = true;
    BFS();
    
    if(CanLaser) LaserAttack();
    else PortAttack();
    
    Heal();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            int num;
            cin >> num;
            if(num) tower[i][j] = make_pair(num, 0);
        }
    }

    for(int i=1; i<=k; i++){
        Simulate();
        turn++;
        int cnt = 0;
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                if(tower[i][j].first){
                    cnt++;
                }
            }
        }
        if(cnt == 1) break;
    }

    int max_str = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(tower[i][j].first){
                max_str = max(max_str, tower[i][j].first);
            }
        }
    }
    cout << max_str;
    return 0;
}



/*
    실수 개많이하고 조심해야하는 문제..

    1. 포탑이 하나만 남아있을 때 종료해야하는데 못보고 지나침
    2. 공격 대상자 좌표에 도착하면 BFS가 종료되는데 매 턴마다 q를 초기화하지않음
    3. k-1로 최근 공격한 턴을 기록하려했는데 k는 바뀌지않음.. turn을 따로만들어줌
    4. back_x, back_y로 돌아와는 좌표를 기록해줌
    5. 포탄 공격일 때 자기 자신 피해안입도록 설정안함..
*/