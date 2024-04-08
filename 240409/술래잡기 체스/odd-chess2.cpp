#include <iostream>
#include <tuple>
#include <algorithm>

#define DIR_NUM 8
#define MAX_N 4
#define TAGGER make_pair(-2, -2)
#define BLANK make_pair(-1, -1)

using namespace std;

int n = 4;

pair<int, int> board[MAX_N][MAX_N];

// 문제에서 주어진 순서대로
// 방향을 정의합니다.
// ↑, ↖, ←, ↙, ↓, ↘, →, ↗ 
int dx[DIR_NUM] = {-1, -1,  0,  1, 1, 1, 0, -1};
int dy[DIR_NUM] = { 0, -1, -1, -1, 0, 1, 1,  1};

int max_score;

bool InRange(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < n;
}

// 도둑말이 이동할 수 있는 곳인지를 판단합니다.
// 격자 안이면서, 술래가 없어야 합니다.
bool ThiefCanGo(int x, int y) {
    return InRange(x, y) && board[x][y] != TAGGER;
}

// 술래가 이동할 수 있는 곳인지를 판단합니다.
// 격자 안이면서, 도둑말이 있어야만 합니다.
bool TaggerCanGo(int x, int y) {
    return InRange(x, y) && board[x][y] != BLANK;
}

bool Done(int x, int y, int d) {
    // 현재 위치에도 한 곳이라도 갈 수 있는지 확인합니다.
    // 존재한다면, 아직 게임은 끝나지 않았습니다.
    for(int dist = 1; dist <= n; dist++) {
        int nx = x + dx[d] * dist, ny = y + dy[d] * dist;
        if(TaggerCanGo(nx, ny))
            return false;
    }
    
    return true;
}

tuple<int, int, int> GetNext(int x, int y, int move_dir) {
    // 45'씩 8번 회전해보면서 최초로 이동 가능한 곳으로 움직입니다.
    for(int rotate_num = 0; rotate_num < 8; rotate_num++) {
        int adjusted_dir = (move_dir + rotate_num) % 8;
        int next_x = x + dx[adjusted_dir];
        int next_y = y + dy[adjusted_dir];
        if(ThiefCanGo(next_x, next_y))
            return make_tuple(next_x, next_y, adjusted_dir);
    }
    // 이동이 불가능하다면 현재 위치, 현재 방향 그대로 유지되어야합니다.
    return make_tuple(x, y, move_dir);
}

void Swap(int x, int y, int next_x, int next_y) {
    pair<int, int> temp_piece = board[x][y];
    board[x][y] = board[next_x][next_y];
    board[next_x][next_y] = temp_piece;
}

void Move(int target_num) {
    for(int x = 0; x < n; x++)
        for(int y = 0; y < n; y++) {
            int piece_num, move_dir;
            tie(piece_num, move_dir) = board[x][y];
            if(piece_num == target_num) {
                int next_x, next_y, next_dir;
                // 이동해야할 위치와 바라보게 될 방향을 구합니다.
                tie(next_x, next_y, next_dir) = GetNext(x, y, move_dir);
                // 현재 말의 방향을 바꿔준 뒤, 두 말의 위치를 교환합니다.
                board[x][y] = make_pair(piece_num, next_dir);
                Swap(x, y, next_x, next_y);
                return;
            }
        }
}

// 모든 도둑말들을 한번씩 움직입니다.
void MoveAll() {
    for(int i = 1; i <= n * n; i++)
        Move(i);
}

// 현재 술래말의 위치가 (x, y), 
// 바라보고 있는 방향이 d이고
// 지금까지 얻은 점수가 score일때
// 탐색을 계속 진행하는 함수입니다.
void SearchMaxScore(int x, int y, int d, int score) {
    // 더 이상 움직일 곳이 없다면
    // 답을 갱신하고 퇴각합니다.
    if(Done(x, y, d))  {
        max_score = max(max_score, score);
        return;
    }
    
    // 현재 턴에 움직일 수 있는 곳을 전부 탐색합니다.
    for(int dist = 1; dist <= n; dist++) {
        int nx = x + dx[d] * dist, ny = y + dy[d] * dist;
        // 술래가 이동 할 수 없는 위치라면, 패스합니다.
        if(!TaggerCanGo(nx, ny))
            continue;
        
        // 더 탐색을 진행한 이후, 초기 상태로 다시 만들기 위해
        // temp 배열에 현재 board 상태를 저장해놓습니다.
        pair<int, int> temp[MAX_N][MAX_N];
        
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                temp[i][j] = board[i][j];
        
        // 해당 위치의 도둑말을 잡고
        int extra_score, next_dir;
        tie(extra_score, next_dir) = board[nx][ny];
        board[nx][ny] = TAGGER;
        board[x][y] = BLANK;
        
        // 모든 도둑말을 움직입니다.
        MoveAll();
        
        // 그 다음 탐색을 진행합니다. 
        SearchMaxScore(nx, ny, next_dir, score + extra_score);
        
        // 퇴각시 다시 이전 board의 값을 넣어줍니다.
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                board[i][j] = temp[i][j];
    }
}

int main() {
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++) {
            int p, d;
            cin >> p >> d;
            board[i][j] = make_pair(p, d - 1);
        }
    
    // 처음 (0, 0) 도둑말을 잡고, 모든 도둑말이 이동한 다음에 시작합니다.
    int init_score, init_dir;
    tie(init_score, init_dir) = board[0][0]; 
    board[0][0] = TAGGER;
    
    MoveAll();
    
    SearchMaxScore(0, 0, init_dir, init_score);
    cout << max_score;
    
    return 0;
}