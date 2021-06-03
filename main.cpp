// バグると嫌なので、bits/stdc++.h や using namespace std などは使わない方針で
#include <iostream>
#include <string>
#include <vector>

int n;
std::vector<std::pair<int, int>> obstacle;
std::vector<std::vector<char>> obstacle_map;
int m;
std::vector<std::vector<std::vector<char>>> edge_map;

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
    edge_map.resize(n, std::vector(n, std::vector(m, (char) 1)));

    for (const auto&[py, px] : obstacle) {
        for (int i = 0; i < m; i++) {
            edge_map[py][px][i] = 0;
        }
    }

    for (int i = 0; i < m; i++) {
        std::string move;
        std::cin >> move;

        int y = 0, x = 0, ymax = 0, ymin = 0, xmax = 0, xmin = 0;
        for (const char &j : move) {
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
                    throw std::invalid_argument("not int UDLR");
            }

            ymax = std::max(y, ymax);
            ymin = std::min(x, xmin);
            xmax = std::max(x, xmax);
            xmin = std::min(x, xmin);

            for (const auto&[py, px] : obstacle) {
                int qy = py - y, qx = px - x;
                if (0 <= qy && qy < n && 0 <= qx && qx < n) {
                    edge_map[qy][qx][i] = 0;
                }
            }
        }

        for (int py = 0; py < n; py++) {
            for (int px = 0; px < n; px++) {
                if (py + ymin < 0 || py + ymax >= n || px + xmin < 0 ||
                    px + xmax >= n) {
                    edge_map[py][px][i] = 0;
                }
            }
        }
    }
}
