#include <iostream>
#include <vector>
#include <tuple>

#define MAX_N 26
#define DIR_NUM 4
using namespace std;

int n, m, d, p;
int grid[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];
vector<pair<int, int>> spirals;

int ans;

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void SearchSpiral(){
    int dx[DIR_NUM] = {0, 1, 0, -1};
    int dy[DIR_NUM] = {-1, 0, 1, 0};

    int cur_x = n/2, cur_y = n/2;
    int move_dir = 0, move_cnt = 1;

    while(cur_x || cur_y){
        for(int i=0; i<move_cnt; i++){
            cur_x += dx[move_dir], cur_y += dy[move_dir];
            spirals.push_back(make_pair(cur_x, cur_y));

            if(!cur_x && !cur_y) break;
        }

        move_dir = (move_dir + 1) % 4;
        if(move_dir == 0 || move_dir == 2) move_cnt++;
    }
}

void Pull(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp[i][j] = 0;
        }
    }

    int temp_idx = 0;
    for(int i=0; i<(int)spirals.size(); i++){
        pair<int, int> grid_point = spirals[i];

        int x, y;
        tie(x, y) = grid_point;

        if(grid[x][y]){
            int tx, ty;
            tie(tx, ty) = spirals[temp_idx];
            temp[tx][ty] = grid[x][y];
            temp_idx++;
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = temp[i][j];
        }
    }
}

void Attack(int dir, int power){
    int dx[DIR_NUM] = {0, 1, 0, -1};
    int dy[DIR_NUM] = {1, 0, -1, 0};

    for(int i=1; i<=power; i++){
        int new_x = n/2 + dx[dir] * i;
        int new_y = n/2 + dy[dir] * i;

        if(InRange(new_x, new_y)){
            ans += grid[new_x][new_y];
            grid[new_x][new_y] = 0;
        }
    }
}

int GetEndOfExplosion(int start_idx, int cur_num){
    int end_idx = start_idx + 1;
    while(end_idx < (int)spirals.size()){
        int x, y;
        tie(x, y) = spirals[end_idx];
        if(grid[x][y] == cur_num)
            end_idx++;
        else break;
    }
    return end_idx - 1;
}

void Remove(int start_idx, int end_idx){
    for(int i=start_idx; i<=end_idx; i++){
        int x, y;
        tie(x, y) = spirals[i];
        ans += grid[x][y];
        grid[x][y] = 0;
    }
}

bool Bomb(){
    bool did_explode = false;

    int cur_idx = 0;
    while(cur_idx < (int) spirals.size()){
        int x, y;
        tie(x, y) = spirals[cur_idx];

        int end_idx = GetEndOfExplosion(cur_idx, grid[x][y]);

        if(grid[x][y] == 0) break;

        if(end_idx - cur_idx + 1 >= 4){
            Remove(cur_idx, end_idx);
            did_explode = true;
        }
        cur_idx = end_idx + 1;
    }
    
    return did_explode;
}

void PutPair(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp[i][j] = 0;
        }
    }

    int temp_idx = 0, cur_idx = 0;
    int end_of_array = (int) spirals.size();

    while(cur_idx < end_of_array){
        int x, y;
        tie(x, y) = spirals[cur_idx];
        int end_idx = GetEndOfExplosion(cur_idx, grid[x][y]);

        if(grid[x][y] == 0) break;

        int continuous_cnt = end_idx - cur_idx + 1;
        if(temp_idx >= end_of_array) break;

        int tx, ty;
        tie(tx, ty) = spirals[temp_idx++];
        temp[tx][ty] = continuous_cnt;

        if(temp_idx >= end_of_array) break;

        tie(tx, ty) = spirals[temp_idx++];
        temp[tx][ty] = grid[x][y];
        
        cur_idx = end_idx + 1;
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = temp[i][j];
        }
    }
}

void Simulate(){
    Attack(d, p);
    Pull();

    while(1){
        bool did_explode = Bomb();
        if(!did_explode) break;
        Pull();
    }

    PutPair();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin >> grid[i][j];
        }
    }

    SearchSpiral();

    while(m--){
        cin >> d >> p;
        Simulate();
    }
    cout << ans;
    return 0;
}