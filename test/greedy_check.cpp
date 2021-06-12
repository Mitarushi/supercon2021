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

    const int beam_width = 30;

    std::vector<std::vector<int>> beam_use = {{}}, next_beam_use;
    std::vector<std::vector<std::vector<char>>> beam_visited = {std::vector(n, std::vector(n, (char) 0))},
            next_beam_visited;
    std::vector<int> beam_count = {1}, next_beam_count;
    beam_visited[0][0][0] = 1;

    int cnt = 0;
    while (true) {
        std::vector<std::tuple<int, int, int>> next;
        next_beam_use.clear();
        next_beam_visited.clear();
        next_beam_count.clear();

        for (int b = 0; b < beam_use.size(); b++) {
            for (int i = 0; i < m; i++) {
                int result = predict(i, beam_visited[b]) + beam_count[b];

                next.emplace_back(result, b, i);
            }
        }

        std::sort(next.begin(), next.end(), std::greater<>());
        cnt++;

        int next_beam_size = std::min(beam_width, (int) next.size());
        next_beam_use.resize(next_beam_size);
        next_beam_visited.resize(next_beam_size);
        next_beam_count.resize(next_beam_size);

        for (int b = 0; b < next_beam_size; b++) {
            auto[_, i, j] = next[b];

            std::tie(next_beam_count[b], next_beam_visited[b]) =
                    std::move(reach_count(beam_visited[i], beam_use[i], j));

            next_beam_use[b] = beam_use[i];
            next_beam_use[b].push_back(j);
        }

        auto max_element = std::max_element(next_beam_count.begin(), next_beam_count.end());

//        printf("cnt: %d, count: %d, need: %d\n", cnt, *max_element, need_count);
//        fflush(stdout);

        if (*max_element == need_count) {
            printf("%d\n", cnt);
            line_print(next_beam_use[max_element - next_beam_count.begin()], 1);
            break;
        }

        std::swap(beam_use, next_beam_use);
        std::swap(beam_visited, next_beam_visited);
        std::swap(beam_count, next_beam_count);
    }


}