#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

#define MAX_NUM 100
#define MAX_N 100
using namespace std;

int n = 3, m = 3;
int r, c, k;
int grid[MAX_N + 1][MAX_N + 1];

int Rowplay(int row){
    vector<pair<int, int>> nums;

    for(int i=1; i<=MAX_NUM; i++){
        int freq = 0;
        for(int col=0; col<m; col++){
            if(grid[row][col] == i){
                freq++;
            }
        }
        if(freq) nums.push_back(make_pair(freq, i));
    }

    for(int col=0; col<m; col++){
        grid[row][col] = 0;
    }

    sort(nums.begin(), nums.end());
    for(int col=0; col<nums.size(); col++){
        int freq, num;
        tie(freq, num) = nums[col];
        grid[row][col*2] = num;
        grid[row][col*2+1] = freq;
    }
    return (int) 2*nums.size();
}

int Colplay(int col){
    vector<pair<int, int>> nums;

    for(int i=1; i<=MAX_NUM; i++){
        int freq = 0;
        for(int row=0; row<n; row++){
            if(grid[row][col] == i){
                freq++;
            }
        }
        if(freq) nums.push_back(make_pair(freq, i));
    }

    for(int row=0; row<n; row++){
        grid[row][col] = 0;
    }

    sort(nums.begin(), nums.end());
    for(int row=0; row<nums.size(); row++){
        int freq, num;
        tie(freq, num) = nums[row];
        grid[row*2][col] = num;
        grid[row*2+1][col] = freq;
    }
    return (int) 2*nums.size();
}

void Simulate(){
    if(n >= m){
        int maxCol = 0;
        for(int i=0; i<n; i++){
            maxCol = max(maxCol, Rowplay(i));
        }
        m = maxCol;
    }
    else{
        int maxRow = 0;
        for(int i=0; i<m; i++){
            maxRow = max(maxRow, Colplay(i));
        }
        n = maxRow;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> r >> c >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            cin >> grid[i][j];
        }
    }

    int ans = -1;
    for(int i=0; i<=100; i++){
        if(grid[r-1][c-1] == k) {
            ans = i;
            break;
        }
        Simulate();
    }
    cout << ans;
    return 0;
}

/*
    어려워보여서 해설봤는데 해설보니까 이해가 잘됨
*/