#include <iostream>
#include <cmath>

#define MAX_N 4
#define MAX_M 8
#define ROTATE_CW 1
#define ROTATE_CCW -1
#define NOT_ROTATE 0

using namespace std;

// 전역 변수 선언:
int n = 4, m = 8, k;

char a[MAX_N + 1][MAX_M + 1];

// 의자별로 회전해야 할 방향을 저장합니다.
int rotate_dir[MAX_N + 1];

// 의자를 해당 방향으로 밀어줍니다.
void Shift(int curr_num, int curr_dir) {
    // 시계방향으로 회전해야 하는 경우
    if(curr_dir == ROTATE_CW) {
        int temp = a[curr_num][m];
        for(int i = m; i >= 2; i--)
            a[curr_num][i] = a[curr_num][i - 1];
        a[curr_num][1] = temp;
    }
    // 반시계방향으로 회전해야 하는경우
    else {
        int temp = a[curr_num][1];
        for(int i = 1; i <= m - 1; i++)
            a[curr_num][i] = a[curr_num][i + 1];
        a[curr_num][m] = temp;
    }
}

// 주어진 방향으로부터 반대 방향의 값을 반환합니다.
int Flip(int curr_dir) {
    return (curr_dir == ROTATE_CW) ? ROTATE_CCW : ROTATE_CW;
}

// 조건에 맞춰 움직여봅니다.
void Simulate(int start_num, int start_dir) {
    // Step1
    // 회전하게 되는 의자가 도는 방향이 동시에 결정되므로
    // 먼저 각 의자마다 회전하게 될 방향을 구합니다. 
    
    // 초기값은 전부 회전하지 않음으로 표시합니다.
    for(int i = 1; i <= n; i++)
        rotate_dir[i] = NOT_ROTATE;
    
    // 시작 위치는 반드시 회전해야 합니다.
    rotate_dir[start_num] = start_dir;
    
    // 좌측에 있는 의자들의 회전 방향을 정합니다.
    // 마주보는 두 사람의 지역이 다를때만 반복하며,
    // 그렇지 않은 경우에는 종료합니다.
    // 방향을 계속 뒤집어줘야 함에 유의합니다.
    for(int i = start_num - 1; i >= 1; i--) {
        if(a[i][3] != a[i + 1][7])
            rotate_dir[i] = Flip(rotate_dir[i + 1]);
        else
            break;
    }
    
    // 우측에 있는 의자들의 회전 방향을 정합니다.
    // 마주보는 두 사람의 지역이 다를때만 반복하며,
    // 그렇지 않은 경우에는 종료합니다.
    // 방향을 계속 뒤집어줘야 함에 유의합니다.
    for(int i = start_num + 1; i <= n; i++) {
        if(a[i][7] != a[i - 1][3])
            rotate_dir[i] = Flip(rotate_dir[i - 1]);
        else
            break;
    }
    
    // Step2
    // step 1에 의해 회전이 결정된 의자들을 회전시켜줍니다.
    for(int i = 1; i <= n; i++)
        if(rotate_dir[i] != NOT_ROTATE)
            Shift(i, rotate_dir[i]);
}

int main() {
    // 입력:
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= m; j++)
            cin >> a[i][j];
    cin >> k;
    
    while(k--) {
        int start_num, start_dir;
        cin >> start_num >> start_dir;

        // 조건에 맞춰 돌려봅니다.
        Simulate(start_num, start_dir);
    }

    int ans = 0;
    
    for(int i = 1; i <= n; i++)
        if(a[i][1] == '1')
            ans += pow(2, i - 1);
    
    // 출력:
    cout << ans;
    
    return 0;
}