#include "../../main.cpp"

int main() {
    read_graph();

    int v = need_count;
    int e = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < m; k++) {
                if (need_map[i][j] == 1) {
                    e += edge_map[i][j][k];
                }
            }
        }
    }

    printf("v: %d, e: %d\n", v, e);
}