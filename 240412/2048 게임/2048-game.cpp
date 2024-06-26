#include <iostream>
#include <algorithm>

#define MAX_N 21
#define NUM_MOVES 5
#define DIR_NUM 4
using namespace std;

int n;
int grid[MAX_N][MAX_N];
int copy_grid[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];
int move_dir[NUM_MOVES];

int ans;

int GetMaxNum(){
    int maxNum = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            maxNum = max(maxNum, grid[i][j]);
        }
    }
    return maxNum;
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

void Drop(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_grid[i][j] = 0;
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
                copy_grid[temp_idx--][j] = keep_num * 2;
                keep_num = -1;
            }
            else{
                copy_grid[temp_idx--][j] = keep_num;
                keep_num = grid[i][j];
            }
        }
        if(keep_num != -1) copy_grid[temp_idx--][j] = keep_num;
    }
    
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = copy_grid[i][j];
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


/*
    drop에서 keep_num으로 하는 것과 tilt에서 이어지는 rotate,,,
    조금 어렵다. 그리고, 5번 기울이기 전 grid를 저장하는 temp를 rotate나 drop에서 쓰면 초기화가 돼버림
    그래서 copy_grid를 써야함
*/