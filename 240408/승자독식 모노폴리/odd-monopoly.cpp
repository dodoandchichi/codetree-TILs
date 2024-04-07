#include <iostream>
#include <tuple>

#define MAX_N 20
#define MAX_M 400
#define DIR_NUM 4
#define EMPTY make_pair(401, 401)
using namespace std;

int n, m, k;
int grid[MAX_N][MAX_N];
int next_dir[MAX_M+1][DIR_NUM][DIR_NUM];
pair<int, int> player[MAX_N][MAX_N];
pair<int, int> copy_player[MAX_N][MAX_N];
pair<int, int> contract[MAX_N][MAX_N];
int turn = 1;

int dx[DIR_NUM] = {-1, 1, 0, 0};
int dy[DIR_NUM] = {0, 0, -1, 1};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y  && y < n;
}

void Update(int x, int y, pair<int, int> new_player) {
    if(copy_player[x][y] > new_player) 
        copy_player[x][y] = new_player;
}

void Move(int x, int y){
    int num, cur_dir;
    tie(num, cur_dir) = player[x][y];

    for(int i=0; i<DIR_NUM; i++){
        int dir = next_dir[num][cur_dir][i];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];
        if(InRange(new_x, new_y) && contract[new_x][new_y] == EMPTY){
            Update(new_x, new_y, make_pair(num, dir));
            return;
        }
    }

    for(int i=0; i<DIR_NUM; i++){
        int dir = next_dir[num][cur_dir][i];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];
        if(InRange(new_x, new_y) && contract[new_x][new_y].first == num){
            Update(new_x, new_y, make_pair(num, dir));
            return;
        }
    }
    
}

void Deccontract(int x, int y){
    int num, time;
    tie(num, time) = contract[x][y];

    if(contract[x][y].second == 1) contract[x][y] = EMPTY;
    else contract[x][y] = make_pair(num, time-1);
}

void Addcontract(int x, int y){
    int num;
    tie(num, ignore) = player[x][y];

    contract[x][y] = make_pair(num, k);
}

void Simulate(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_player[i][j] = EMPTY;
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(player[i][j] != EMPTY) Move(i, j);
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            player[i][j] = copy_player[i][j];
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(contract[i][j] != EMPTY)
                Deccontract(i, j);
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(player[i][j] != EMPTY)
                Addcontract(i, j);
        }
    }
}

bool End(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(player[i][j] == EMPTY) continue;

            int num;
            tie(num, ignore) = player[i][j];

            if(num != 1) return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin >> grid[i][j];
            if(grid[i][j] == 0){
                player[i][j] = EMPTY;
                contract[i][j] = EMPTY;
            }
        }
    }

    for(int num=1; num<=m; num++){
        int dir;
        cin >> dir;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(grid[i][j] == num){
                    player[i][j] = make_pair(num, dir-1);
                    contract[i][j] = make_pair(num, k);
                }
            }
        }
    }

    for(int i=1; i<=m; i++){
        for(int dir=0; dir<DIR_NUM; dir++){
            for(int j=0; j<4; j++){
                cin >> next_dir[i][dir][j];
                next_dir[i][dir][j]--;
            }
        }
    }

    while(1){
        Simulate();
        if(turn >= 1000 || End()) break;
        turn++;
    }
    if(turn >= 1000) cout << "-1";
    else cout << turn;
    return 0;
}