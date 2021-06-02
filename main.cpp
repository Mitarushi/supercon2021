// バグると嫌なので、bits/stdc++.h や using namespace std などは使わない方針で
#include <vector>
#include <iostream>
#include <string>

struct graph {
    int n;
    std::vector<std::vector<int>> obstacle;
    int m;

    void read_graph() {
        std::cin >> n;
        obstacle.resize(n, std::vector<int>(n, 0));

        for (int i = 0; i < n; i++) {
            int y, x;
            std::cin >> y >> x;
            obstacle[y - 1][x - 1] = 1;
        }

        std::cin >> m;

        for (int i = 0; i < m; i++) {
            std::string move;
            std::cin >> move;
        }

    }

};