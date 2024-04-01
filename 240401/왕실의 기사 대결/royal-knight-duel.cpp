#include<iostream>
#include<vector>
#include<string.h>

using namespace std;

struct INFO {
    int r;
    int c;
    int h;
    int w;
    int k;
};

int L,N,Q;
int map[41][41];
int player_map[41][41];
int copy_map[41][41];
int visit[41][41];
vector<INFO> player;
vector<int> v_first;

int dy[4] = {-1,0,1,0};
int dx[4] = {0,1,0,-1};

int trial = 0;
void print() {
    trial++;
    cout << "***" << trial << "th trial***" << "\n";
    for(int i=1;i<=L;i++){
        for(int j=1;j<=L;j++){
            cout << player_map[i][j] << " ";
        }
        cout << "\n";
    }

    for(int v=1;v<=N;v++){
        cout << player[v].k << " ";
    }
    cout << "\n";
}

int possible_move(int n, int d) {
    int flag = 0;
    for(int i=1;i<=L;i++){
        for(int j=1;j<=L;j++){
            if(player_map[i][j] == n){
                int now_y = i;
                int now_x = j;
                int next_y = i + dy[d];
                int next_x = j + dx[d];
                if(map[next_y][next_x] == 2){
                    flag = 1;
                    break;
                }
                if(next_y <= 0 || next_y > L || next_x <= 0 || next_x > L){
                    flag = 1;
                    break;
                }
                if(player_map[next_y][next_x] != 0 && player_map[next_y][next_x] != n){
                    int new_n = player_map[next_y][next_x];
                    int new_flag = possible_move(new_n,d);
                    if(new_flag == 1){
                        flag = 1;
                        break;
                    }
                }
            }
        }
        if(flag == 1){
            break;
        }
    }
    return flag;
}

void move(int n, int d, int f) {
    int now_flag = -1;
    if(f == 0){
        now_flag = possible_move(n,d);
    }else{
        now_flag = 0;
    }
    
    if(now_flag == 0){
        for(int i=1;i<=L;i++){
            for(int j=1;j<=L;j++){
                if(player_map[i][j] == n && visit[i][j] == 0){
                    int now_y = i;
                    int now_x = j;
                    int next_y = now_y + dy[d];
                    int next_x = now_x + dx[d];

                    copy_map[next_y][next_x] = player_map[now_y][now_x];
                    visit[now_y][now_x] = 1;

                    if(map[next_y][next_x] == 1 && f == 1){
                        player[n].k--;
                    }

                    if(player_map[next_y][next_x] != 0 && player_map[next_y][next_x] != n){
                        move(player_map[next_y][next_x],d,1);
                    }
                }
            }
        }

        if(f == 0){
            for(int i=1;i<=L;i++){
                for(int j=1;j<=L;j++){
                    if(visit[i][j] == 1){
                        player_map[i][j] = 0;
                    }
                }
            }

            for(int i=1;i<=L;i++){
                for(int j=1;j<=L;j++){
                    if(copy_map[i][j] > 0){
                        player_map[i][j] = copy_map[i][j];
                    }
                }
            }
        }
    }
}

int main() {
    cin >> L >> N >> Q;
    for(int i=1;i<=L;i++){
        for(int j=1;j<=L;j++){
            cin >> map[i][j];
        }
    }
    player.push_back({0,0,0,0,0});
    v_first.push_back(0);
    for(int i=0;i<N;i++){
        int a, b, c, d, e;
        cin >> a >> b >> c >> d >> e;
        player.push_back({a,b,c,d,e});
        v_first.push_back(e);
    }
    for(int v=1;v<=N;v++){
        int yy = player[v].r;
        int xx = player[v].c;
        int hh = player[v].h;
        int ww = player[v].w;

        for(int i=yy;i<yy+hh;i++){
            for(int j=xx;j<xx+ww;j++){
                player_map[i][j] = v;
            }
        }
    }
    for(int i=0;i<Q;i++){
        int a, b;
        cin >> a >> b;
        if(player[a].k > 0){
            memset(copy_map,0,sizeof(copy_map));
            memset(visit,0,sizeof(visit));
            move(a,b,0);
        }

        for(int v=1;v<=N;v++){
            if(player[v].k <= 0){
                for(int i=1;i<=N;i++){
                    for(int j=1;j<=N;j++){
                        if(player_map[i][j] == v){
                            player_map[i][j] = 0;
                        }
                    }
                }
            }
        }
    }

    int result = 0;
    for(int v=1;v<=N;v++){
        if(player[v].k > 0){
            result += v_first[v] - player[v].k;
        }
    }

    cout << result;

    return 0;
}