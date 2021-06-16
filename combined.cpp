// バグると嫌なので、bits/stdc++.h や using namespace std などは使わない方針で！
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <array>
#include <algorithm>
#include <tuple>
#include <unordered_set>
#include <sys/time.h>
#include <bitset>

constexpr int n_max = 250;
using board_bit = std::bitset<n_max * n_max>;

int n;
std::vector<std::pair<int, int>> obstacle; // obstacle[i] := i番目の障害物の座礁(y,x)
board_bit obstacle_map; // obstacle_map[y][x] := 座標(y,x)に障害物があるか
int m;
std::vector<std::pair<int, int>> operation_shift; // operation[i] := i番目の操作をした時、どれだけ動くか(y,x)
std::vector<board_bit> edge_map; // edge_map[i][y][x] := 座標(y,x)で操作iが使えるか
std::vector<board_bit> edge_from_map; // edge_map[i][y][x] := 座標(y,x)に操作iで行けるか
std::vector<std::vector<std::pair<int, int>>> op_edge_list; // op_edge_list[i][j] := 操作iが使える座標
board_bit need_map; // need_map[y][x] := 座標(y,x)に到達する必要があるか
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
    need_count = 1;

    std::stack<std::pair<int, int>> stack;
    stack.emplace(0, 0);
    need_map.set(0);

    const std::array<int, 4>
            y_shift = {-1, 0, 1, 0},
            x_shift = {0, 1, 0, -1};

    while (!stack.empty()) {
        auto[y, x] = stack.top();
        stack.pop();

        for (int i = 0; i < 4; i++) {
            int ny = y + y_shift[i], nx = x + x_shift[i];
            if (0 <= ny && ny < n && 0 <= nx && nx < n && !need_map[ny * n_max + nx] &&
                !obstacle_map[ny * n_max + nx]) {
                need_map.set(ny * n_max + nx);
                stack.emplace(ny, nx);
                need_count++;
            }
        }
    }
}

template<typename T>
void line_print(std::vector<T> &args, T shift = 0) {
    for (int i = 0; i < args.size(); i++) {
        if (i) {
            printf(" ");
        }
        printf("%d", args[i] + shift);
    }
    printf("\n");
}

void read_graph() {
    std::cin >> n;
    obstacle.clear();

    for (int i = 0; i < n; i++) {
        int y, x;
        std::cin >> y >> x;
        y--;
        x--;
        obstacle.emplace_back(y, x);
        obstacle_map.set(y * n_max + x);
    }

    std::cin >> m;
    operation_shift.clear();
    edge_map.resize(m);
    for (int i = 0; i < m; i++) {
        edge_map[i].flip();
    }
    op_edge_list.resize(m);

    for (int i = 0; i < m; i++) {
        for (const auto&[py, px] : obstacle) {
            edge_map[i].reset(py * n_max + px);
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
                    edge_map[i].reset(qy * n_max + qx);
                }
            }
        }

        operation_shift.emplace_back(y, x);

        for (int py = 0; py < n; py++) {
            for (int px = 0; px < n; px++) {
                if (py + y_min < 0 || py + y_max >= n || px + x_min < 0 ||
                    px + x_max >= n) {
                    edge_map[i].reset(py * n_max + px);
                }
            }
        }

        for (int py = 0; py < n; py++) {
            for (int px = 0; px < n; px++) {
                if (edge_map[i][py * n_max + px]) {
                    op_edge_list[i].emplace_back(py, px);
                }
            }
        }
    }

    init_need_map();

    edge_from_map.resize(m);
    for (int i = 0; i < m; i++) {
        auto[dy, dx] = operation_shift[i];
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (edge_map[i][j * n_max + k]) {
                    edge_from_map[i].set((j + dy) * n_max + k + dx);
                }
            }
        }
    }
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
    board_bit visited;
    auto edge_list = std::move(get_edge_list());
    stack.emplace(0, 0);
    visited.set(0);
    int count = 1;

    while (!stack.empty()) {
        auto[y, x] = stack.top();
        stack.pop();

        for (int i : edge_list[y][x]) {
            int ny = y + operation_shift[i].first, nx = x + operation_shift[i].second;
            if (!visited[ny * n_max + nx]) {
                stack.emplace(ny, nx);
                visited.set(ny * n_max + nx);
                count++;
            }
        }
    }

    return count == need_count;
}

