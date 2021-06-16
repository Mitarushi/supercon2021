#include "../main.cpp"

template<typename T>
void shuffle(std::vector<T> &a, int length) {
    for (int i = 0; i < length; i++) {
        int j = i + xor128() % (a.size() - i);
        std::swap(a[i], a[j]);
    }
}

std::vector<int> predict(std::vector<std::vector<char>> &visited, int prev_count, std::vector<char> &hash_skip) {
    std::vector<int> results(m, 0);
    const int boarder = 3000;

    if (prev_count < need_count / 3) {
        std::vector<std::pair<int, int>> sample_point;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (visited[i][j] == 1) {
                    sample_point.emplace_back(i, j);
                }
            }
        }

        shuffle(sample_point, std::min(boarder, (int) sample_point.size()));
        float scale = sample_point.size() > boarder ? float(sample_point.size()) / float(boarder) : 1.0f;

        for (int i = 0; i < m; i++) {
            if (hash_skip[i] == 1) {
                continue;
            }
            auto[dy, dx] = operation_shift[i];
            int s = 0;
            for (auto[y, x] : sample_point) {
                s += (edge_map[i][y][x] == 1) && (visited[y + dy][x + dx] == 0);
            }

            results[i] = int((float) s * scale);
        }
    } else if (prev_count > need_count * 5 / 6) {
        std::vector<std::pair<int, int>> sample_point;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (visited[i][j] == 0 && need_map[i][j] == 1) {
                    sample_point.emplace_back(i, j);
                }
            }
        }

        shuffle(sample_point, std::min(boarder, (int) sample_point.size()));
        float scale = sample_point.size() > boarder ? float(sample_point.size()) / float(boarder) : 1.0f;

        for (int i = 0; i < m; i++) {
            if (hash_skip[i] == 1) {
                continue;
            }
            auto[dy, dx] = operation_shift[i];
            int s = 0;
            for (auto[y, x] : sample_point) {
                s += edge_from_map[i][y][x] && visited[y - dy][x - dx];
            }

            results[i] = int((float) s * scale);
        }
    } else {
        for (int i = 0; i < m; i++) {
            if (hash_skip[i] == 1) {
                continue;
            }
            auto[dy, dx] = operation_shift[i];
            int count = 0;
            for (auto&[y, x]:op_edge_list[i]) {
                count += (visited[y][x] == 1) && (visited[y + dy][x + dx] == 0);
            }
            results[i] = count;
        }
    }

    return results;
}

using hash_t = unsigned int;
std::vector<hash_t> zobrist;

void init_hash() {
    zobrist.resize(m);
    for (int i = 0; i < m; i++) {
        zobrist[i] = xor128();
    }
}

hash_t vector_hash(std::vector<int> &use) {
    hash_t s = 0;
    for (const auto &i : use) {
        s ^= zobrist[i];
    }
    return s;
}

void first_greedy() {
    std::vector<int> use;
    std::vector<std::vector<char>> visited(n, std::vector(n, (char) 0));
    visited[0][0] = 1;

    std::vector<char> hash_skip(m, 0);

    int cnt = 0, visited_count = 1;
    while (true) {
        std::vector<int> results = std::move(predict(visited, visited_count, hash_skip));
        int max_i = int(std::max_element(results.begin(), results.end()) - results.begin());

        std::tie(visited_count, visited) = std::move(reach_count(visited, use, max_i));
        cnt++;
        use.push_back(max_i);
        if (visited_count == need_count) {
            break;
        }
    }

    printf("YES\n");
    printf("%d\n", cnt);
    line_print(use, 1);
}

int main() {
    read_graph();
    init_hash();

    if (!is_ok()) {
        printf("NO\n");
        return 0;
    }
    first_greedy();

    const int beam_width = 70;

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

        std::unordered_set<hash_t> beam_table;

        for (int b = 0; b < beam_use.size(); b++) {
            hash_t prev_hash = vector_hash(beam_use[b]);

            std::vector<char> hash_skip(m, 0);
            for (int i = 0; i < m; i++) {
                hash_t hash = prev_hash ^ zobrist[i];
                if (beam_table.count(hash) == 1) {
                    hash_skip[i] = 1;
                }
                beam_table.insert(hash);
            }

            std::vector<int> results = std::move(predict(beam_visited[b], beam_count[b], hash_skip));

            for (int i = 0; i < m; i++) {
                int result = results[i] + beam_count[b];
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

        printf("cnt: %d, count: %d, need: %d\n", cnt, *max_element, need_count);
        fflush(stdout);

        if (*max_element == need_count) {
            printf("YES\n");
            printf("%d\n", cnt);
            line_print(next_beam_use[max_element - next_beam_count.begin()], 1);
            fflush(stdout);
            break;
        }

        std::swap(beam_use, next_beam_use);
        std::swap(beam_visited, next_beam_visited);
        std::swap(beam_count, next_beam_count);
    }


}