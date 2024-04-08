#include <iostream>
#include <algorithm>

#define MAX_N 20
#define NUM_MOVES 5
#define NONE -1

using namespace std;

int n;

int grid[MAX_N][MAX_N];
int next_grid[MAX_N][MAX_N];
int temp[MAX_N][MAX_N];

int move_dirs[NUM_MOVES];

int ans;

int GetMaxBlockNum() {
    int max_num = 0;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            max_num = max(max_num, grid[i][j]);
    
    return max_num;
}

// grid를 시계방향으로 90' 회전시킵니다.
void Rotate() {
    // next_grid를 0으로 초기화합니다.
    for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
            next_grid[i][j] = 0;
    
    // 90' 회전합니다.
    for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
			next_grid[i][j] = grid[n - j - 1][i];
    
    // next_grid를 grid에 옮겨줍니다.
     for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++)
            grid[i][j] = next_grid[i][j];
}

// 아래로 숫자들을 떨어뜨립니다.
void Drop() {
    // next_grid를 0으로 초기화합니다.
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            next_grid[i][j] = 0;
    
    // 아래 방향으로 떨어뜨립니다.
    for(int j = 0; j < n; j++) {
        // 같은 숫자끼리 단 한번만
        // 합치기 위해 떨어뜨리기 전에
        // 숫자 하나를 keep해줍니다.
        int keep_num = NONE, next_row = n - 1;
        for(int i = n - 1; i >= 0; i--) {
            if(!grid[i][j]) continue;
            
            // 아직 떨어진 숫자가 없다면, 갱신해줍니다.
            if(keep_num == NONE)
                keep_num = grid[i][j];
            // 가장 최근에 관찰한 숫자가 현재 숫자와 일치한다면
            // 하나로 합쳐주고, keep 값을 비워줍니다.
            else if(keep_num == grid[i][j]) {
                next_grid[next_row--][j] = keep_num * 2;
                keep_num = NONE;
            }
            // 가장 최근에 관찰한 숫자와 현재 숫자가 다르다면
            // 최근에 관찰한 숫자를 실제 떨어뜨려주고, keep 값을 갱신해줍니다.
            else {
                next_grid[next_row--][j] = keep_num;
                keep_num = grid[i][j];
            }
        }
        
        // 전부 다 진행했는데도 keep 값이 남아있다면
        // 실제로 한번 떨어뜨려줍니다.
        if(keep_num != NONE)
            next_grid[next_row--][j] = keep_num;
    }
    
    // next_grid를 grid에 옮겨줍니다.
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            grid[i][j] = next_grid[i][j];
}

// move_dir 방향으로 기울이는 것을 진행합니다.
// 회전을 규칙적으로 하기 위해
// 아래, 오른쪽, 위, 왼쪽 순으로 dx, dy 순서를 가져갑니다.
void Tilt(int move_dir) {
    // Step 1.
    // move_dir 횟수만큼 시계방향으로 90'회전하는 것을 반복하여
    // 항상 아래로만 숫자들을 떨어뜨리면 되게끔 합니다.
    for(int i = 0; i < move_dir; i++)
        Rotate();

    // Step 2.
    // 아래 방향으로 떨어뜨립니다.
    Drop();
    
    // Step 3.
    // 4 - move_dir 횟수만큼 시계방향으로 90'회전하는 것을 반복하여
    // 처음 상태로 돌아오게 합니다. (총 360' 회전)
    for(int i = 0; i < 4 - move_dir; i++)
        Rotate();
}

void Simulate() {
    // 시뮬레이션 전 상황을 저장해놓습니다.
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            temp[i][j] = grid[i][j];
    
    // 각 방향으로 기울이는 것을 진행합니다.
    for(int i = 0; i < NUM_MOVES; i++)
        Tilt(move_dirs[i]);
    
    // 답을 갱신합니다.
    ans = max(ans, GetMaxBlockNum());
    
    // grid를 시뮬레이션 전 상황으로 되돌립니다.
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            grid[i][j] = temp[i][j];
}

void SearchMaxNum(int cnt) {
    // 5번 이동할 방향을 정했다면
    // 직접 시뮬레이션을 진행합니다.
    if(cnt == NUM_MOVES)  {
        Simulate();
        return;
    }

    // 4 방향 중 이동할 방향을 선택합니다.
    for(int i = 0; i < 4; i++) {
        move_dirs[cnt] = i;
        SearchMaxNum(cnt + 1);
    }
}

int main() {
    cin >> n;
    
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            cin >> grid[i][j];
    
    SearchMaxNum(0);
    
    cout << ans;
    return 0;
}