std::tuple<int, board_bit>
reach_count(board_bit &prev_visited, std::vector<int> &use, int next_use) {
    std::stack<std::pair<int, int>> stack;
    board_bit visited = prev_visited;
    int count = 0;

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (edge_map[next_use][y * n_max + x]) {
                if (prev_visited[y * n_max + x] == 1) {
                    stack.emplace(y, x);
                }
            }

            count += prev_visited[y * n_max + x];
        }
    }
    use.push_back(next_use);

    while (!stack.empty()) {
        auto[y, x] = stack.top();
        stack.pop();

        for (int i : use) {
            if (!edge_map[i][y * n_max + x]) {
                continue;
            }

            int ny = y + operation_shift[i].first, nx = x + operation_shift[i].second;
            if (!visited[ny * n_max + nx]) {
                stack.emplace(ny, nx);
                visited.set(ny * n_max + nx);
                count++;
            }
        }
    }
    use.pop_back();

    return std::forward_as_tuple(count, visited);
}

template<typename T>
void shuffle(std::vector<T> &a, int length) {
    for (int i = 0; i < length; i++) {
        int j = i + xor128() % (a.size() - i);
        std::swap(a[i], a[j]);
    }
}

#include <cassert>

std::vector<int> predict(board_bit &visited, int prev_count, std::vector<char> &hash_skip) {
    std::vector<int> results(m, 0);
    const int boarder = 3000;

    if (prev_count < need_count / 4) {
        std::vector<std::pair<int, int>> sample_point;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (visited[i * n_max + j]) {
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
                s += edge_map[i][y * n_max + x] && !visited[(y + dy) * n_max + x + dx];
            }

            results[i] = int((float) s * scale);
        }
    } else if (prev_count > need_count * 19 / 20) {
        std::vector<std::pair<int, int>> sample_point;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (!visited[i * n_max + j] && need_map[i * n_max + j]) {
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
                s += edge_from_map[i][y * n_max + x] && visited[(y - dy) * n_max + x - dx];
            }

            results[i] = int((float) s * scale);
        }
    } else {
        board_bit not_visited = ~visited;
        for (int i = 0; i < m; i++) {
            if (hash_skip[i] == 1) {
                continue;
            }
            auto[dy, dx] = operation_shift[i];
            int shift = dy * n_max + dx;
            board_bit mask = (edge_map[i] & visited);
            if (shift > 0) {
                mask <<= shift;
            } else {
                mask >>= -shift;
            }
            mask &= not_visited;
            results[i] = int(mask.count());
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
    board_bit visited;
    visited.set(0);

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

double get_elapsed_time(struct timeval *begin, struct timeval *end) {
    return (end->tv_sec - begin->tv_sec) * 1000 +
           (end->tv_usec - begin->tv_usec) / 1000.0;
}

int main() {
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    read_graph();
    init_hash();

    if (!is_ok()) {
        printf("NO\n");
        return 0;
    }
    first_greedy();

    int beam_width = 128;

    std::vector<std::vector<int>> beam_use = {{}}, next_beam_use;
    std::vector<board_bit> beam_visited = {board_bit()},
            next_beam_visited;
    std::vector<int> beam_count = {1}, next_beam_count;
    beam_visited[0].set(0);

    int cnt = 0;
    while (true) {
        std::vector<std::tuple<int, int, int>> next;
        next_beam_use.clear();
        next_beam_visited.clear();
        next_beam_count.clear();

//        if (beam_count[0] < need_count / 3) {
//            printf("a\n");
//        } else if (beam_count[0] > need_count * 19 / 20) {
//            printf("c\n");
//        } else {
//            printf("b\n");
//        }

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

//        printf("cnt: %d, count: %d, need: %d\n", cnt, *max_element, need_count);
//        fflush(stdout);

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

        gettimeofday(&t2, NULL);
        double time = get_elapsed_time(&t1, &t2);
        if (time > 9500.0) {
            beam_width = 10;
        }
    }
}