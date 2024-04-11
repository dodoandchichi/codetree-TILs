#include <iostream>
#include <tuple>

#define MAX_N 101
using namespace std;

int n, k;
pair<int, bool> u[MAX_N];
pair<int, bool> d[MAX_N];

void Shift(){
    pair<int, bool> temp1 = u[n-1];
    pair<int, bool> temp2 = d[0];

    for(int i=n-1; i>0; i--){
        u[i] = u[i-1];
    }

    for(int i=0; i<n-1; i++){
        d[i] = d[i+1];
    }

    d[n-1] = temp1;
    u[0] = temp2;

    int safe; bool human;
    tie(safe, human) = u[n-1];

    if(human) u[n-1] = make_pair(safe, false);
}

void Move(){
    for(int i=n-2; i>=0; i--){
        int safe; bool human;
        tie(safe, human) = u[i+1];
        if(safe > 0 && !human && u[i].second) {
            u[i+1] = make_pair(safe-1, true);
            int before_safe;
            tie(before_safe, ignore) = u[i];
            u[i] = make_pair(before_safe, false);
        }
    }

    int safe; bool human;
    tie(safe, human) = u[n-1];

    if(human) u[n-1] = make_pair(safe, false);
}

void Put(){
    int safe; bool human;
    tie(safe, human) = u[0];
    if(safe > 0 && !human) u[0] = make_pair(safe-1, true);
}

bool Simulate(){
    Shift();
    // cout << "after shift\n";
    // for(int i=0; i<n; i++){
    //     cout << u[i].first << " ";
    // }
    // for(int i=n-1; i>=0; i--){
    //     cout << d[i].first << " ";
    // }
    // cout << "\n";
    Move();
    // cout << "after move\n";
    // for(int i=0; i<n; i++){
    //     cout << u[i].first << " ";
    // }
    // for(int i=n-1; i>=0; i--){
    //     cout << d[i].first << " ";
    // }
    // cout << "\n";
    Put();
    // cout << "after put\n";
    // for(int i=0; i<n; i++){
    //     cout << u[i].first << " ";
    // }
    // for(int i=n-1; i>=0; i--){
    //     cout << d[i].first << " ";
    // }
    // cout << "\n";

    int cnt = 0;
    for(int i=0; i<n; i++){
        if(u[i].first == 0) cnt++;
        if(d[i].first == 0) cnt++;
    }
    if(cnt >= k) return true;
    return false;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> k;
    for(int i=0; i<n; i++) {
        int num;
        cin >> num;
        u[i] = make_pair(num, false);
    }
    for(int i=n-1; i>=0; i--) {
        int num;
        cin >> num;
        d[i] = make_pair(num, false);
    }
    int turn = 1;
    while(1){
        bool is_end = Simulate();
        if(is_end) break;
        turn++;
    }
    cout << turn;
    return 0;
}