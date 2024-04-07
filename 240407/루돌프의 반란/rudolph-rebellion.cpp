#include <iostream>
#include <tuple>

#define MAX_N 51
#define MAX_M 31
#define DIR_NUM 4
#define EMPTY make_pair(-1, -1)
using namespace std;

int n, m, p, c, d;
pair<int, int> santa[MAX_M];
pair<int, int> rudolf;
int grid[MAX_N][MAX_N];
int point[MAX_M];
int stun[MAX_M];
bool is_live[MAX_M];
int turn;
int dx[DIR_NUM] = {-1, 0, 1, 0};
int dy[DIR_NUM] = {0, 1, 0, -1};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

bool CloserSanta(pair<int, int> best_santa, pair<int, int> cur_santa){
    if(best_santa == EMPTY) return true;

    int best_x, best_y;
    tie(best_x, best_y) = best_santa;

    int cur_x, cur_y;
    tie(cur_x, cur_y) = cur_santa;

    int best_dist = (rudolf.first - best_x) * (rudolf.first - best_x) + (rudolf.second - best_y) * (rudolf.second - best_y);
    int cur_dist = (rudolf.first - cur_x) * (rudolf.first - cur_x) + (rudolf.second - cur_y) * (rudolf.second - cur_y);
    return make_tuple(-best_dist, best_x, best_y) < make_tuple(-cur_dist, cur_x, cur_y);
}

void Move_Rudolf(){
    int rudol_x, rudol_y;
    tie(rudol_x, rudol_y) = rudolf;
    grid[rudol_x][rudol_y] = 0;

    pair<int, int> best_santa = EMPTY;
    int closerIdx = -1;

    for(int i=1; i<=p; i++){
        if(CloserSanta(best_santa, santa[i]) && is_live[i]) {
            best_santa = santa[i];
            closerIdx = i;
        }
    }
    int santa_x, santa_y;
    tie(santa_x, santa_y) = best_santa;

    if(best_santa != EMPTY){
        int move_x = 0;
        if(rudol_x < santa_x) move_x++;
        else if(rudol_x > santa_x) move_x--;

        int move_y = 0;
        if(rudol_y < santa_y) move_y++;
        else if(rudol_y > santa_y) move_y--;

        rudolf = make_pair(rudol_x+move_x, rudol_y+move_y);
        grid[rudolf.first][rudolf.second] = -1;

        if(rudolf.first == santa_x && rudolf.second == santa_y){
            int firstX = santa_x + move_x * c;
            int firstY = santa_y + move_y * c;
            int lastX = firstX;
            int lastY = firstY;

            stun[closerIdx] = 2;
            point[closerIdx] += c;

            while(InRange(lastX, lastY) && grid[lastX][lastY] > 0){
                lastX += move_x;
                lastY += move_y;
            }

            while(!(lastX == firstX && lastY == firstY)){
                int beforeX = lastX - move_x;
                int beforeY = lastY - move_y;

                int idx = grid[beforeX][beforeY];

                if(!InRange(lastX, lastY)){
                    is_live[idx] = false;
                }
                else{
                    grid[lastX][lastY] = grid[beforeX][beforeY];
                    santa[idx] = make_pair(lastX, lastY);
                }
                
                lastX -= move_x;
                lastY -= move_y;
            }

            santa[closerIdx] = make_pair(firstX, firstY);
            if(InRange(firstX, firstY)){
                grid[firstX][firstY] = closerIdx;
            }
            else is_live[closerIdx] = false;
        }
    }
}

void Move_Santa(){
    int rudol_x, rudol_y;
    tie(rudol_x, rudol_y) = rudolf;
    grid[rudolf.first][rudolf.second] = -1;

    for(int i=1; i<=p; i++){
        if(!is_live[i] || stun[i] > 0) continue;

        int santa_x, santa_y;
        tie(santa_x, santa_y) = santa[i];

        int minDist = (santa_x - rudol_x) * (santa_x - rudol_x) + (santa_y - rudol_y) * (santa_y - rudol_y);
        int move_dir = -1;

        for(int dir=0; dir<DIR_NUM; dir++){
            int new_x = santa_x + dx[dir];
            int new_y = santa_y + dy[dir];
            if(!InRange(new_x, new_y) || grid[new_x][new_y] > 0) continue;

            int dist = (new_x - rudol_x) * (new_x - rudol_x) + (new_y - rudol_y) * (new_y - rudol_y);
            if(dist < minDist){
                minDist = dist;
                move_dir = dir;
            }
        }

        if(move_dir != -1){
            int new_x = santa_x + dx[move_dir];
            int new_y = santa_y + dy[move_dir];

            if(new_x == rudol_x && new_y == rudol_y){
                grid[santa_x][santa_y] = 0;
                stun[i] = 2;
                point[i] += d;

                int move_x = -dx[move_dir];
                int move_y = -dy[move_dir];

                int firstX = rudol_x + move_x * d;
                int firstY = rudol_y + move_y * d;
                int lastX = firstX;
                int lastY = firstY;

                while(InRange(lastX, lastY) && grid[lastX][lastY] > 0){
                    lastX += move_x;
                    lastY += move_y;
                }

                while(!(lastX == firstX && lastY == firstY)){
                    int beforeX = lastX - move_x;
                    int beforeY = lastY - move_y;

                    int idx = grid[beforeX][beforeY];

                    if(!InRange(lastX, lastY)){
                        is_live[idx] = false;
                    }
                    else{
                        grid[lastX][lastY] = grid[beforeX][beforeY];
                        santa[idx] = make_pair(lastX, lastY);
                    }
                    
                    lastX -= move_x;
                    lastY -= move_y;
                }

                santa[i] = make_pair(firstX, firstY);
                if(InRange(firstX, firstY)){
                    grid[firstX][firstY] = i;
                }
                else {
                    is_live[i] = false;
                    grid[santa_x][santa_y] = 0;
                }
            }
            else{
                santa[i] = make_pair(new_x, new_y);
                grid[santa_x][santa_y] = 0;
                grid[new_x][new_y] = i;
            }
        }
    }
}

void GetScore(){
    for(int i=1; i<=p; i++){
        if(is_live[i]) {
            point[i] += 1;
            if(stun[i] > 0) stun[i]--;
        }
    }
}

void Simulate(){
    /*
    cout << "Turn " << turn << "\n";
    */
    Move_Rudolf();
    /*
    cout << "after rudolf\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n";
    */
    
    Move_Santa();
    /*
    cout << "after santa\n";
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
    */
    GetScore();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> p >> c >> d;

    int r, c;
    cin >> r >> c;
    rudolf = make_pair(r-1, c-1);

    for(int i=1; i<=p; i++){
        int num, x, y;
        cin >> num >> x >> y;
        grid[x-1][y-1] = num;
        santa[num] = make_pair(x-1, y-1);
        is_live[num] = true;
    }

    for(int i=1; i<=m; i++){
        turn = i;
        Simulate();
    }

    for(int i=1; i<=p; i++) cout << point[i] << " ";
    return 0;
}


/*
이게 뭔 스파게티 코드인진 모르겠지만,, 디버깅을 편하게 하기 위해서 grid배열로 산타 위치를 추적했고
Move Rudolf에서 best_santa가 empty라면 에러가 떠서 조심해야함
*/