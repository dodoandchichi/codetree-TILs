#include <iostream>
#include <tuple>
#include <algorithm>

#define MAX_N 4
#define DIR_NUM 8
#define BLANK make_pair(-1, -1)
#define TAGGER make_pair(-2, -2)
using namespace std;

int n = 4;
pair<int, int> grid[MAX_N][MAX_N];

int dx[DIR_NUM] = {-1, -1,  0,  1, 1, 1, 0, -1};
int dy[DIR_NUM] = { 0, -1, -1, -1, 0, 1, 1,  1};
int ans;

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Swap(int x, int y, int new_x, int new_y){
    pair<int, int> temp = grid[x][y];
    grid[x][y] = grid[new_x][new_y];
    grid[new_x][new_y] = temp;
}

void Move(int target){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(grid[i][j].first == target){
                int num, dir;
                tie(num, dir) = grid[i][j];

                int new_x = i + dx[dir];
                int new_y = j + dy[dir];
                while(!InRange(new_x, new_y) || grid[new_x][new_y] == TAGGER){
                    dir = (dir + 1) % 8;
                    new_x = i + dx[dir];
                    new_y = j + dy[dir];
                }
                grid[i][j] = make_pair(num, dir);
                Swap(i, j, new_x, new_y);
                return;
            }
        }
    }
}

void MoveAll(){
    for(int i=1; i<=n*n; i++){
        Move(i);
    //     for(int i=0; i<n; i++){
    //     for(int j=0; j<n; j++){
    //         cout << grid[i][j].first << " ";
    //     }
    //     cout << "\n";
    // }
    // cout << "\n";
    // }
    }
}

bool Done(int x, int y, int dir){
    for(int dist=1; dist<4; dist++){
        int new_x = x + dx[dir] * dist;
        int new_y = y + dy[dir] * dist;
        if(InRange(new_x, new_y) && grid[new_x][new_y] != BLANK){
            return false;
        }
    }
    return true;
}

void SearchMax(int x, int y, int dir, int score){
    if(Done(x, y, dir)){
        ans = max(ans, score);
        return;
    }

    for(int dist=1; dist<n; dist++){
        int new_x = x + dx[dir] * dist;
        int new_y = y + dy[dir] * dist;
        if(!InRange(new_x, new_y) || grid[new_x][new_y] == BLANK) continue;

        pair<int, int> temp[MAX_N][MAX_N];
        
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                temp[i][j] = grid[i][j];
            }
        }

        int new_score, new_dir;
        tie(new_score, new_dir) = grid[new_x][new_y];
        grid[new_x][new_y] = TAGGER;
        grid[x][y] = BLANK;

        MoveAll();
        SearchMax(new_x, new_y, new_dir, score + new_score);

        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                grid[i][j] = temp[i][j];
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int p, d;
            cin >> p >> d;
            grid[i][j] = make_pair(p, d-1);
        }
    }

    int init_dir, init_score;
    tie(init_score, init_dir) = grid[0][0];

    grid[0][0] = TAGGER;

    MoveAll();

    SearchMax(0, 0, init_dir, init_score);
    cout << ans;
    return 0;
}