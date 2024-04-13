#include <iostream>
#include <tuple>
#include <vector>
#include <algorithm>

#define MAX_N 21
#define MAX_M 31
#define DIR_NUM 4
#define Player tuple<int, int, int, int, int, int> 
#define EMPTY make_tuple(-1, -1, -1, -1, -1, -1)

using namespace std;

int n, m, k;
vector<int> gun[MAX_N][MAX_N];
Player players[MAX_M];
int point[MAX_M];

int dx[DIR_NUM] = {-1, 0, 1, 0};
int dy[DIR_NUM] = {0, 1, 0, -1};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

Player OtherPlayer(int x, int y){
    for(int i=1; i<=m; i++){
        int nx, ny;
        tie(ignore, nx, ny, ignore, ignore, ignore) = players[i];
        if(x == nx && y == ny) return players[i];
    }
    return EMPTY;
}

// player의 위치를 업데이트하고 총을 줍는 함수
void Move(Player player, int new_x, int new_y, int new_dir){
    int num, x, y, d, s, a;
    tie(num, x, y, d, s, a) = player;

    if(gun[new_x][new_y].size() > 0){
        sort(gun[new_x][new_y].begin(), gun[new_x][new_y].end());
        int floor_g = gun[new_x][new_y].back();

        if(a < floor_g){
        players[num] = make_tuple(num, new_x, new_y, new_dir, s, floor_g);
        gun[new_x][new_y].pop_back();
        gun[new_x][new_y].push_back(a);
        }
        else{
            players[num] = make_tuple(num, new_x, new_y, new_dir, s, a);
        }
    }
    else{
        players[num] = make_tuple(num, new_x, new_y, new_dir, s, a);
    }
}

void LoserMove(Player player){
    int num, x, y, d, s, a;
    tie(num, x, y, d, s, a) = player;
    /*
    cout << "after match\n";
    cout << num << " " << x << " " << y << " " << d << "\n"; 
    */
    gun[x][y].push_back(a);
    int new_x = x + dx[d];
    int new_y = y + dy[d];

    Player next_player = OtherPlayer(new_x, new_y);

    while(!InRange(new_x, new_y) || next_player != EMPTY){
        d = (d + 1) % 4;
        new_x = x + dx[d];
        new_y = y + dy[d];

        next_player = OtherPlayer(new_x, new_y);
    }
    players[num] = make_tuple(num, new_x, new_y, d, s, 0);
    Move(players[num], new_x, new_y, d);
}

void Match(Player player1, Player player2){
    int num1, num2, s1, a1, s2, a2, x1, y1, x2, y2, d1, d2;
    tie(num1, x1, y1, d1, s1, a1) = player1;
    tie(num2, x2, y2, d2, s2, a2) = player2;
    if(make_pair(s1+a1, s1) < make_pair(s2+a2, s2)){
        int score = (s2+a2) - (s1+a1);
        point[num2] += score;
        LoserMove(player1);
        Move(player2, x2, y2, d2);
    }
    else{
        int score = (s1+a1) - (s2+a2);
        point[num1] += score;
        LoserMove(player2);
        Move(player1, x1, y1, d1);
    }
}

void Simulate(){
    /*
    for(int i=1; i<=m; i++){
        cout << get<0>(players[i]) << ": " << get<1>(players[i]) << " " << get<2>(players[i]) << " " << get<3>(players[i])<<"\n";
    }
    cout << "\n";*/

    for(int i=1; i<=m; i++){
        int num, x, y, dir, s, a;
        tie(num, x, y, dir, s, a) = players[i];

        int new_x = x + dx[dir];
        int new_y = y + dy[dir];
        if(!InRange(new_x, new_y)){
            dir = (dir + 2) % 4;
        }
        new_x = x + dx[dir];
        new_y = y + dy[dir];
        // 격자 밖이라면 이동까지 구현했음

        Player next_player = OtherPlayer(new_x, new_y);
        if(next_player == EMPTY){
            Move(players[i], new_x, new_y, dir);
        }
        else{
            /*
            cout << "before match\n";
            cout << num << " " << new_x << " " << new_y << " " << dir << "\n";
            */
            players[i] = make_tuple(num, new_x, new_y, dir, s, a);
            Match(players[i], next_player);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int num;
            cin >> num;
            if(num) gun[i][j].push_back(num);
        }
    }

    for(int i=1; i<=m; i++){
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        players[i] = make_tuple(i, x-1, y-1, d, s, 0);
    }

    while(k--){
        Simulate();
    }
    for(int i=1; i<=m; i++) cout << point[i] << " ";
    return 0;
}


/*
    격자 밖으로 넘어가서 방향바꾼걸 파라미터로 넘겨주고 move 함수안에서
    new_dir로 make_tuple해야하는데 d로 하고그랬음..

    파라미터로 넘겨준 튜플을 업데이트하는게 아니라 번호를 추적해서 업데이트해야함
*/