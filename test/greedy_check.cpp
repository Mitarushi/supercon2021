#include "../main.cpp"

int main() {
    read_graph();

    std::vector<int> use;
    while (true) {
        int max_i, max = 0;
        for (int rnd = 0; rnd < 100; rnd++) {
            int i = int(xor128() % m);

            use.push_back(i);
            int result = reach_count(use);
            use.pop_back();

            if (result > max) {
                max_i = i;
                max = result;
            }
        }
        use.push_back(max_i);
        printf("idx: %d,  count: %d, need: %d\n", max_i, max, need_count);
        fflush(stdout);

        if (max == need_count) {
            break;
        }
    }
}