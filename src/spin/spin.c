#define DEFAULT_SPINNER "|\\-/"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char        input;
    char        *spinner;
    char        *line;
    char        *end;

    unsigned short  num_spin_chars;

    /* Allow the specification of the spinning string on the cmd line */

    if (argc > 1) {
        spinner = strdup(argv[argc - 1]);
    } else {
        spinner = strdup(DEFAULT_SPINNER);
    }

    num_spin_chars = strlen(spinner) - 1;

    line = spinner;
    end = &spinner[num_spin_chars];

    input = '\0';

    putchar(' ');

    while((input = getchar()) != EOF) {
        if (input == '\n') {
            putchar('\b');
            putchar(*line);
            fflush(stdout);

            if (line == end) {
                line = spinner;
            } else {
                line++;
            }
        }
    }

    printf("\b");
    fflush(stdout);

    free(spinner);
    return 0;
}
