#include <iostream>
#include <tuple>
#include <queue>
#include <algorithm>
#include <climits>

#define MAX_N 11
#define MAX_M 11
#define DIR_NUM 4
#define P_DIR_NUM 8
#define EMPTY make_tuple(-1, -1, -1, -1)
#define WEAK_EMPTY make_tuple(INT_MIN, -1, -1, -1)
using namespace std;

int n, m, k;
pair<int, int> tower[MAX_N][MAX_M];
pair<int, int> attaker;
pair<int, int> subattacker;
bool visited[MAX_N][MAX_M];
bool related[MAX_N][MAX_M];
int back_x[MAX_N][MAX_M];
int back_y[MAX_N][MAX_M];
queue<pair<int, int>> q;
bool can_laser;
int t;

void Init(){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            visited[i][j] = false;
            related[i][j] = false;
        }
    }
    can_laser = false;

    while(!q.empty()){
        q.pop();
    }
}

void ChooseAttaker(){
    tuple<int, int, int, int> weak_tower = WEAK_EMPTY;
    int weak_x, weak_y;

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(tower[i][j].first != 0){
                int power, past;
                tie(power, past) = tower[i][j];
                tuple<int, int, int, int> cur_tower = make_tuple(-power, past, (i+j), j);
            
                if(weak_tower < cur_tower){
                    weak_tower = cur_tower;
                    weak_x = i;
                    weak_y = j;
                }
            }
        }
    }
    int power;
    tie(power, ignore, ignore, ignore) = weak_tower;
    tower[weak_x][weak_y] = make_pair(-power + n + m, t);
    attaker = make_pair(weak_x, weak_y);
    related[weak_x][weak_y] = true;
}

void ChooseSubAttaker(){
    tuple<int, int, int, int> strong_tower = EMPTY;
    int strong_x, strong_y;

    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(tower[i][j].first != 0 && attaker != make_pair(i, j)){
                int power, past;
                tie(power, past) = tower[i][j];
                tuple<int, int, int, int> cur_tower = make_tuple(power, -past, -(i+j), -j);
            
                if(strong_tower < cur_tower){
                    strong_tower = cur_tower;
                    strong_x = i;
                    strong_y = j;
                }
            }
        }
    }
    subattacker = make_pair(strong_x, strong_y);
    related[strong_x][strong_y] = true;
}

void BFS(){
    int dx[DIR_NUM] = {0, 1, 0, -1}, dy[DIR_NUM] = {1, 0, -1, 0};

    while(!q.empty()){
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for(int i=0; i<DIR_NUM; i++){
            int new_x = (x + dx[i] + n) % n;
            int new_y = (y + dy[i] + m) % m;
            
            if(!visited[new_x][new_y] && tower[new_x][new_y].first != 0){
                back_x[new_x][new_y] = x;
                back_y[new_x][new_y] = y;
                visited[new_x][new_y] = true;
                if(subattacker == make_pair(new_x, new_y)) {
                    can_laser = true;
                    return;
                }
                q.push(make_pair(new_x, new_y));
            }
        }
    }
}

void FindLaser(){
    q.push(attaker);
    visited[attaker.first][attaker.second] = true;
    BFS();
}

void LaserAttack(){
    int first_x, first_y;
    tie(first_x, first_y) = attaker;

    int power;
    tie(power, ignore) = tower[first_x][first_y];

    int last_x, last_y;
    tie(last_x, last_y) = subattacker;
    int sub_power, past_attack;
    tie(sub_power, past_attack) = tower[last_x][last_y];

    tower[last_x][last_y] = make_pair(max(sub_power - power, 0), past_attack);

    while(!(first_x == last_x && first_y == last_y)){
        int new_x = back_x[last_x][last_y];
        int new_y = back_y[last_x][last_y];
        
        if(first_x == new_x && first_y == new_y) break;

        int p, past;
        tie(p, past) = tower[new_x][new_y];
        tower[new_x][new_y] = make_pair(max(p - power/2, 0), past);
        related[new_x][new_y] = true;
        last_x = new_x, last_y = new_y;
    }
}

void PortAttack(){
    int dx[P_DIR_NUM] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[P_DIR_NUM] = {-1, 0, 1, 1, -1, -1, 0, 1};

    int sx, sy;
    tie(sx, sy) = attaker;
    
    int power;
    tie(power, ignore) = tower[sx][sy];

    int ex, ey;
    tie(ex, ey) = subattacker;

    int p, past;
    tie(p, past) = tower[ex][ey];

    tower[ex][ey] = make_pair(max(p - power, 0), past);

    for(int i=0; i<P_DIR_NUM; i++){
        int new_x = (ex + dx[i] + n) % n;
        int new_y = (ey + dy[i] + m) % m;
        if(tower[new_x][new_y].first != 0 && tower[new_x][new_y] != attaker){
            int p, past;
            tie(p, past) = tower[new_x][new_y];

            tower[new_x][new_y] = make_pair(max(p - power/2, 0), past);
            related[new_x][new_y] = true;
        }
    }
}

void HealTower(){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(!related[i][j] && tower[i][j].first != 0){
                int p, past;
                tie(p, past) = tower[i][j];

                tower[i][j] = make_pair(p + 1, past);
            }
        }
    }
}

void Simulate(){
    Init();
    // cout << "Befor attack\n";
    // for(int i=0; i<n; i++){
    //     for(int j=0; j<m; j++){
    //         cout << tower[i][j].first << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";
    ChooseAttaker();
    ChooseSubAttaker();
    // cout << "choose attacker\n";
    // for(int i=0; i<n; i++){
    //     for(int j=0; j<m; j++){
    //         cout << tower[i][j].first << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";
    FindLaser();
    if(can_laser) LaserAttack();
    else PortAttack();

    // cout << "After attack\n";
    // for(int i=0; i<n; i++){
    //     for(int j=0; j<m; j++){
    //         cout << tower[i][j].first << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";

    HealTower();

    // cout << "After Heal\n";
    // for(int i=0; i<n; i++){
    //     for(int j=0; j<m; j++){
    //         cout << tower[i][j].first << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";
}

bool End(){
    int cnt = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(tower[i][j].first != 0){
                cnt++;
            }
        }
    }

    if(cnt == 1) return true;
    
    return false;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            int num;
            cin >> num;
            tower[i][j] = make_pair(num, 0);
        }
    }

    while(k--){
        t++;
        Simulate();
        if(End()) break;
    }

    int ans = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            if(tower[i][j].first != 0){
                ans = max(ans, tower[i][j].first);
            }
        }
    }
    cout << ans;
    return 0;
}