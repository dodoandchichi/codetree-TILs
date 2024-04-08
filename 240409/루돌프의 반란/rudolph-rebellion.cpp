#include <iostream>
#include <tuple>
using namespace std;
int n, m, p, c, d;
pair<int, int> rudolf;
pair<int, int> santa[31];
bool is_live[31];
int grid[51][51];
int points[31];
int stun[31];
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };

bool inRange(int x, int y)
{
    return 0 <= x && x < n && 0 <= y && y < n;
}

int main() {
    cin >> n >> m >> p >> c >> d;
    cin >> rudolf.first >> rudolf.second;
    rudolf.first--; rudolf.second--;
    grid[rudolf.first][rudolf.second] = -1;
    for (int i = 1; i <= p; i++)
    {
        int id;
        cin >> id;
        cin >> santa[id].first >> santa[id].second;
        santa[id].first--; santa[id].second--;
        is_live[id] = true;
        grid[santa[id].first][santa[id].second] = id;
    }
    
    for (int t = 1; t <= m; t++)
    {
        int minx = 10000; int miny = 10000; int closeIdx = 0;
        for (int i = 1; i <= p; i++)
        {
            if (!is_live[i]) continue;
            int cur_x = santa[i].first; int cur_y = santa[i].second;
            pair<int, pair<int, int>> min_dist = make_pair((((minx - rudolf.first) * (minx - rudolf.first)) + ((miny - rudolf.second) * (miny - rudolf.second))), make_pair(-minx, -miny));
            pair<int, pair<int, int>> cur_dist = make_pair((((cur_x - rudolf.first) * (cur_x - rudolf.first)) + ((cur_y - rudolf.second) * (cur_y - rudolf.second))), make_pair(-cur_x, -cur_y));
            if (cur_dist < min_dist)
            {
                minx = cur_x; miny = cur_y; closeIdx = i;
            }
        }
        
        if (closeIdx)
        {
            pair<int, int> prevRudolf = rudolf;
            int moveX = 0; int moveY = 0;
            if (rudolf.first > minx)
            {
                moveX = -1;
            }
            else if (rudolf.first < minx)
            {
                moveX = 1;
            }
            if (rudolf.second > miny)
                moveY = -1;
            else if (rudolf.second < miny) moveY = 1;
            rudolf.first += moveX; rudolf.second += moveY;
            grid[prevRudolf.first][prevRudolf.second] = 0;
            if (rudolf.first == minx && rudolf.second == miny)
            {
                int firstx = minx + moveX * c;
                int firsty = miny + moveY * c;
                int lastx = firstx;
                int lasty = firsty;
                stun[closeIdx] = t + 1;
                while (inRange(lastx, lasty) && grid[lastx][lasty] > 0)
                {
                    lastx += moveX; lasty += moveY;
                }

                while (!(lastx == firstx && lasty == firsty))
                {
                    int beforeX = lastx - moveX;
                    int beforeY = lasty - moveY;
                    if (!inRange(beforeX, beforeY)) break;
                    int idx = grid[beforeX][beforeY];
                    if (!inRange(lastx, lasty))
                    {
                        is_live[idx] = false;
                    }
                    else
                    {
                        grid[lastx][lasty] = grid[beforeX][beforeY];
                        santa[idx] = make_pair(lastx, lasty);
                    }

                    lastx = beforeX; lasty = beforeY;
                }
                points[closeIdx] += c;
                santa[closeIdx] = { firstx,firsty };
                if (inRange(firstx, firsty))
                {
                    grid[firstx][firsty] = closeIdx;
                }
                else
                {
                    is_live[closeIdx] = false;
                }
            }
        }
        grid[rudolf.first][rudolf.second] = -1;

        for (int i = 1; i <= p; i++)
        {
            if (!is_live[i] || stun[i] >= t) continue;
            int min_dist = (santa[i].first - rudolf.first) * (santa[i].first - rudolf.first) +
                (santa[i].second - rudolf.second) * (santa[i].second - rudolf.second);
            int min_dir = -1;
            for (int dir = 0; dir < 4; dir++)
            {
                int nx = santa[i].first + dx[dir];
                int ny = santa[i].second + dy[dir];
                if (!inRange(nx, ny) || grid[nx][ny] > 0) continue;
                int dist = (nx - rudolf.first) * (nx - rudolf.first) + (ny - rudolf.second) * (ny - rudolf.second);
                if (min_dist > dist)
                {
                    min_dist = dist;
                    min_dir = dir;
                }
            }

            int nx = santa[i].first + dx[min_dir]; int ny = santa[i].second + dy[min_dir];
            if (min_dir != -1)
            {
                if (nx == rudolf.first && ny == rudolf.second)
                {
                    stun[i] = t + 1;
                    int movex = -dx[min_dir];
                    int movey = -dy[min_dir];
                    int firstx = nx + d * movex;
                    int firsty = ny + d * movey;
                    int lastx = firstx; int lasty = firsty;
                    if (d == 1)
                    {
                        points[i] += d;
                    }
                    else
                    {
                        while (inRange(lastx, lasty) && grid[lastx][lasty] > 0)
                        {
                            lastx += movex;
                            lasty += movey;
                        }

                        while (!(lastx == firstx && lasty == firsty))
                        {
                            int beforex = lastx - movex;
                            int beforey = lasty - movey;
                            if (!inRange(beforex, beforey)) break;
                            int idx = grid[beforex][beforey];
                            if (!inRange(lastx, lasty))
                            {
                                is_live[idx] = false;
                            }
                            else
                            {
                                grid[lastx][lasty] = grid[beforex][beforey];
                                santa[idx] = { lastx, lasty };
                            }
                            lastx = beforex;
                            lasty = beforey;
                        }

                        points[i] += d;
                        grid[santa[i].first][santa[i].second] = 0;
                        santa[i].first = firstx; santa[i].second = firsty;
                        if (!inRange(firstx, firsty))
                        {
                            is_live[i] = false;
                        }
                        else grid[firstx][firsty] = i;
                    }

                }
                
            }
            else
            {
                grid[santa[i].first][santa[i].second] = 0;
                santa[i] = { nx, ny };
                grid[nx][ny] = i;
            }
        }
        for (int i = 1; i <= p; i++)
        {
            if (is_live[i]) points[i]++;
        }
    }
    for (int i = 1; i <= p; i++) cout << points[i] << " ";
    return 0;
}