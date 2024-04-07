#include <iostream>
#include <queue>
#include <algorithm>

#define MAX_N 100
#define DIR_NUM 4
using namespace std;

int n, t, level, grid_size;
int grid[MAX_N][MAX_N];
int copy_grid[MAX_N][MAX_N];
queue<pair<int, int>> q;
bool visited[MAX_N][MAX_N];

int dx[DIR_NUM] = {0, 1, -1, 0};
int dy[DIR_NUM] = {1, 0, 0, -1};
int total_ice, best_size, cnt;

bool InRange(int x, int y){
    return 0 <= x && x < grid_size && 0 <= y && y < grid_size;
}

void Move(int x, int y, int half_size, int dir){
    for(int i=x; i<x+half_size; i++){
        for(int j=y; j<y+half_size; j++){
            int new_x = i + dx[dir] * half_size;
            int new_y = j + dy[dir] * half_size;

            copy_grid[new_x][new_y] = grid[i][j];
        }
    }
}

void Rotate(){
    int box_size = (1 << level);
    int half_size = box_size / 2;

    if(level == 0) return;
    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){
            copy_grid[i][j] = 0;
        }
    }

    for(int i=0; i<grid_size; i+=box_size){
        for(int j=0; j<grid_size; j+=box_size){
            Move(i, j, half_size, 0);
            Move(i, j+half_size, half_size, 1);
            Move(i+half_size, j, half_size, 2);
            Move(i+half_size, j+half_size, half_size, 3);
        }
    }    

    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){
            grid[i][j] = copy_grid[i][j];
        }
    }
}

void Melt(){
    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){
            copy_grid[i][j] = 0;
        }
    }

    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){
            if(grid[i][j] > 0){
                int cnt = 0;
                for(int dir=0; dir<DIR_NUM; dir++){
                    int new_x = i + dx[dir];
                    int new_y = j + dy[dir];
                    if(InRange(new_x, new_y) && grid[new_x][new_y]) cnt++;
                }
                if(cnt >= 3){
                    copy_grid[i][j] = grid[i][j];
                }
                else copy_grid[i][j] = grid[i][j] - 1;
            }
        }
    }

    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){
            grid[i][j] = copy_grid[i][j];
        }
    }
}

void Simulate(){
    Rotate(); 
    Melt();
}

void BFS(){
    while(!q.empty()){
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for(int i=0; i<DIR_NUM; i++){
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if(InRange(new_x, new_y) && !visited[new_x][new_y] && grid[new_x][new_y]){
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
                cnt++;
            }
        }
    }
}

void Count(){
    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){
            if(grid[i][j]){
                total_ice += grid[i][j];

                if(visited[i][j]) continue;

                q.push(make_pair(i, j));
                visited[i][j] = true;
                cnt = 1;
                BFS();

                best_size = max(best_size, cnt);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    
    cin >> n >> t;
    grid_size = (1 << n);
    
    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){
            cin >> grid[i][j];
        }
    }

    while(t--){
        cin >> level;
        Simulate();
    }
    Count();
    cout << total_ice << "\n" << best_size;
    return 0;
}


/*
    좀 독특한 회전, << 로 grid_size를 만들었는데 자꾸 for문에서 n을써서 이상하게 됨 ㅎㅎ..
    level이 0일 때랑 grid[i][j]가 0일때 제외를 안하면 얼음이 하나도 없어도 126행의 cnt가 항상 1이되는 것 조심
*/