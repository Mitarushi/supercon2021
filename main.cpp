// バグると嫌なので、bits/stdc++.h や using namespace std などは使わない方針で
#include <iostream>
#include <string>
#include <vector>

int n;
std::vector<std::pair<int, int>> obstacle; // obstacle[i] := i番目の障害物の座礁(y,x)
std::vector<std::vector<char>> obstacle_map; // obstacle_map[y][x] := 座標(y,x)に障害物があるか
int m;
std::vector<std::pair<int, int>> operation_shift; // operation[i] := i番目の操作をした時、どれだけ動くか(y,x)
std::vector<std::vector<std::vector<char>>> edge_map; // edge_map[y][x][i] := 座標(y,x)で操作iが使えるか


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
    edge_map.resize(n, std::vector(n, std::vector(m, (char) 1)));

    for (const auto&[py, px] : obstacle) {
        for (int i = 0; i < m; i++) {
            edge_map[py][px][i] = 0;
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
            y_min = std::min(x, x_min);
            x_max = std::max(x, x_max);
            x_min = std::min(x, x_min);

            for (const auto&[py, px] : obstacle) {
                int qy = py - y, qx = px - x;
                if (0 <= qy && qy < n && 0 <= qx && qx < n) {
                    edge_map[qy][qx][i] = 0;
                }
            }
        }

        operation_shift.emplace_back(y, x);

        for (int py = 0; py < n; py++) {
            for (int px = 0; px < n; px++) {
                if (py + y_min < 0 || py + y_max >= n || px + x_min < 0 ||
                    px + x_max >= n) {
                    edge_map[py][px][i] = 0;
                }
            }
        }
    }
}
