#include <stdio.h>
#include <stdlib.h>

int rightrot(int x, int n) {
    int nibble;

    /* Find the first n bits of x and move them to the LHS of the byte */
    nibble = (~(~0 << n) & x) << (sizeof(x) * 8 - n);

    return((x >> n) | nibble);
}

int main(int argc, char *argv[]) {
    int x, n, result;

    if (argc == 3) {
        x = atoi(argv[1]);
        n = atoi(argv[2]);
        result = rightrot(x, n);

        printf("x:\t%d\tn:\t%d\n", x, n);
        printf("result:\t%d\n", result);
    } else {
        return(1);
    }

    return(0);
}
