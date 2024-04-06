#include <iostream>
#include <tuple>

#define MAX_N 10
using namespace std;

int n, m, k;
int board[MAX_N][MAX_N];
int next_board[MAX_N][MAX_N];
pair<int, int> human[MAX_N+1];
pair<int, int> exits;

int ans;
int sx, sy, square_size;

void Move(){
    int ex, ey;
    tie(ex, ey) = exits;
    for(int i=0; i<m; i++){
        if(human[i].first == ex && human[i].second == ey) continue;

        int x, y;
        tie(x, y) = human[i];

        int move_x = 0;
        if(x < ex) move_x++;
        else if(x > ex) move_x--;

        if(move_x != 0){
            if(board[x+move_x][y] == 0){
                human[i] = make_pair(x+move_x, y);
                ans++;
                continue;
            }
        }
        
        int move_y = 0;
        if(y < ey) move_y++;
        else if(y > ey) move_y--;

        if(move_y != 0){
            if(board[x][y+move_y] == 0){
                human[i] = make_pair(x, y+move_y);
                ans++;
                continue;
            }
        }
    }
}

void MinSquare(){
    for(int sz=2; sz<=n; sz++){
        for(int x1=0; x1<n-sz+1; x1++){
            for(int y1=0; y1<n-sz+1; y1++){
                int x2 = x1 + sz - 1;
                int y2 = y1 + sz - 1;

                if(!(x1 <= exits.first && exits.first <= x2 && y1 <= exits.second && exits.second <= y2)) {
                    continue;
                } 
                
                bool in_human = false;
                for(int i=0; i<m; i++){
                    int x, y;
                    tie(x, y) = human[i];
                    if(x1 <= x && x <= x2 && y1 <= y && y <= y2) {
                        if(!(x == exits.first && y == exits.second)){
                            in_human = true;
                        }
                    }     
                }

                if(in_human){
                    sx = x1;
                    sy = y1;
                    square_size = sz;

                    return;
                }
            }
        }
    }
}

void Rotate(){
    for(int x = sx; x < sx + square_size; x++)
        for(int y = sy; y < sy + square_size; y++) {
            if(board[x][y]) board[x][y]--;
        }

    for(int x=sx; x<sx+square_size; x++){
        for(int y=sy; y<sy+square_size; y++){
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = square_size - ox - 1;
            next_board[rx + sx][ry + sy] = board[x][y];
        }
    }

    for(int x = sx; x < sx + square_size; x++)
        for(int y = sy; y < sy + square_size; y++) {
            board[x][y] = next_board[x][y];
        }
}

void RotateHumanAndExit(){
    for(int i=0; i<m; i++){
        int x, y;
        tie(x, y) = human[i];
        if(sx <= x && x < sx + square_size && sy <= y && y < sy + square_size){
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = square_size - ox -1;
            human[i] = make_pair(rx + sx, ry+ sy);
        }
    }

    int ex, ey;
    tie(ex, ey) = exits;
    if(sx <= ex && ex < sx + square_size && sy <= ey && ey < sy + square_size){
        int ox = ex - sx, oy = ey - sy;
        int rx = oy, ry = square_size - ox -1;
        exits = make_pair(rx + sx, ry+ sy);
    }
}

bool all_escape(){
    for(int i=0; i<m; i++){
        if(!(human[i].first == exits.first && human[i].second == exits.second))
            return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int num;
            cin >> num;
            if(num) board[i][j] = num;
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
        if(all_escape()) break;
        MinSquare();
        Rotate();
        RotateHumanAndExit();
    }
    cout << ans << "\n";
    cout << exits.first+1 << " " << exits.second+1;
    return 0;
}