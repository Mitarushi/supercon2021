#include <stdio.h>
#include <stdlib.h>
// 予選審査ではSEEDの値を別の「ある値」に変更する。
#define SEED 0x0000000000000123ull

#define N_MIN 2
#define N_MAX 250
#define M_MIN 1
#define M_MAX 2000

unsigned long long xor_shift() {
    static unsigned long long x = SEED;
    x = x ^ (x << 13);
    x = x ^ (x >> 7);
    x = x ^ (x << 17);
    return x;
}

// [0, x)
unsigned long long rnd(unsigned long long x) { return xor_shift() % x; }

// [l, r]
unsigned long long range_rnd(unsigned long long l, unsigned long long r) { return l + rnd(r - l + 1); }

void swap(int *x, int *y) {
    int t;
    t = *x;
    *x = *y;
    *y = t;
}

int main() {
    for (int t = 0; t < 100; t++) {
        char file_name[64];
        sprintf(file_name, "inputs/qual_random_%02d.in", t + 1);

        int N = range_rnd(N_MIN, N_MAX);

        int C = N * N - 1;
        int *ys = (int *) malloc(C * sizeof(int));
        int *xs = (int *) malloc(C * sizeof(int));
        {
            int i = 0;
            for (int y = 1; y <= N; y++) {
                for (int x = 1; x <= N; x++) {
                    if (y == 1 && x == 1) continue;
                    ys[i] = y;
                    xs[i] = x;
                    i++;
                }
            }
        }
        // The Fisher - Yates shuffle
        for (int i = 0; i < C - 1; i++) {
            int j = range_rnd(i, C - 1);
            swap(&ys[i], &ys[j]);
            swap(&xs[i], &xs[j]);
        }

        int M = range_rnd(M_MIN, M_MAX);
        int L = M * N;
        int *ls = (int *) calloc(M, sizeof(int));
        for (int i = 0; i < L; i++) {
            int pos = i < M ? i : range_rnd(0, M - 1);
            ls[pos]++;
        }
        char **ss = (char **) malloc(M * sizeof(char *));
        for (int i = 0; i < M; i++) {
            ss[i] = (char *) malloc((ls[i] + 1) * sizeof(char));
            int l = 0;
            while (l < ls[i]) {
                int r = range_rnd(0, 3);
                char c;
                if (r == 0) c = 'R';
                if (r == 1) c = 'D';
                if (r == 2) c = 'L';
                if (r == 3) c = 'U';
                int len = range_rnd(1, ls[i] - l);
                for (int j = l; j < l + len; j++) ss[i][j] = c;
                l = l + len;
            }
            ss[i][ls[i]] = '\0';
        }

        {
            FILE *fp = fopen(file_name, "w");
            if (fp == NULL) return 1;
            fprintf(fp, "%d\n", N);
            for (int i = 0; i < N; i++) fprintf(fp, "%d %d\n", ys[i], xs[i]);
            fprintf(fp, "%d\n", M);
            for (int i = 0; i < M; i++) fprintf(fp, "%s\n", ss[i]);
            fclose(fp);
        }

        free(ls);
        for (int i = 0; i < M; i++) free(ss[i]);
        free(ss);
        free(xs);
        free(ys);
    }
    return 0;
}