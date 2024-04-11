#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#define MAX_N 100
#define MAX_NUM 100
using namespace std;

int n = 3, m = 3, r, c, k;
int grid[MAX_N+1][MAX_N+1];

int Rowplay(int row){
    vector<pair<int, int>> nums; // 빈도수, 숫자

    for(int num=1; num<=MAX_NUM; num++){
        int freq = 0;
        for(int col=0; col<m; col++){
            if(grid[row][col] == num)
                freq++;
        }
        if(freq) nums.push_back(make_pair(freq, num));
    }

    for(int i=0; i<m; i++) grid[row][i] = 0;

    sort(nums.begin(), nums.end());

    for(int i=0; i<nums.size(); i++){
        pair<int, int> temp = nums[i];
        int freq, num;
        tie(freq, num) = temp;
        grid[row][i*2] = num;
        grid[row][i*2 + 1] = freq;
    }

    return (int) nums.size() * 2;
}

int Colplay(int col){
    vector<pair<int, int>> nums; // 빈도수, 숫자

    for(int num=1; num<=MAX_NUM; num++){
        int freq = 0;
        for(int row=0; row<n; row++){
            if(grid[row][col] == num)
                freq++;
        }
        if(freq) nums.push_back(make_pair(freq, num));
    }

    for(int i=0; i<n; i++) grid[i][col] = 0;

    sort(nums.begin(), nums.end());

    for(int i=0; i<nums.size(); i++){
        pair<int, int> temp = nums[i];
        int freq, num;
        tie(freq, num) = temp;
        grid[i*2][col] = num;
        grid[i*2+1][col] = freq;
    }

    return (int) nums.size() * 2;
}

void Simulate(){
    int maxCol = 0, maxRow = 0;

    if(n>=m){
        for(int i=0; i<n; i++){
            maxCol = max(maxCol, Rowplay(i));
        }
        m = maxCol;
    }
    else{
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
    for(int i=1; i<=100; i++){
        Simulate();
        if(grid[r-1][c-1] == k){
            ans = i;
            break;
        }
    }
    cout << ans;
    return 0;
}