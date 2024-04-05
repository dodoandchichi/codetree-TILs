#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <functional>

#define MAX_N 20
#define MAX_M 30
#define DIR_NUM 4
#define Player tuple<int, int, int, int, int, int>
#define EMPTY make_tuple(-1, -1, -1, -1, -1, -1)

using namespace std;

// 변수 선언
int n, m, k;

// 각 칸마다 놓여있는 총 목록을 관리합니다.
vector<int> gun[MAX_N][MAX_N];

// 각 칸마다 플레이어 정보를 관리합니다.
// 순서대로 (num, x, y, d, s, a) 정보를 관리합니다.
// (x, y)위치에서 방향 d를 보고 있으며
// 초기 능력치가 s인 num번 플레이어가
// 공격력이 a인 총을 들고 있음을 뜻합니다.
// 총이 없으면 a는 0입니다.
Player players[MAX_M];

// 입력으로 주어지는
// 방향 순서대로 
// dx, dy를 정의합니다.
// ↑, →, ↓, ←
int dx[DIR_NUM] = {-1, 0, 1,  0};
int dy[DIR_NUM] = { 0, 1, 0, -1};

// 플레이어들의 포인트 정보를 기록합니다.
int points[MAX_M];

// (x, y)가 격자를 벗어나는지 확인합니다.
bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

// 현재 (x, y)위치에서 방향 d를 보고 있을 때
// 그 다음 위치와 방향을 찾아줍니다.
tuple<int, int, int> GetNext(int x, int y, int d) {
    int nx = x + dx[d], ny = y + dy[d];
    // 격자를 벗어나면
    // 방향을 뒤집어
    // 반대 방향으로 한 칸 이동합니다.
    if(!InRange(nx, ny)) {
        // 반대 방향 : 0 <-> 2 / 1 <-> 3
        d = (d < 2) ? (d + 2) : (d - 2);
        nx = x + dx[d], ny = y + dy[d];
    }

    return make_tuple(nx, ny, d);
}

// 해당 칸에 있는 Player를 찾아줍니다.
// 없다면 EMPTY를 반환합니다.
Player FindPlayer(pair<int, int> pos) {
    for(int i = 0; i < m; i++) {
        int x, y;
        tie(ignore, x, y, ignore, ignore, ignore) = players[i];

        if(pos == make_pair(x, y))
            return players[i];
    }

    return EMPTY;
}

// Player p의 정보를 갱신해줍니다.
void Update(Player p) {
    int num;
    tie(num, ignore, ignore, ignore, ignore, ignore) = p;

    // Player의 위치를 찾아
    // 값을 갱신해줍니다.
    for(int i = 0; i < m; i++) {
        int num_i;
        tie(num_i, ignore, ignore, ignore, ignore, ignore) = players[i];

        if(num_i == num) {
            players[i] = p;
            break;
        }
    }
}

// 플레이어 p를 pos 위치로 이동시켜줍니다.
void Move(Player p, pair<int, int> pos) {
    int num, x, y, d, s, a;
    tie(num, x, y, d, s, a) = p;
    
    int nx, ny;
    tie(nx, ny) = pos;

    // 가장 좋은 총으로 갱신해줍니다.
    gun[nx][ny].push_back(a);
    sort(gun[nx][ny].begin(), gun[nx][ny].end(), greater<int>());
    a = gun[nx][ny][0];
    gun[nx][ny].erase(gun[nx][ny].begin());

    p = make_tuple(num, nx, ny, d, s, a);
    Update(p);
}

// 진 사람의 움직임을 진행합니다.
// 결투에서 패배한 위치는 pos입니다.
void LoserMove(Player p) {
    int num, x, y, d, s, a;
    tie(num, x, y, d, s, a) = p;
    
    // 먼저 현재 위치에 총을 내려놓게 됩니다.
    gun[x][y].push_back(a);

    // 빈 공간을 찾아 이동하게 됩니다.
    // 현재 방향에서 시작하여
    // 90'씩 시계방향으로
    // 회전하다가 
    // 비어있는 최초의 곳으로 이동합니다.
    for(int i = 0; i < 4; i++) {
        int ndir = (d + i) % 4;
        int nx = x + dx[ndir], ny = y + dy[ndir];
        if(InRange(nx, ny) && FindPlayer(make_pair(nx, ny)) == EMPTY) {
            p = make_tuple(num, x, y, ndir, s, 0);
            Move(p, make_pair(nx, ny));
            break;
        }
    }
}

// p2과 p2가 pos에서 만나 결투를 진행합니다.
void Duel(Player p1, Player p2, pair<int, int> pos) {
    int num1, d1, s1, a1;
    tie(num1, ignore, ignore, d1, s1, a1) = p1;

    int num2, d2, s2, a2;
    tie(num2, ignore, ignore, d2, s2, a2) = p2;

    // (초기 능력치 + 총의 공격력, 초기 능력치) 순으로 우선순위를 매겨 비교합니다.

    // p1이 이긴 경우
    if(make_pair(s1 + a1, s1) > make_pair(s2 + a2, s2)) {
        // p1은 포인트를 얻게 됩니다.
        points[num1] += (s1 + a1) - (s2 + a2);
        // p2는 진 사람의 움직임을 진행합니다.
        LoserMove(p2);
        // 이후 p1은 이긴 사람의 움직임을 진행합니다.
        Move(p1, pos);
    }
    // p2가 이긴 경우
    else {
        // p2는 포인트를 얻게 됩니다.
        points[num2] += (s2 + a2) - (s1 + a1);
        // p1은 진 사람의 움직임을 진행합니다.
        LoserMove(p1);
        // 이후 p2는 이긴 사람의 움직임을 진행합니다.
        Move(p2, pos);
    }
}

// 1라운드를 진행합니다.
void Simulate() {
    // 첫 번째 플레이어부터 순서대로 진행합니다.
    for(int i = 0; i < m; i++) {
        int num, x, y, d, s, a;
        tie(num, x, y, d, s, a) = players[i];


        // Step 1-1. 현재 플레이어가 움직일 그 다음 위치와 방향을 구합니다.
        int nx, ny, ndir;
        tie(nx, ny, ndir) = GetNext(x, y, d);
        
        // 해당 위치에 있는 전 플레이어 정보를 얻어옵니다.
        Player next_player = FindPlayer(make_pair(nx, ny));
        
        // 현재 플레이어의 위치와 방향을 보정해줍니다.
        Player curr_player = make_tuple(num, nx, ny, ndir, s, a);
        Update(curr_player);
        
        // Step 2. 해당 위치로 이동해봅니다.
        // Step 2-1. 해당 위치에 플레이어가 없다면 그대로 움직입니다.
        if(next_player == EMPTY)
            Move(curr_player, make_pair(nx, ny));
        // Step 2-2. 해당 위치에 플레이어가 있다면 결투를 진행합니다.
        else
            Duel(curr_player, next_player, make_pair(nx, ny));
    }
}

int main() {
    // 입력:
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cin >> n >> m >> k;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            int num;
            cin >> num;

            // 총이 놓여 있는 칸입니다.
            if(num != 0)
                gun[i][j].push_back(num);
        }

    // 플레이어 정보를 입력받습니다.
    for(int i = 0; i < m; i++) {
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        players[i] = make_tuple(i, x - 1, y - 1, d, s, 0);
    }

    // k번에 걸쳐 시뮬레이션을 진행합니다.
    while(k--)
        Simulate();
    
    // 각 플레이어가 획득한 포인트를 출력합니다.
    for(int i = 0; i < m; i++)
        cout << points[i] << " ";
    return 0;
}