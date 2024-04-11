#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

int n;
vector<pair<int, int>> times;
vector<int> selected_time;
vector<int> profit;
int ans;

int Calc(){
    int money = 0;

    for(int i=0; i<(int)selected_time.size(); i++){
        money += profit[selected_time[i]];
    }
    return money;
}

bool Possible(){
    for(int i=0; i<(int)selected_time.size()-1; i++){
        if(times[selected_time[i]].second >= times[selected_time[i+1]].first) return false;
    }

    for(int i = 0; i < (int) selected_time.size(); i++)
        if(times[selected_time[i]].second > n)
            return false;

    return true;
}

void FindMax(int idx){
    if(n == idx){
        if(Possible()){
            ans = max(ans, Calc());
        }
        return;
    }

    FindMax(idx + 1);

    selected_time.push_back(idx);
    FindMax(idx + 1);
    selected_time.pop_back();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    cin >> n;
    for(int i=1; i<=n; i++){
        int t, p;
        cin >> t >> p;
        times.push_back(make_pair(i, i+t-1));
        profit.push_back(p);
    }
    FindMax(0);
    cout << ans;
    return 0;
}