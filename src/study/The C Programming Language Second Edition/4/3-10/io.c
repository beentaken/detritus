/* $Id: io.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>
#include <ctype.h>
#include "io.h"

static int getch(void);
static void ungetch(int c);

/* getch: get next operator or numeric operand from stdin */
int getop(char s[]) {
	int	i, c;

	while ((s[0] = c = getch()) == ' ' || c == '\t') {
	}

	s[1] = '\0';

	if (!(isdigit(c) || c == '.' || c == '-' || c == '+')) {
		/* not a number */
		return(c);
	}

	i = 0;
	/* Handle the sign if there is one */
	if (c == '-' || c == '+') {
		s[i++] = c;
		if(! isdigit(s[i] = c = getch())) {
			/* This is an operator not an operand */
			ungetch(c);
			return(s[--i]);
		}
	}

	if (isdigit(c)) {
		/* collect integer part */
		while (isdigit(s[++i] = c = getch())) {
		}
	}

	if (c == '.') {
		/* collect fraction part */
		while (isdigit(s[++i] = c = getch())) {
		}
	}

	s[i] = '\0';
	if (c != EOF) {
		ungetch(c);
	}

	return(NUMBER);
}

#define BUFSIZE 100

char	buf[BUFSIZE];	/* buffer for ungetc */
int	bufp = 0;	/* next free position in buf */

int getch(void) {
	return((bufp > 0) ? buf[--bufp] : getchar());
}

void ungetch(int c) {
	if (bufp >= BUFSIZE) {
		printf("ungetch: buffer full\n");
	} else {
		buf[bufp++] = c;
	}
}
