#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#define MAX_N 20
#define DIR_NUM 4
using namespace std;

int n, m, k;
vector<int> gun[MAX_N][MAX_N];
tuple<int, int, int, int> player[MAX_N][MAX_N]; // 번호, 초기 능력치, 총, 방향
int score[MAX_N + 10];
int point;
int dx[DIR_NUM] = {-1, 0, 1, 0}, dy[DIR_NUM] = {0, 1, 0 , -1};

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void AddGun(int x, int y){
    if(gun[x][y].size() > 0){
        sort(gun[x][y].begin(), gun[x][y].end());

        int s, player_g, d;
        int floor_g = gun[x][y].back();
        gun[x][y].pop_back();
        tie(s, player_g, d) = player[x][y];
        if(player_g < floor_g) {
            gun[x][y].push_back(player_g);
            player[x][y] = make_tuple(s, floor_g, d);
        }
        else gun[x][y].push_back(floor_g);
    }
}

bool Match(int x, int y, int new_x, int new_y){
    int play1_s, play1_g;
    tie(play1_s, play1_g, ignore) = player[x][y];

    int play2_s, play2_g;
    tie(play2_s, play2_g, ignore) = player[new_x][new_y];
    
    point = abs(play1_s + play1_g - play2_s - play2_g);
    // cout << play1_s << " "<< play1_g << " " << play2_s << " " << play2_g << "\n";
    return make_pair(play1_s+play1_g, play1_s) < make_pair(play2_s+play2_g, play2_s);
}

void Move(){
    int idx = 0;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int num;
            tie(num, ignore, ignore) = player[i][j];
            cout << num << " " << order[idx] << "\n";
            if(num == order[idx]){
                idx++;
                int s, g, d;
                tie(s, g, d) = player[i][j];
                int new_x = i + dx[d];
                int new_y = j + dy[d];
                if(!InRange(new_x, new_y)){
                    d = (d + 2) % 4;
                    new_x = i + dx[d];
                    new_y = j + dy[d];
                }

                if(player[new_x][new_y] == make_tuple(-1, -1, -1)){
                    player[i][j] = make_tuple(-1, -1, -1);
                    player[new_x][new_y] = make_tuple(s, g, d);
                    AddGun(new_x, new_y);
                }
                else{
                    if(Match(i, j, new_x, new_y)){
                        int win_num;
                        tie(win_num, ignore, ignore) = player[new_x][new_y];
                        for(int score_idx=0; score_idx<m; score_idx++){
                            if(order[score_idx] == win_num){
                                score[score_idx+1] += point;
                            } 
                        } 
                        int lose_s, lose_g, lose_d;
                        tie(lose_s, lose_g, lose_d) = player[i][j];
                        gun[new_x][new_y].push_back(lose_g);
                        player[i][j] = make_tuple(-1, -1, -1);

                        for(int dir=0; dir<DIR_NUM; dir++){
                            int x = new_x + dx[lose_d];
                            int y = new_y + dy[lose_d];
                            if(player[x][y] != make_tuple(-1, -1, -1) || !InRange(x, y)){
                                lose_d = (lose_d + 1) % 4;
                            }
                            else{
                                player[x][y] = make_tuple(lose_s, 0, lose_d);
                                AddGun(x, y);
                                break;
                            }
                        }
                        AddGun(new_x, new_y);
                    }
                    else{
                        int win_num;
                        tie(win_num, ignore, ignore) = player[i][j];
                        for(int score_idx=0; score_idx<m; score_idx++){
                            if(order[score_idx] == win_num){
                                score[score_idx+1] += point;
                            } 
                        } 
                        int lose_s, lose_g, lose_d;
                        tie(lose_s, lose_g, lose_d) = player[new_x][new_y];
                        gun[new_x][new_y].push_back(lose_g);
                        player[new_x][new_y] = make_tuple(-1, -1, -1);

                        for(int dir=0; dir<DIR_NUM; dir++){
                            int x = new_x + dx[lose_d];
                            int y = new_y + dy[lose_d];
                            if(player[x][y] != make_tuple(-1, -1, -1) || !InRange(x, y)){
                                lose_d = (lose_d + 1) % 4;
                            }
                            else{
                                player[x][y] = make_tuple(lose_s, 0, lose_d);
                                AddGun(x, y);
                                break;
                            }
                        }

                        AddGun(i, j);
                    }
                }
            }
        }
    }
}

void Simulate(){
    Move();
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout << get<0>(player[i][j])<< " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int num;
            cin >> num;
            if(num > 0) gun[i][j].push_back(num); 
            player[i][j] = make_tuple(-1, -1, -1, -1);
        }
    }
    
    for(int i=0; i<m; i++){
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        player[x-1][y-1] = make_tuple(i, s, 0, d);
    }

    while(k--){
        Simulate();
    }
    for(int i=0; i<m; i++) cout << score[i] << " ";
    return 0;
}