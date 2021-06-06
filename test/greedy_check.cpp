#include "../main.cpp"

int main() {
    read_graph();

    std::vector<int> use;
    std::vector<std::vector<char>> visited(n, std::vector(n, (char) 0));
    visited[0][0] = 1;

    int cnt = 0;
    while (true) {
        int max_i, max = 0;
        std::vector<std::vector<char>> max_visited;
        for (int rnd = 0; rnd < 20; rnd++) {
            int i = int(xor128() % m);

            auto[result, visited_tmp] = std::move(reach_count(visited, use, i));

            if (result > max) {
                max_i = i;
                max = result;
                max_visited = std::move(visited_tmp);
            }
        }
        use.push_back(max_i);
        visited = std::move(max_visited);
        cnt++;
        printf("cnt: %d, idx: %d,  count: %d, need: %d\n", cnt, max_i, max, need_count);
        fflush(stdout);

        if (max == need_count) {
            break;
        }
    }
}