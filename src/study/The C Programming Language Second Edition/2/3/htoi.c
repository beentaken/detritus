/* $Id: htoi.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>

/*
	Write the function htoi(s) which converts a string of hexidecimal digits
	(including an optional 0x or 0X) in to its equivalent integer value.

	The allowable digits are 0 through 9, a through f and A through F.
*/

int htoi(char *s) {
	int i, n;

	n = 0;
	i = 0;

	if (s[0] != '\0' && s[1] != '\0') {
		if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
			i = 2;
		}
	}

	for (; s[i] != '\0'; i++) {
		if (s[i] >= '0' && s[i] <= '9') {
			n = n * 16 + (s[i] - '0');
		} else if (s[i] >= 'a' && s[i] <= 'f') {
			n = n * 16 + (s[i] - 'a' + 10);
		} else if (s[i] >= 'A' && s[i] <= 'F') {
			n = n * 16 + (s[i] - 'A' + 10);
		} else {
			break;
		}
	}

	return(n);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("string:\t\t%s\ninteger:\t%d\n", argv[1], htoi(argv[1]));
	} else {
		return(1);
	}

	return(0);
}
