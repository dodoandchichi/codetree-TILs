#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

#define MAX_N 100
#define DIR_NUM 4
using namespace std;

int n, m, h, k;
vector<int> escape[MAX_N][MAX_N];
vector<int> copy_escape[MAX_N][MAX_N];
bool tree[MAX_N][MAX_N];
queue<pair<int, int>> q;
pair<int, int> catcher;
bool clock_dir = true;

int dir;
int step[MAX_N][MAX_N];
bool visited[MAX_N][MAX_N];
int score, turn;
int move_cnt = 1, move_dir = 0;

int dx[DIR_NUM] = {-1, 0, 1, 0}, dy[DIR_NUM] = {0, 1, 0, -1};

void Init(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_escape[i][j].clear();
            step[i][j] = 0;
            visited[i][j] = false;
        }
    }
}

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void BFS(){
    while(!q.empty()){
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        visited[x][y] = true;
        q.pop();

        for(int i=0; i<DIR_NUM; i++){
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if(InRange(new_x, new_y) && !visited[new_x][new_y]){
                step[new_x][new_y] = step[x][y] + 1;
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
            }
        }
    }
}

void CopyToEscape(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            escape[i][j] = copy_escape[i][j];
        }
    }
}

void Escape(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(escape[i][j].size() >= 1 && step[i][j] <= 3){
                for(int k=0; k<(int)escape[i][j].size(); k++){
                    int d = escape[i][j][k];
                    int new_x = i + dx[d];
                    int new_y = j + dy[d];
                    if(InRange(new_x, new_y) && catcher != make_pair(new_x, new_y)){
                        copy_escape[new_x][new_y].push_back(d);
                    }
                    else{
                        d = (d + 2) % 4;
                        new_x = i + dx[d];
                        new_y = j + dy[d];
                        if(catcher != make_pair(new_x, new_y)){
                            copy_escape[new_x][new_y].push_back(d);
                        }
                    }
                }
            }
        }
    }
}

void Catcher(){
    int x, y;
    tie(x, y) = catcher;
    for(int i=0; i<move_cnt; i++){
        x = x + dx[move_dir];
        y = y + dy[move_dir];
        if(x == 0 && y == 0) break;
    }
    move_dir = (move_dir + 1) % 4;
    if(move_dir == 0 || move_dir == 2) move_cnt++;

    catcher = make_pair(x, y);
    q.push(catcher);

    for(int i=0; i<3; i++){
        int nx = x + dx[move_dir] * i;
        int ny = y + dy[move_dir] * i;

        if(escape[nx][ny].size() >= 1 && !tree[nx][ny]){
            score += turn * escape[nx][ny].size();
            escape[nx][ny].clear();
        }
    }

    if(x == 0 && y == 0) {
        move_dir = (move_dir + 1) % 4;
        move_cnt--;
        clock_dir = false;
    }
}

void ACatcher(){
    int x, y;
    tie(x, y) = catcher;

    for(int i=0; i<move_cnt; i++){
        x = x + dx[move_dir];
        y = y + dy[move_dir];
        if(x == n/2 && y == n/2) break;
    }
    move_dir = (move_dir + 3) % 4;
    if(!(x==n-1 && y==0) && move_dir == 1 || move_dir == 3) move_cnt--;

    catcher = make_pair(x, y);
    q.push(catcher);

    for(int i=0; i<3; i++){
        int nx = x + dx[move_dir] * i;
        int ny = y + dy[move_dir] * i;

        if(escape[nx][ny].size() >= 1 && !tree[nx][ny]){
            score += turn * escape[nx][ny].size();
            escape[nx][ny].clear();
        }
    }

    if(x == n/2 && y == n/2) {
        move_dir = (move_dir + 3) % 4;
        move_cnt++;
        clock_dir = true;
    }
}

void Simulate(){
    Init();
    BFS();
    Escape();
    CopyToEscape();
    if(clock_dir) Catcher();
    else ACatcher();
}  

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> h >> k;
    for(int i=0; i<m; i++){
        int x, y, d;
        cin >> x >> y >> d;
        escape[x-1][y-1].push_back(d);
    }

    catcher = make_pair(n/2, n/2);
    q.push(catcher);
    for(int i=0; i<h; i++){
        int x, y;
        cin >> x >> y;
        tree[x-1][y-1] = true;
    }

    for(int i=1; i<=k; i++){
        turn = i;
        Simulate();
    }
    cout << score;
    return 0;
}