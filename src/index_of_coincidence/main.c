#include <stdio.h>
#include "ic.h"

int
main(int argc, char **argv) {
    while (argc > 1) {
        printf("%e\n", ic(argv[--argc]));
    }

    return(0);
}
