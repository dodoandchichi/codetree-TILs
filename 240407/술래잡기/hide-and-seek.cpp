#include <iostream>
#include <tuple>
#include <vector>

#define MAX_N 100
#define DIR_NUM 4
using namespace std;

int n, m, h, k;
int seeker_next[MAX_N][MAX_N];
int seeker_rev[MAX_N][MAX_N];
vector<int> hiders[MAX_N][MAX_N];
vector<int> copy_hiders[MAX_N][MAX_N];
bool tree[MAX_N][MAX_N];
bool forward_facing = true;
int turn = 1, ans;
pair<int, int> seeker;

int dx[DIR_NUM] = {-1, 0, 1, 0};
int dy[DIR_NUM] = {0, 1, 0, -1};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Initalize(){
    int cur_x = n/2, cur_y = n/2;
    int move_dir = 0, move_cnt = 1;

    while(cur_x || cur_y){
        for(int i=0; i<move_cnt; i++){
            seeker_next[cur_x][cur_y] = move_dir;
            cur_x += dx[move_dir], cur_y += dy[move_dir];
            seeker_rev[cur_x][cur_y] = (move_dir < 2) ? (move_dir + 2) : (move_dir - 2);

            if(!(cur_x || cur_y)) break;
        }
        move_dir = (move_dir + 1) % 4;
        if(move_dir == 0 || move_dir == 2) move_cnt++;
    }
}

int DistFromSeeker(int x, int y){
    int sx, sy;
    tie(sx, sy) = seeker;

    return abs(x-sx) + abs(y-sy);
}

void Hider_Move(int x, int y, int dir){
    int new_x = x + dx[dir];
    int new_y = y + dy[dir];

    int sx, sy;
    tie(sx, sy) = seeker;

    if(InRange(new_x, new_y)){
        if(sx == new_x && sy == new_y){
            return;
        }
        else{
            copy_hiders[new_x][new_y].push_back(dir);
        }
    }
    else{
        dir = (dir + 2) % 4;
        new_x = x + dx[dir];
        new_y = y + dy[dir];
        if(sx == new_x && sy == new_y){
            return;
        }
        else{
            copy_hiders[new_x][new_y].push_back(dir);
        }
    }
}

void HiderMoveAll(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_hiders[i][j].clear();
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(DistFromSeeker(i, j) <= 3){
                for(int k=0; k<(int)hiders[i][j].size(); k++){
                    Hider_Move(i, j, hiders[i][j][k]);
                }
            }
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            hiders[i][j] = copy_hiders[i][j];
        }
    }
}

void SeekerMove(){
    int x, y, move_dir;
    tie(x, y) = seeker;

    if(forward_facing){
        move_dir = seeker_next[x][y]; 
    }
    else move_dir = seeker_rev[x][y];
    
    int new_x = x + dx[move_dir];
    int new_y = y + dy[move_dir];


    seeker = make_pair(new_x, new_y);

    if(seeker == make_pair(0, 0) && forward_facing){
        forward_facing = false;
    }
    else if(seeker == make_pair(n/2, n/2) && !forward_facing){
        forward_facing = true;
    }

    int sx, sy, dir;
    tie(sx, sy) = seeker;
    if(forward_facing) dir = seeker_next[sx][sy];
    else dir = seeker_rev[sx][sy];

    for(int i=0; i<=2; i++){
        int new_x = sx + dx[dir] * i;
        int new_y = sy + dy[dir] * i;
        if(InRange(new_x, new_y) && !tree[new_x][new_y] && hiders[new_x][new_y].size() > 0){
            ans += hiders[new_x][new_y].size() * turn;
            hiders[new_x][new_y].clear();
        }
    }
}

void Simulate(){
    HiderMoveAll();
    SeekerMove();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> h >> k;

    for(int i=0; i<m; i++){
        int x, y, d;
        cin >> x >> y >> d;
        hiders[x-1][y-1].push_back(d);
    }

    int r, c;
    cin >> r >> c;
    tree[r-1][c-1] = true;

    Initalize();
    seeker = make_pair(n/2, n/2);
    while(k--){
        Simulate();
        turn++;
    }
    cout << ans;
    return 0;
}