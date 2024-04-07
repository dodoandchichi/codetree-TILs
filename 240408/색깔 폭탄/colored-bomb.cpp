#include <iostream>
#include <queue>
#include <tuple>

#define MAX_N 21
#define DIR_NUM 4
#define Bundle tuple<int, int, int, int>
#define EMPTY make_tuple(-1, -1, -1, -1)
// 폭탄 개수, 빨간색 폭탄, 기준행, 기준열
using namespace std;

int n, m;
int grid[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];
int copy_grid[MAX_N][MAX_N];
int ans;
queue<pair<int, int>> q;
bool visited[MAX_N][MAX_N];

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void BFS(int x,  int y, int color){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            visited[i][j] = false;
        }
    }

    int dx[DIR_NUM] = {-1, 0, 1, 0}, dy[DIR_NUM] = {0, 1, 0, -1};

    visited[x][y] = true;
    q.push(make_pair(x, y));

    while(!q.empty()){
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for(int i=0; i<DIR_NUM; i++){
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if(InRange(new_x, new_y) && !visited[new_x][new_y] && 
            (grid[new_x][new_y] == color || grid[new_x][new_y] == 0)){
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
            }
        }
    }
}

Bundle GetBundle(int x, int y){
    BFS(x, y, grid[x][y]);
    
    int bomb_cnt = 0, red_cnt = 0;;
    pair<int, int> standard = make_pair(-1, -1);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(!visited[i][j]) continue;

            bomb_cnt++;

            if(grid[i][j] == 0) red_cnt++;
            else if(make_pair(i, -j) > standard)
                standard = make_pair(i, -j);
        }
    }
    int sx, sy;
    tie(sx, sy) = standard;

    return make_tuple(bomb_cnt, -red_cnt, sx, sy);
}

Bundle FindBundle(){
    Bundle best_bundle = EMPTY;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(grid[i][j] > 0){
                Bundle cur_bundle = GetBundle(i, j);
                if(best_bundle < cur_bundle) best_bundle = cur_bundle;
            }
        }
    }
    return best_bundle;
}

void Gravity(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp[i][j] = -2;
        }
    }

    for(int j=0; j<n; j++){
        int idx = n-1;
        for(int i=n-1; i>=0; i--){
            if(grid[i][j] >= 0){
                temp[idx--][j] = grid[i][j];
            }
            else if(grid[i][j] == -1){
                idx = i;
                temp[idx--][j] = grid[i][j];
            }
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = temp[i][j];
        }
    }
}

void Rotate(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_grid[i][j] = 0;
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_grid[n-j-1][i] = grid[i][j];
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = copy_grid[i][j];
        }
    }
}

void Clean(int x, int y){
    BFS(x, y, grid[x][y]);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(!visited[i][j]) continue;

            grid[i][j] = -2;
        }
    }
/*
    cout << "before clean\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    
    Gravity();
}

bool Simulate(){
    Bundle best_bundle = FindBundle();

    int bomb_cnt, x, y;
    tie(bomb_cnt, ignore, x, y) = best_bundle;

    if(bomb_cnt <= 1 || best_bundle == EMPTY) return false;

    ans += bomb_cnt * bomb_cnt;
    
    Clean(x, -y);
  /*cout << "after clean\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    Rotate();
/*
    cout << "after rotate\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";*/
    Gravity();
/*
    cout << "after gravity\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
*/
    return true;
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
    while(1){
       
        bool keep_going = Simulate();

        if(!keep_going) break;
    }
    cout << ans;
    return 0;
}


/*

    중력으로 떨어지는거랑 회전은 뭐 이제 잘하니깐,,
    가장 큰 폭탄 묶음 구하는건 좀 신기했음
*/