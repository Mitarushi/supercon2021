// バグると嫌なので、bits/stdc++.h や using namespace std などは使わない方針で！
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <array>

int n;
std::vector<std::pair<int, int>> obstacle; // obstacle[i] := i番目の障害物の座礁(y,x)
std::vector<std::vector<char>> obstacle_map; // obstacle_map[y][x] := 座標(y,x)に障害物があるか
int m;
std::vector<std::pair<int, int>> operation_shift; // operation[i] := i番目の操作をした時、どれだけ動くか(y,x)
std::vector<std::vector<std::vector<char>>> edge_map; // edge_map[i][y][x] := 座標(y,x)で操作iが使えるか
std::vector<std::vector<std::pair<int, int>>> op_edge_list; // op_edge_list[i][j] := 操作iが使える座標
std::vector<std::vector<char>> need_map; // need_map[y][x] := 座標(y,x)に到達する必要があるか
int need_count; // 訪れる必要がある場所の数 探索の終了判定に

unsigned int xor128() {
    static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    unsigned int t;
    t = (x ^ (x << 11));
    x = y;
    y = z;
    z = w;
    return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}


void init_need_map() {
    need_map.resize(n, std::vector(n, (char) 0));
    need_count = 1;

    std::stack<std::pair<int, int>> stack;
    stack.emplace(0, 0);
    need_map[0][0] = 1;

    const std::array<int, 4>
            y_shift = {-1, 0, 1, 0},
            x_shift = {0, 1, 0, -1};

    while (!stack.empty()) {
        auto[y, x] = stack.top();
        stack.pop();

        for (int i = 0; i < 4; i++) {
            int ny = y + y_shift[i], nx = x + x_shift[i];
            if (0 <= ny && ny < n && 0 <= nx && nx < n && need_map[ny][nx] == 0 && obstacle_map[ny][nx] == 0) {
                need_map[ny][nx] = 1;
                stack.emplace(ny, nx);
                need_count++;
            }
        }
    }
}

void read_graph() {
    std::cin >> n;
    obstacle.clear();
    obstacle_map.resize(n, std::vector(n, (char) 0));

    for (int i = 0; i < n; i++) {
        int y, x;
        std::cin >> y >> x;
        y--;
        x--;
        obstacle.emplace_back(y, x);
        obstacle_map[y][x] = 1;
    }

    std::cin >> m;
    operation_shift.clear();
    edge_map.resize(m, std::vector(n, std::vector(n, (char) 1)));
    op_edge_list.resize(m);

    for (int i = 0; i < m; i++) {
        for (const auto&[py, px] : obstacle) {
            edge_map[i][py][px] = 0;
        }
    }

    for (int i = 0; i < m; i++) {
        std::string operation;
        std::cin >> operation;

        int y = 0, x = 0, y_max = 0, y_min = 0, x_max = 0, x_min = 0;
        for (const char &j : operation) {
            switch (j) {
                case 'U':
                    y--;
                    break;
                case 'D':
                    y++;
                    break;
                case 'L':
                    x--;
                    break;
                case 'R':
                    x++;
                    break;
                default:
                    throw std::invalid_argument("not in UDLR");
            }

            y_max = std::max(y, y_max);
            y_min = std::min(y, y_min);
            x_max = std::max(x, x_max);
            x_min = std::min(x, x_min);

            for (const auto&[py, px] : obstacle) {
                int qy = py - y, qx = px - x;
                if (0 <= qy && qy < n && 0 <= qx && qx < n) {
                    edge_map[i][qy][qx] = 0;
                }
            }
        }

        operation_shift.emplace_back(y, x);

        for (int py = 0; py < n; py++) {
            for (int px = 0; px < n; px++) {
                if (py + y_min < 0 || py + y_max >= n || px + x_min < 0 ||
                    px + x_max >= n) {
                    edge_map[i][py][px] = 0;
                }
            }
        }

        for (int py = 0; py < n; py++) {
            for (int px = 0; px < n; px++) {
                if (edge_map[i][py][px] == 1) {
                    op_edge_list[i].emplace_back(py, px);
                }
            }
        }
    }

    init_need_map();
}

std::vector<std::vector<std::vector<int>>> get_edge_list() {
    std::vector<std::vector<std::vector<int>>> edge_list(n, std::vector(n, std::vector<int>()));

    for (int i = 0; i < m; i++) {
        for (const auto &[py, px] : op_edge_list[i]) {
            edge_list[py][px].push_back(i);
        }
    }

    return edge_list;
}

std::vector<std::vector<std::vector<int>>> get_limited_edge_list(const std::vector<int> &use) {
    std::vector<std::vector<std::vector<int>>> edge_list(n, std::vector(n, std::vector<int>()));

    for (int i: use) {
        for (const auto &[py, px] : op_edge_list[i]) {
            edge_list[py][px].push_back(i);
        }
    }

    return edge_list;
}


// 全ての場所に到達可能か
bool is_ok() {
    std::stack<std::pair<int, int>> stack;
    std::vector<std::vector<char>> visited(n, std::vector(n, (char) 0));
    auto edge_list = std::move(get_edge_list());
    stack.emplace(0, 0);
    visited[0][0] = 1;
    int count = 1;

    while (!stack.empty()) {
        auto[y, x] = stack.top();
        stack.pop();

        for (int i : edge_list[y][x]) {
            int ny = y + operation_shift[i].first, nx = x + operation_shift[i].second;
            if (visited[ny][nx] == 0) {
                stack.emplace(ny, nx);
                visited[ny][nx] = 1;
                count++;
            }
        }
    }

    return count == need_count;
}

int reach_count(std::vector<int> &use) {
    std::stack<std::pair<int, int>> stack;
    std::vector<std::vector<char>> visited(n, std::vector(n, (char) 0));
    auto edge_list = std::move(get_limited_edge_list(use));
    stack.emplace(0, 0);
    visited[0][0] = 1;
    int count = 1;

    while (!stack.empty()) {
        auto[y, x] = stack.top();
        stack.pop();

        for (int i : edge_list[y][x]) {
            int ny = y + operation_shift[i].first, nx = x + operation_shift[i].second;
            if (visited[ny][nx] == 0) {
                stack.emplace(ny, nx);
                visited[ny][nx] = 1;
                count++;
            }
        }
    }

    return count;
}

