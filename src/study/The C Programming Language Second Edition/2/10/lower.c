
#include <stdio.h>
#include <stdlib.h>

char lower(char x) {
	return((x >= 'A' && x <= 'Z') ? x + ' ' : x);
}

int main(int argc, char *argv[]) {
	char	x, result;

	if (argc == 2) {
		x = argv[1][0];
		result = lower(x);

		printf("x:\t%c\n", x);
		printf("result:\t%c\n", result);
	} else {
		return(1);
	}

	return(0);
}
