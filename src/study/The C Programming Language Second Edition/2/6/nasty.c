#include <stdio.h>
#include <stdlib.h>

int setbits(int x, int y, int n, int p) {
    return(x & (~(~0 << n) & y << p));
}

int main(int argc, char *argv[]) {
    int x, y, n, p, result;

    if (argc == 5) {
        x = atoi(argv[1]);
        y = atoi(argv[2]);
        n = atoi(argv[3]);
        p = atoi(argv[4]);
        result = setbits(x, y, n, p);

        printf("x:\t%d\ty:\t%d\tn:\t%d\tp:\t%d\n", x, y, n, p);
        printf("result:\t%d\n", result);
    } else {
        return(1);
    }

    return(0);
}
