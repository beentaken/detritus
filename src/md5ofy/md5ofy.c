/* $Id: md5ofy.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>
#include <crypt.h>
#include <libgen.h>

int main(int argc, char *argv[]) {
	/* Use MD5 with crypt() */
	const char	salt[] = "$1$";

	char		*passwd;

	if (argc == 2) {
		passwd = crypt(argv[1], salt);

		printf("%s\n", passwd);
	} else {
		fprintf(stderr, "usage: %s \"string\"\n", basename(argv[0]));
	}

	return(0);
}
