/* $Id: printbits.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int	i;
	int	numbits;

	numbits = sizeof(argv[0][0]) * 8;

	/* Set i = 1 to skip the name of the executable */
	for (i = 1; i < argc; i++) {
		int	len;
		int	j;

		len = strlen(argv[i]);

		for (j = 0; j < len; j++) {
			int	k;

			printf("argv[%d][%d] = %c = ", i, j, argv[i][j]);

			for (k = numbits; k != 0; k--) {
				printf("%d", argv[i][j] >> k & 1);
			}

			putchar('\n');
		}
	}

		
	return(!(argc > 1));
}
