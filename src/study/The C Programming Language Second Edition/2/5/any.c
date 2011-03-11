/* $Id: any.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>
#include <stdlib.h>

int any(char *s, char *search) {
	int i, j;

	int	rtcode;

	rtcode = -1;

	for (i = j = 0; s[i] != '\0' && rtcode == -1; i++) {
		int count;

		for (count = 0; search[count] != '\0'; count++) {
			if (s[i] == search[count]) {
				rtcode = i;
				break;
			}
		}
	}

	return(rtcode);
}

int main(int argc, char *argv[]) {
	if (argc == 3) {
		printf("String:\t%s\tSearch:\t%s\n", argv[1], argv[2]);
		printf("Index:\t%d\n", any(argv[1], argv[2]));
	} else {
		return(1);
	}

	return(0);
}
