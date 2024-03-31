#include <iostream>
#include <vector>
#include <tuple>

#define MAX_N 21
#define DIR_NUM 4
using namespace std;

int n, m, k, c;
int grid[MAX_N][MAX_N];
int copy_grid[MAX_N][MAX_N];
int area[MAX_N][MAX_N];
int drug[MAX_N][MAX_N];
bool CanGo[DIR_NUM];
vector<pair<int, int>> wall;
int score;

bool InRange(int x, int y){
    return 0 <= x && x < n && 0 <= y && y < n;
}

void Init(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            copy_grid[i][j] = 0;
            area[i][j] = 0;
            drug[i][j]--;
        }
    }

    for(int i=0; i<(int)wall.size(); i++){
        int x, y;
        tie(x, y) = wall[i];
        copy_grid[x][y] = -1;
    }
}

void Grow(){
    int dx[DIR_NUM] = {-1, 0, 1, 0}, dy[DIR_NUM] = {0, 1, 0, -1};

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            int cnt = 0;
            if(grid[i][j] > 0){
                for(int dir=0; dir<DIR_NUM; dir++){
                    int new_x = i + dx[dir];
                    int new_y = j + dy[dir];
                    if(InRange(new_x, new_y) && grid[new_x][new_y] > 0) cnt++;
                }
            }
            grid[i][j] += cnt;
        }
    }
}

void Breed(){
    int dx[DIR_NUM] = {-1, 0, 1, 0}, dy[DIR_NUM] = {0, 1, 0, -1};

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            vector<pair<int,int>> breed_tree;
            if(grid[i][j] > 0){
                for(int dir=0; dir<DIR_NUM; dir++){
                    int new_x = i + dx[dir];
                    int new_y = j + dy[dir];
                    if(InRange(new_x, new_y) && grid[new_x][new_y] == 0 && drug[new_x][new_y] <= 0){
                        breed_tree.push_back(make_pair(new_x, new_y));
                    }
                }
                copy_grid[i][j] = grid[i][j];
            }
            for(int k=0; k<(int)breed_tree.size(); k++){
                int x, y;
                tie(x, y) = breed_tree[k];
                copy_grid[x][y] += copy_grid[i][j] / breed_tree.size();
            }
        }
    }
}

pair<int, int> FindMaxArea(){
    int dx[DIR_NUM] = {-1, -1, 1, 1}, dy[DIR_NUM] = {-1, 1, -1, 1};

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){

            if(copy_grid[i][j]>0){
                CanGo[0] = true, CanGo[1] = true, CanGo[2] = true, CanGo[3] = true;
                area[i][j] += copy_grid[i][j];

                for(int range=1; range<=k; range++){
                    for(int dir=0; dir<DIR_NUM; dir++){
                        if(CanGo[dir]){
                            int new_x = i + dx[dir] * range;
                            int new_y = j + dy[dir] * range;
                            if(InRange(new_x, new_y)){
                                if(copy_grid[new_x][new_y] <= 0){
                                    CanGo[dir] = false;
                                }
                                else{
                                    area[i][j] += copy_grid[new_x][new_y];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    int maxArea = 0;
    pair<int, int> best_pos;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(maxArea < area[i][j]){
                maxArea = area[i][j];
                best_pos = make_pair(i, j);
            }   
        }
    }
    score += maxArea;
    return best_pos;
}

void Drug(pair<int, int> drug_pos){
    int x, y;
    tie(x, y) = drug_pos;

    int dx[DIR_NUM] = {-1, -1, 1, 1}, dy[DIR_NUM] = {-1, 1, -1, 1};
    CanGo[0] = true, CanGo[1] = true, CanGo[2] = true, CanGo[3] = true;
    drug[x][y] = c+1;
    copy_grid[x][y] = 0;

    for(int range=1; range<=k; range++){
        for(int dir=0; dir<DIR_NUM; dir++){
            if(CanGo[dir]){
                int new_x = x + dx[dir] * range;
                int new_y = y + dy[dir] * range;
                if(InRange(new_x, new_y)){
                    if(copy_grid[new_x][new_y] <= 0){
                        CanGo[dir] = false;
                        drug[new_x][new_y] = c+1;
                    }
                    else{
                        drug[new_x][new_y] = c+1;
                        copy_grid[new_x][new_y] = 0;
                    }
                }
            }
        }
    }
}

void CopyToGrid(){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            grid[i][j] = copy_grid[i][j];
        }
    }
}

void Simulate(){
    Init();
    Grow();
    Breed();
    pair<int, int> drug_pos = FindMaxArea();
    Drug(drug_pos);
    CopyToGrid();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k >> c;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin >> grid[i][j];
            if(grid[i][j] == -1) wall.push_back(make_pair(i, j));
        }
    }

    while(m--){
        Simulate();
    }
    cout << score;
    return 0;
}


/*
    딱히 막힌 건 없는듯..? drug 배열을 잘못다뤄서 오래걸린것뿐

*/