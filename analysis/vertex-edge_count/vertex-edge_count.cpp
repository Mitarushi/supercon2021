#include "../../main.cpp"

int main() {
    read_graph();

    int v = need_count;
    int e = 0;

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (need_map[j][k] == 1) {
                    e += edge_map[i][j][k];
                }
            }
        }
    }

    printf("v: %d, e: %d, e/v: %lf, e/m: %lf, v/(e/m): %lf\n", v, e, double(e) / v, double(e) / m, double(v) * m / e);
}