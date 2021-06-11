#include "../main.cpp"

int predict(int next_use, std::vector<std::vector<char>> &visited) {
    auto[dy, dx] = operation_shift[next_use];

    int count = 0;
    for (auto&[y, x]:op_edge_list[next_use]) {
        count += (visited[y][x] == 1) && (visited[y + dy][x + dx] == 0);
    }
    return count;
}

int main() {
    read_graph();

    if (!is_ok()) {
        printf("NO\n");
        return 0;
    }
    printf("YES\n");

    std::vector<int> use;
    std::vector<std::vector<char>> visited(n, std::vector(n, (char) 0));
    visited[0][0] = 1;

    int cnt = 0;
    while (true) {
        int max_i, max = 0;
        for (int j = 0; j < m; j++) {
            int i = j;
            int result = predict(i, visited);

            if (result > max) {
                max_i = i;
                max = result;
            }
        }
        
        std::tie(max, visited) = std::move(reach_count(visited, use, max_i));
        cnt++;
        use.push_back(max_i);
        // printf("cnt: %d, idx: %d,  count: %d, need: %d\n", cnt, max_i, max, need_count);
        // fflush(stdout);

        if (max == need_count) {
            break;
        }
    }

    printf("%d\n", cnt);
    line_print(use, 1);
}