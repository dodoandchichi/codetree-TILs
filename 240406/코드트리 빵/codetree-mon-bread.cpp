#include <iostream>
#include <queue>
#include <tuple>

#define MAX_N 16
#define DIR_NUM 4
using namespace std;

int n, m;
int base[MAX_N][MAX_N]; 
pair<int, int> cvs_list[MAX_N * 2];
pair<int, int> people[MAX_N * 2];
bool visited[MAX_N][MAX_N];
int step[MAX_N][MAX_N];
queue<pair<int, int>> q;

int turn;
int dx[DIR_NUM] = {-1, 0, 0, 1}, dy[DIR_NUM] = {0, -1, 1, 0};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Init(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            step[i][j] = 0;
            visited[i][j] = false;
        }
    }
}

void BFS(pair<int, int> cvs_list){
    q.push(cvs_list);
    visited[cvs_list.first][cvs_list.second] = true;

    while(!q.empty()){
        pair<int, int> cur_pos = q.front();
        int x = cur_pos.first;
        int y = cur_pos.second;
        q.pop();

        for(int i=0; i<DIR_NUM; i++){
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            if(InRange(new_x, new_y) && !visited[new_x][new_y] && base[new_x][new_y] != 2){
                visited[new_x][new_y] = true;
                q.push(make_pair(new_x, new_y));
                step[new_x][new_y] = step[x][y] + 1;
            }
        }
    }
}

void Move(){
    for(int i=0; i<m; i++){
        if(people[i] == make_pair(-1, -1) || people[i] == cvs_list[i]) continue;
        Init();
        BFS(cvs_list[i]);
        int x, y, nx, ny;
        tie(x, y) = people[i];
        step[x][y] = abs(x-cvs_list[i].first) + abs(y-cvs_list[i].second);
        int min_dist = step[x][y];

        for(int j=0; j<DIR_NUM; j++){
            int new_x = x + dx[j];
            int new_y = y + dy[j];
            int cur_dist = step[new_x][new_y];
            if(InRange(new_x, new_y) && min_dist > cur_dist 
                && visited[new_x][new_y] && base[new_x][new_y] != 2){
                nx = new_x;
                ny = new_y;
                min_dist = cur_dist;
            }
        }
        if(cvs_list[i] == make_pair(nx, ny)){
            base[nx][ny] = 2;
        }
        people[i] = make_pair(nx, ny);
    }
}

bool NeedUpdate(pair<int, int> best_base, pair<int, int> cur_base){
    if(best_base == make_pair(-1, -1)) return true;

    int best_x, best_y;
    tie(best_x, best_y) = best_base;

    int cur_x, cur_y;
    tie(cur_x, cur_y) = cur_base;

    return make_tuple(-step[best_x][best_y], -best_x, -best_y) < 
            make_tuple(-step[cur_x][cur_y], -cur_x, -cur_y);
}

void GoBase(){
    if(turn <= m){ 
        Init();
        BFS(cvs_list[turn-1]);

        pair<int, int> best_base = make_pair(-1, -1);

        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                if(base[i][j] == 1){
                    pair<int, int> cur_base = make_pair(i, j);
                    if(visited[i][j] && NeedUpdate(best_base, cur_base)) best_base = cur_base;
                }
            }
        }
        int nx, ny;
        tie(nx, ny) = best_base;
        people[turn-1] = make_pair(nx, ny);
        base[nx][ny] = 2;
    }
}

void Simulate(){
    Move(); // 1번, 편의점 방향을 향해 1칸 이동
    /*
    cout << "people\n";
    for(int i=0; i<m; i++){
        cout << people[i].first << " " << people[i].second << "\n";
    }
    */
    GoBase(); // 3번, 가장 가까운 베이스캠프로 들어감
    /*
    cout << "base\n";
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                cout << base[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
        */
}

bool End(){
    for(int i=0; i<m; i++){
        if(!(people[i] == cvs_list[i])) return false;
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int num;
            cin >> num;
            if(num) base[i][j] = true;
        }
    }

    for(int i=0; i<m; i++){
        int x, y;
        cin >> x >> y;
        cvs_list[i] = make_pair(x-1, y-1);
    }

    for(int i=0; i<m; i++) people[i] = make_pair(-1, -1);

    while(1){
        turn++;
        Simulate();
        if(End()) break;
    }
    cout << turn;
    return 0;
}