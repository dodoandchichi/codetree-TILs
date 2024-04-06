#include <iostream>
#include <climits>

#define MAX_N 21
#define MAX_DRUG 10
#define DIR_NUM 4
using namespace std;

int n, m, k, c;
int tree[MAX_N][MAX_N];
int copy_tree[MAX_N][MAX_N];
bool breed[MAX_N][MAX_N];
int drug[MAX_N][MAX_N];

int max_x, max_y;
int dx[DIR_NUM] = {-1, 0, 1, 0};
int dy[DIR_NUM] = {0, 1, 0, -1};

int ans;

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Grow(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(tree[i][j] > 0){
                int cnt = 0;
                for(int k=0; k<DIR_NUM; k++){
                    int new_x = i + dx[k];
                    int new_y = j + dy[k];
                    if(InRange(new_x, new_y) && tree[new_x][new_y] > 0)
                        cnt++;
                }
                tree[i][j] += cnt;
            }
        }
    }
}

void Divide(int amount){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(breed[i][j]) {
                breed[i][j] = false;
                copy_tree[i][j] += amount;
            }
        }
    }
}

void Breed(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_tree[i][j] = 0;
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(tree[i][j] > 0){
                int can_breed = 0;
                for(int k=0; k<DIR_NUM; k++){
                    int new_x = i + dx[k];
                    int new_y = j + dy[k];
                    if(InRange(new_x, new_y) && tree[new_x][new_y] == 0 && !drug[new_x][new_y]){
                        can_breed++;
                        breed[new_x][new_y] = true;
                    }
                }
                int breed_amount = tree[i][j] / can_breed;
                if(can_breed > 0) Divide(breed_amount);
            }
        }
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            tree[i][j] += copy_tree[i][j];
        }
    }
}

void BestKill(){
    int tx[DIR_NUM] = {-1, -1, 1, 1};
    int ty[DIR_NUM] = {-1, 1, -1, 1};

    int max_cut = INT_MIN;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int tree_cut = 0;
            if(tree[i][j] > 0){
                bool CanGo[DIR_NUM] = {true, true, true, true};
                tree_cut += tree[i][j];
                for(int l=1; l<=k; l++){
                    for(int dir=0; dir<DIR_NUM; dir++){
                        if(!CanGo[dir]) continue;

                        int new_x = i + tx[dir] * l;
                        int new_y = j + ty[dir] * l;
                        if(InRange(new_x, new_y) && tree[new_x][new_y] > 0){
                            tree_cut += tree[new_x][new_y];
                        }
                        else if(InRange(new_x, new_y)){
                            if(tree[new_x][new_y] <= 0){
                                CanGo[dir] = false;
                            }
                        }
                        else if(!InRange(new_x, new_y)) CanGo[dir] = false;
                    }
                }
            }
            if(max_cut < tree_cut){
                max_x = i;
                max_y = j;
                max_cut = tree_cut;
            }
        }
    }
}

void Drug(){
    int tx[DIR_NUM] = {-1, -1, 1, 1};
    int ty[DIR_NUM] = {-1, 1, -1, 1};

    bool CanGo[DIR_NUM] = {1, 1, 1, 1};
    drug[max_x][max_y] = c+1;
    if(tree[max_x][max_y] >=0) ans += tree[max_x][max_y];
    tree[max_x][max_y] = 0;
    for(int l=1; l<=k; l++){
        for(int dir=0; dir<DIR_NUM; dir++){
            if(!CanGo[dir]) continue;

            int new_x = max_x + tx[dir] * l;
            int new_y = max_y + ty[dir] * l;
            if(InRange(new_x, new_y) && tree[new_x][new_y] > 0){
                ans += tree[new_x][new_y];
                tree[new_x][new_y] = 0;
                drug[new_x][new_y] = c+1;
            }
            else if(InRange(new_x, new_y)){
                if(tree[new_x][new_y] <= 0){
                    CanGo[dir] = false;
                    drug[new_x][new_y] = c+1;
                }
            }
        }
    }
}

void Drug_Dec(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(drug[i][j] > 0) drug[i][j]--;
        }
    }
}

void Simulate(){
    Grow();
    Breed();
    BestKill();
    Drug();
    Drug_Dec();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cin >> n >> m >> k >> c;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int num;
            cin >> num;
            tree[i][j] = num;
        }
    }
    
    while(m--){
        Simulate();
    }
    cout << ans;
    return 0;
}