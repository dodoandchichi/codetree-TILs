#include <iostream>
#include <queue>

#define MAX_N 30
#define DIR_NUM 4
using namespace std;

int n, ans;
int grid[MAX_N][MAX_N];
int copy_grid[MAX_N][MAX_N];
int group[MAX_N][MAX_N];
int group_cnt[MAX_N];
int group_n;
queue<pair<int, int>> q;
bool visited[MAX_N][MAX_N];

int dx[DIR_NUM] = {-1, 0, 1, 0};
int dy[DIR_NUM] = {0, 1, 0, -1};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
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
            if(InRange(new_x, new_y) && grid[new_x][new_y] == grid[x][y] && !visited[new_x][new_y]){
                visited[new_x][new_y] = true;
                group[new_x][new_y] = group_n;
                q.push(make_pair(new_x, new_y));
                group_cnt[group_n]++;
            }
        }
    }
}

void MakeGroup(){
    group_n = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            visited[i][j] = false;
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
           if(visited[i][j]) continue;

           group_n++;
           group[i][j] = group_n; 
           visited[i][j] = true;
           group_cnt[group_n] = 1;
           q.push(make_pair(i, j));
           BFS();
        }
    }
}

int GetScore(){
    int score = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            for(int dir=0; dir<DIR_NUM; dir++){
                int new_x = i + dx[dir];
                int new_y = j + dy[dir];
                if(InRange(new_x, new_y) && grid[i][j] != grid[new_x][new_y]){
                    int g1 = group[i][j], g2 = group[new_x][new_y];
                    int cnt1 = group_cnt[g1], cnt2 = group_cnt[g2];
                    //cout << g1 << " " << g2 << " " << cnt1 << " " << cnt2 << "\n"; 
                    score += (cnt1 + cnt2) * grid[i][j] * grid[new_x][new_y];
                }
            }
        }
    }

    return score / 2;
}

void RotateSquare(int sx, int sy, int square_size){
    for(int x = sx; x<sx+square_size; x++){
        for(int y = sy; y < sy+square_size; y++){
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = square_size - ox - 1;
            copy_grid[rx+sx][ry+sy] = grid[x][y];
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
            if(i == n/2){
                copy_grid[n-j-1][i] = grid[i][j];
            }

            else if(j == n/2){
                copy_grid[j][i] = grid[i][j];
            }
        }
    }
    int square_size = n/2;
    RotateSquare(0, 0, square_size);
    RotateSquare(0, square_size+1, square_size);
    RotateSquare(square_size+1, 0, square_size);
    RotateSquare(square_size+1, square_size+1, square_size);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = copy_grid[i][j];
        }
    }
}

void Simulate(){
    MakeGroup();
    ans += GetScore();
    Rotate();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin >> grid[i][j];
        }
    }

    for(int i=0; i<4; i++){
        Simulate();
    }
    cout << ans;
    return 0;
}