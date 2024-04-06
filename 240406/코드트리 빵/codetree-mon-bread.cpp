#include <iostream>
#include <tuple>
#include <queue>
#include <climits>

#define MAX_N 15
#define MAX_M 30
#define DIR_NUM 4
#define EMPTY make_pair(-1, -1)

using namespace std;

// 변수 선언
int n, m;

// 0이면 빈 칸, 1이면 베이스 캠프, 2라면 아무도 갈 수 없는 곳을 뜻합니다.
int grid[MAX_N][MAX_N];

// 편의점 목록을 관리합니다.
pair<int, int> cvs_list[MAX_M];

// 현재 사람들의 위치를 관리합니다.
pair<int, int> people[MAX_M];

// 현재 시간을 기록합니다.
int curr_t;

// dx, dy값을 
// 문제에서의 우선순위인 상좌우하 순으로 적어줍니다.
int dx[DIR_NUM] = {-1,  0, 0, 1};
int dy[DIR_NUM] = { 0, -1, 1, 0};

// bfs에 사용되는 변수들입니다.
int step[MAX_N][MAX_N];      // 최단거리 결과 기록
bool visited[MAX_N][MAX_N];  // 방문 여부 표시

// (x, y)가 격자 내에 있는 좌표인지를 판단합니다.
bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

// (x, y)로 이동이 가능한지 판단합니다.
bool CanGo(int x, int y) {
    return InRange(x, y) &&       // 범위를 벗어나지 않으면서 
           !visited[x][y] &&      // 방문했던 적이 없으면서
           grid[x][y] != 2;       // 이동 가능한 곳이어야 합니다.
}

// start_pos를 시작으로 하는 BFS를 진행합니다.
// 시작점으로부터의 최단거리 결과는 step배열에 기록됩니다.
void BFS(pair<int, int> start_pos) {
    // visited, step 값을 전부 초기화합니다.
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            visited[i][j] = false;
            step[i][j] = 0;
        }
    
    // 초기 위치를 넣어줍니다.
    queue<pair<int, int> > q;
    int sx = start_pos.first, sy = start_pos.second;
    q.push(start_pos);
    visited[sx][sy] = true;
    step[sx][sy] = 0;

    // BFS를 진행합니다.
    while(!q.empty()) {
        // 가장 앞에 원소를 골라줍니다.
        pair<int, int> curr_pos = q.front();
        q.pop();

        // 인접한 칸을 보며 아직 방문하지 않은 칸을 큐에 넣어줍니다.
        int x = curr_pos.first, y = curr_pos.second;
        for(int i = 0; i < DIR_NUM; i++) {
            int nx = x + dx[i], ny = y + dy[i];
            // 갈 수 있는 경우에만 진행합니다.
            if(CanGo(nx, ny)) {
                visited[nx][ny] = true;
                step[nx][ny] = step[x][y] + 1;
                q.push(make_pair(nx, ny));
            }
        }
    }
}

// 시뮬레이션을 진행합니다.
void Simulate() {
    // Step 1. 격자에 있는 사람들에 한하여 편의점 방향을 향해 1칸 움직입니다.
    for(int i = 0; i < m; i++) {
        // 아직 격자 밖에 있는 사람이거나 이미 편의점에 도착한 사람이라면 패스합니다.
        if(people[i] == EMPTY || people[i] == cvs_list[i])
            continue;
        
        // 원래는 현재 위치에서 편의점 위치까지의 최단거리를 구해줘야 합니다.
        // 다만 최단거리가 되기 위한 그 다음 위치를 구하기 위해서는
        // 거꾸로 편의점 위치를 시작으로 현재 위치까지 오는 최단거리를 구해주는 것이 필요합니다.
        // 따라서 편의점 위치를 시작으로 하는 BFS를 진행합니다.
        BFS(cvs_list[i]);

        int px, py;
        tie(px, py) = people[i];
        // 현재 위치에서 상좌우하 중 최단거리 값이 가장 작은 곳을 고르면
        // 그 곳으로 이동하는 것이 최단거리 대로 이동하는 것이 됩니다.
        // 그러한 위치 중 상좌우하 우선순위대로 가장 적절한 곳을 골라줍니다.
        int min_dist = INT_MAX;
        int min_x = -1, min_y = -1;
        for(int j = 0; j < DIR_NUM; j++) {
            int nx = px + dx[j], ny = py + dy[j];
            if(InRange(nx, ny) && visited[nx][ny] && min_dist > step[nx][ny]) {
                min_dist = step[nx][ny];
                min_x = nx; min_y = ny;
            }
        }

        // 우선순위가 가장 높은 위치로 한 칸 움직여줍니다.
        people[i] = make_pair(min_x, min_y);
    }

    // Step 2. 편의점에 도착한 사람에 한하여 
    //         앞으로 이동 불가능하다는 표시로 
    //         grid값을 2로 바꿔줍니다.
    for(int i = 0; i < m; i++) {
        if(people[i] == cvs_list[i]) {
            int px, py;
            tie(px, py) = people[i];
            grid[px][py] = 2;
        }
    }

    // Step 3. 현재 시간 curr_t에 대해 curr_t ≤ m를 만족한다면
    //         t번 사람이 베이스 캠프로 이동합니다.

    // curr_t가 m보다 크다면 패스합니다.
    if(curr_t > m)
        return;
    
    // Step 3-1. 편의점으로부터 가장 가까운 베이스 캠프를 고르기 위해
    //           편의점을 시작으로 하는 BFS를 진행합니다.
    BFS(cvs_list[curr_t - 1]);

    // Step 3-2. 편의점에서 가장 가까운 베이스 캠프를 선택합니다.
    //           i, j가 증가하는 순으로 돌리기 때문에
    //           가장 가까운 베이스 캠프가 여러 가지여도
    //           알아서 (행, 열) 우선순위대로 골라집니다.
    int min_dist = INT_MAX;
    int min_x = -1, min_y = -1;
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            // 방문 가능한 베이스 캠프 중
            // 거리가 가장 가까운 위치를 찾아줍니다.
            if(visited[i][j] && grid[i][j] == 1 && min_dist > step[i][j]) {
                min_dist = step[i][j];
                min_x = i; min_y = j;
            }
        }
    }
    
    // 우선순위가 가장 높은 베이스 캠프로 이동합니다.
    people[curr_t - 1] = make_pair(min_x, min_y);
    // 해당 베이스 캠프는 앞으로 이동이 불가능한 칸임을 표시합니다.
    grid[min_x][min_y] = 2;
}

// 전부 편의점에 도착헀는지를 확인합니다.
bool End() {
    // 단 한 사람이라도
    // 편의점에 도착하지 못했다면
    // 아직 끝나지 않은 것입니다.
    for(int i = 0; i < m; i++) {
        if(people[i] != cvs_list[i])
            return false;
    }

    // 전부 편의점에 도착했다면 끝입니다.
    return true;
}

int main() {
    // 입력:
    cin >> n >> m;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            cin >> grid[i][j];
    
    for(int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        cvs_list[i] = make_pair(x - 1, y - 1);
    }

    // 초기 사람들은 격자 밖에 있으므로
    // 위치를 EMPTY 상태로 놓습니다.
    for(int i = 0; i < m; i++)
        people[i] = EMPTY;

    // 1분에 한번씩 시뮬레이션을 진행합니다.
    while(true) {
        curr_t++;
        Simulate();
        // 전부 이동이 끝났다면 종료합니다.
        if(End()) break;
    }

    cout << curr_t;
    return 0;
}