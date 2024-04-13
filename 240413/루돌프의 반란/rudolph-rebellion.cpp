#include <iostream>
#include <climits>
#include <tuple>

#define MAX_N 500
#define MAX_P 500
#define DIR_NUM 4
using namespace std;

int n, m, p, c, d;
pair<int, int> rudolf;
pair<int, int> santa[MAX_P];
int grid[MAX_N][MAX_N];
int point[MAX_P];
int stun[MAX_P];
bool lived[MAX_P];

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Print(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << grid[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Rudolf_Move(){
    tuple<int, int, int> closest_santa = make_tuple(INT_MIN, -1, -1);
    int closest_idx = -1;
    int rx, ry;
    tie(rx, ry) = rudolf;

    for(int i=1; i<=p; i++){
        if(!lived[i]) continue;
        int santa_x, santa_y;
        tie(santa_x, santa_y) = santa[i];

        int dist = (santa_x - rx) * (santa_x - rx) + (santa_y - ry) * (santa_y - ry);
        tuple<int, int, int> cur_santa = make_tuple(-dist, santa_x, santa_y);
        if(closest_santa < cur_santa){
            closest_santa = cur_santa;
            closest_idx = i;
        }
    }

    int x, y;
    tie(x, y) = santa[closest_idx];

    int move_x = 0;
    if(x < rx) move_x--;
    else if(x > rx) move_x++;

    int move_y = 0;
    if(y < ry) move_y--;
    else if(y > ry) move_y++;

    rudolf = make_pair(rx + move_x, ry + move_y); 
    grid[rx][ry] = 0;
    grid[rx + move_x][ry + move_y] = -1;

    if(x == rx + move_x && y == ry + move_y){
        x = x + move_x * c;
        y = y + move_y * c;
        point[closest_idx] += c;
        stun[closest_idx] = 2; 

        int first_x = x, first_y = y;
        int last_x = first_x, last_y = first_y;
        if(!InRange(first_x, first_y)){
            lived[closest_idx] = false;
            return;
        }
        while(grid[last_x][last_y] != 0){
            last_x += move_x;
            last_y += move_y;
        }

        while(!(first_x == last_x && first_y == last_y)){
            int before_x = last_x - move_x;
            int before_y = last_y - move_y;

            int santa_idx = grid[before_x][before_y];
            grid[last_x][last_y] = grid[before_x][before_y];
            santa[santa_idx] = make_pair(last_x, last_y);
            if(!InRange(last_x, last_y)){
                lived[santa_idx] = false;
            }
            last_x = before_x, last_y = before_y;
        }
     
        santa[closest_idx] = make_pair(first_x, first_y);
        grid[first_x][first_y] = closest_idx;
    }
}

void Santa_Move(){
    int dx[DIR_NUM] = {-1, 0, 1, 0}, dy[DIR_NUM] = {0, 1, 0, -1};

    for(int i=1; i<=p; i++){
        if(lived[i] == false || stun[i] != 0) continue;

        int x, y;
        tie(x, y) = santa[i];
        int min_x = x, min_y = y, dir;

        int minDist = (rudolf.first - x) * (rudolf.first - x) + (rudolf.second - y) * (rudolf.second - y);
        for(int j=0; j<DIR_NUM; j++){
            int new_x = x + dx[j];
            int new_y = y + dy[j];
            if(InRange(new_x, new_y) && grid[new_x][new_y] <= 0){
                int cur_dist = (rudolf.first - new_x) * (rudolf.first - new_x) + (rudolf.second - new_y) * (rudolf.second - new_y);
                if(cur_dist < minDist){
                    minDist = cur_dist;
                    min_x = new_x, min_y = new_y;
                    dir = j;
                }
            }
        }
        
        if(rudolf == make_pair(min_x, min_y)){
            grid[x][y] = 0;
            x = min_x - dx[dir] * d;
            y = min_y - dy[dir] * d;
            // cout << x << " " << y << "\n";
            stun[i] = 2;
            point[i] += d;

            int first_x = x, first_y = y;
            int last_x = first_x, last_y = first_y;
            
            if(!InRange(first_x, first_y)){
                lived[i] = false;
                continue;
            }

            while(grid[last_x][last_y] != 0){
                last_x -= dx[dir];
                last_y -= dy[dir];
            }

            while(!(first_x == last_x && first_y == last_y)){
                int before_x = last_x + dx[dir];
                int before_y = last_y + dy[dir];

                int santa_idx = grid[before_x][before_y];
                grid[last_x][last_y] = grid[before_x][before_y];
                santa[santa_idx] = make_pair(last_x, last_y);
                if(!InRange(last_x, last_y)){
                    lived[santa_idx] = false;
                }
                last_x = before_x, last_y = before_y;
            }
            if(!InRange(first_x, first_y)){
                lived[i] = false;
            }
            else {
                santa[i] = make_pair(first_x, first_y);
                grid[first_x][first_y] = i;
            }
        }
        else{
            santa[i] = make_pair(min_x, min_y);
            grid[x][y] = 0;
            grid[min_x][min_y] = i;
        }
    }
}

void DecStun(){
    for(int i=1; i<=p; i++){
        if(stun[i] > 0) stun[i]--;
    }
}

void LivePoint(){
    for(int i=1; i<=p; i++){
        if(lived[i]){
            point[i] += 1;
        }
    }
}

bool End(){
    for(int i=1; i<=p; i++){
        if(lived[i]) return false;
    }
    return true;
}

bool Simulate(){
    // cout << "before rudolf\n";
    // Print();
    Rudolf_Move();
    if(End()) return true;
    // cout << "After rudolf\n";
    // Print();
    Santa_Move();
    if(End()) return true;
    // cout << "After santa\n";
    // Print();
    DecStun();
    LivePoint();

    return false;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> p >> c >> d;
    int r, c;
    cin >> r >> c;
    rudolf = make_pair(r-1, c-1);
    grid[r-1][c-1] = -1;

    for(int i=1; i<=p; i++){
        int num, x, y;
        cin >> num >> x >> y;
        santa[num] = make_pair(x-1, y-1);
        grid[x-1][y-1] = num;
        lived[num] = true;
    }

    while(m--){
        bool is_end = Simulate();
        if(is_end) break;
    }
    for(int i=1; i<=p; i++) cout << point[i] << " ";
    return 0;
}

/*
    배열을 크게 해야만 통과가 된다.. 왜 일까,, 어렵다ㅡ,,,
*/