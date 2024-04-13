#include <iostream>
#include <queue>

#define MAX_N 41
#define MAX_M 31
using namespace std;

int n, m, q;
int grid[MAX_N][MAX_N];
int bef_k[MAX_N];
int r[MAX_N], c[MAX_N], h[MAX_N], w[MAX_N], k[MAX_N];
int nr[MAX_N], nc[MAX_N];
int dmg[MAX_N];
bool is_moved[MAX_N];

int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};

bool TryMovement(int idx, int dir){
    for(int i=0; i<m; i++){
        dmg[i] = 0;
        is_moved[i] = false;
        nr[i] = r[i];
        nc[i] = c[i];
    }

    queue<int> q;

    q.push(idx);
    is_moved[idx] = true;

    while(!q.empty()){
        int x = q.front(); q.pop();
        nr[x] += dx[dir];
        nc[x] += dy[dir];

        if(nr[x] < 0 || nc[x] < 0 || nr[x] + h[x] -1 > n-1 || nc[x] + w[x] -1 > n-1)
            return false;

        for(int i = nr[x]; i <= nr[x] + h[x] - 1; i++) {
            for(int j = nc[x]; j <= nc[x] + w[x] - 1; j++) {
                if(grid[i][j] == 1) 
                    dmg[x]++;
                if(grid[i][j] == 2)
                    return false;
            }
        } 

        for(int i = 0; i < m; i++) {
            if(is_moved[i] || k[i] <= 0) 
                continue;
            if(r[i] > nr[x] + h[x] - 1 || nr[x] > r[i] + h[i] - 1) 
                continue;
            if(c[i] > nc[x] + w[x] - 1 || nc[x] > c[i] + w[i] - 1) 
                continue;

            is_moved[i] = true;
            q.push(i);
        }
    }

    dmg[idx] = 0;
    return true;
}

void MovePiece(int idx, int dir){
    if(k[idx] <= 0) return;

    if(TryMovement(idx, dir)){
        for(int i=0; i<m; i++){
            r[i] = nr[i];
            c[i] = nc[i];
            k[i] -= dmg[i];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> q;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cin >> grid[i][j];
        }
    }

    for(int i=0; i<m; i++){
        cin >> r[i] >> c[i] >> h[i] >> w[i] >> k[i];
        r[i]--; c[i]--;
        bef_k[i] = k[i];
    }

    for(int i=0; i<q; i++){
        int idx, dir;
        cin >> idx >> dir;
        MovePiece(idx-1, dir);
    }

    int ans = 0;
    for(int i = 0; i < m; i++) {
        if(k[i] > 0) {
            ans += bef_k[i] - k[i];
        }
    }

    cout << ans;
    return 0;
}