#include <iostream>
#include <algorithm>

#define MAX_N 21
#define NUM_MOVES 5
#define DIR_NUM 4
using namespace std;

int n;
int grid[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];
int move_dir[NUM_MOVES];

int ans;

int GetMaxNum(){
    int maxNum = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            maxNum = max(maxNum, temp[i][j]);
        }
    }
    return maxNum;
}

void Rotate(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp[i][j] = 0;
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp[i][j] = grid[n-j-1][i];
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = temp[i][j];
        }
    }
}

void Drop(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp[i][j] = 0;
        }
    }

    for(int j=0; j<n; j++){
        int temp_idx = n-1, keep_num = -1;
        for(int i=n-1; i>=0; i--){
            if(grid[i][j] == 0) continue;

            if(keep_num == -1){
                keep_num = grid[i][j];
            }
            else if(keep_num == grid[i][j]){
                temp[temp_idx--][j] = keep_num * 2;
                keep_num = -1;
            }
            else{
                temp[temp_idx--][j] = keep_num;
                keep_num = grid[i][j];
            }
        }
        if(keep_num != -1) temp[temp_idx--][j] = keep_num;
    }
    
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = temp[i][j];
        }
    }
}

void Tilt(int dir){
    for(int i=0; i<dir; i++)
        Rotate();

    Drop();

    for(int i=0; i<4-dir; i++)
        Rotate();
}

void Simulate(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            temp[i][j] = grid[i][j];
        }
    }

    for(int i=0; i<NUM_MOVES; i++){
        Tilt(move_dir[i]);
    }

    ans = max(ans, GetMaxNum());

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = temp[i][j];
        }
    }
}

void SearchMax(int cnt){
    if(cnt == NUM_MOVES){
        Simulate();
        return;
    }

    for(int i=0; i<4; i++){
        move_dir[cnt] = i;
        SearchMax(cnt + 1);
    }
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
    SearchMax(0);
    cout << ans;
    return 0;
}