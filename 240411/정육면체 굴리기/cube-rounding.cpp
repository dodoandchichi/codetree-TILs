#include <iostream>
#include <tuple>

#define MAX_N 20
#define MAX_M 20
#define DIR_NUM 4
using namespace std;

int n, m, x, y, k;
int grid[MAX_N][MAX_N];
pair<int, int> dice;

int dx[DIR_NUM] = {0, 0, -1, 1};
int dy[DIR_NUM] = {1, -1, 0, 0};
int dir;

int up = 1, front = 2, ryght= 3;
int dices[7];

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < m;
}

void Roll(int dir){
    int x, y;
    tie(x, y) = dice;

    int new_x = x + dx[dir];
    int new_y = y + dy[dir];
    if(InRange(new_x, new_y)){
        int temp;
        if(dir == 0){
            temp = ryght;
            ryght = up;
            up = 7 - temp;
        }
        else if(dir == 1){
            temp = up;
            up = ryght;
            ryght = 7 - temp;
        }
        else if(dir == 2){
            temp = up;
            up = front;
            front = 7 - temp;
        }
        else if(dir == 3){
            temp = front;
            front = up;
            up = 7 - temp;
        }

        if(grid[new_x][new_y] == 0){
            grid[new_x][new_y] = dices[7-up];
        }
        else{
            dices[7-up] = grid[new_x][new_y];
            grid[new_x][new_y] = 0;
        }
        dice = make_pair(new_x, new_y);
        cout << dices[up] << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> x >> y >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            cin >> grid[i][j];
        }
    }
    dice = make_pair(x, y);
    while(k--){
        cin >> dir;
        Roll(dir - 1);
    }
    return 0;
}


/*
    쉽다시어
*/