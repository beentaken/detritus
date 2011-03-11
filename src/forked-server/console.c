
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "console.h"

static void
console_prompt(int out) {
	write(out, "> ", 2);
}

/* Simple wrapper function */
int
console(int fd) {
	return(console_io(fd, fd));
}

int
console_io(int in, int out) {
	int	buf_len = 1024;
	int	rtcode = 0;

	char	*buf = NULL;

	buf = calloc(buf_len, sizeof(char));

	if (buf == NULL) {
		perror("console_io:allocate buffer");
		rtcode = 1;
	} else {
		do {
			console_prompt(out);

			rtcode = read(in, buf, buf_len);

			if (rtcode > 1) {
				write(out, buf, rtcode);
			} else if (rtcode < 0) {
				perror("console_io:read");
				rtcode = 2;
			}
		} while (rtcode > 0);
	}

	free(buf);

	return(rtcode);
}
