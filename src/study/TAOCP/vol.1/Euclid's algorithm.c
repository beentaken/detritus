#include <stdio.h>

void swap(int *a, int *b) {
    *a ^= *b ^= *a ^= *b;
}

int main(int argc, char **argv) {
    int m, n;

    if (argc == 3) {
        m = atoi(argv[1]);
        n = atoi(argv[2]);

        printf("GCD of %d and %d is", m, n);

        printf(" %d.\n", GCD(m, n));
    }

    return (0);
}

int GCD(int m, int n) {
    int r;

    if (n < m) {
        swap(&m, &n);
    }

    while (r = m % n) {
        m = n;
        n = r;
    }

    return(n);
}
