#include <iostream>
#include <tuple>

#define MAX_N 11
#define DIR_NUM 4
using namespace std;

int n, m, k;
pair<int, int> exits;
pair<int, int> human[MAX_N];
int grid[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];
int dist;
int sx, sy, square_size;

void Move(){
    for(int i=0; i<m; i++){
        if(human[i] == exits) continue;

        int x, y;
        tie(x, y) = human[i];

        int ex, ey;
        tie(ex, ey) = exits;

        if(x < ex && grid[x+1][y] == 0) {
            human[i] = make_pair(x+1, y);
            dist++;
            continue;
        }
        else if(x > ex && grid[x-1][y] == 0){
            human[i] = make_pair(x-1, y);
            dist++;
            continue;
        } 

        if(y < ey && grid[x][y+1] == 0) {
            human[i] = make_pair(x, y+1);
            dist++;
            continue;
        }
        else if(y > ey && grid[x][y-1] == 0){
            human[i] = make_pair(x, y-1);
            dist++;
            continue;
        } 
    }
}

void FindMinSquare(){
    for(int sz=2; sz<=n; sz++){
        for(int x1=0; x1<n-sz+1; x1++){
            for(int y1=0; y1<n-sz+1; y1++){
                int x2 = x1 + sz - 1;
                int y2 = y1 + sz - 1;

                if(x1 <= exits.first && exits.first <= x2 && y1 <= exits.second && exits.second <= y2){
                    for(int i=0; i<m; i++){
                        if(x1 <= human[i].first && human[i].first <= x2 && y1 <= human[i].second && human[i].second <= y2){
                            if(human[i] != exits){
                                sx = x1;
                                sy = y1;
                                square_size = sz;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void RotateSquare(int sx, int sy, int square_size){
    for(int x=sx; x<sx+square_size; x++){
        for(int y=sy; y<sy+square_size; y++){
            if(grid[x][y] > 0) grid[x][y]--;
        }
    }
    // cout << sx << " " << sy << " " << square_size << "\n";

    for(int x=sx; x<sx+square_size; x++){
        for(int y=sy; y<sy+square_size; y++){
            int ox = x-sx, oy = y-sy;
            int rx = oy, ry = square_size - ox - 1;
            temp[rx+sx][ry+sy] = grid[x][y];
        }
    }

    for(int x=sx; x<sx+square_size; x++){
        for(int y=sy; y<sy+square_size; y++){
            grid[x][y] = temp[x][y];
        }
    }
}

void RotateObjects(int sx, int sy, int square_size){
    for(int i=0; i<m; i++){
        int x, y;
        tie(x, y) = human[i];
        if(sx <= x && x < sx + square_size && sy <= y && y < sy + square_size){
            int ox = x-sx, oy = y-sy;
            int rx = oy, ry = square_size - ox - 1;
            human[i] = make_pair(rx+sx, ry+sy);
        }
    }
   
    int ex, ey;
    tie(ex, ey) = exits;

    int ox = ex-sx, oy = ey-sy;
    int rx = oy, ry = square_size - ox - 1;
    exits = make_pair(rx+sx, ry+sy);
}

void Rotate(){
    FindMinSquare();
    RotateSquare(sx, sy, square_size);
    RotateObjects(sx, sy, square_size);
}

bool End(){
    for(int i=0; i<m; i++){
        if(human[i] != exits) return false;
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
        }
    }
    
    for(int i=0; i<m; i++){
        int x, y;
        cin >> x >> y;
        human[i] = make_pair(x-1, y-1);
    }
    int r, c;
    cin >> r >> c;
    exits = make_pair(r-1, c-1);

    while(k--){
        Move();
        if(End()) break;
        Rotate();
    }
    cout << dist << "\n" << exits.first+1 << " " << exits.second+1;
    return 0;
}


/*
    회전하는거 한번 더보고, 움직였을 때 모든 참가자가 출구에있으면 바로 종료해야하는데
    회전을 다시 한번하고 끝내서 틀렸었음
*/