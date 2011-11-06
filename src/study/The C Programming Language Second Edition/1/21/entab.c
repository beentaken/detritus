#include <stdio.h>

#define TABSTOP 8

void print_spaces(int number) {
    int i;

    for (i = 0; i < number; i++) {
        putchar(' ');
    }

    return;
}

int main(void) {
    char    c;
    int space_count;

    space_count = 0;

    while ((c = getchar()) != EOF) {
        switch (c) {
            case ' ':   if (space_count == TABSTOP) {
                        putchar('\t');
                        space_count = 0;
                    } else {
                        space_count++;
                    }
                    break;

            default:    if (space_count) {
                        print_spaces(space_count);
                        space_count = 0;
                    }
                    putchar(c);
                    break;
        }

    }

    return(0);
}
