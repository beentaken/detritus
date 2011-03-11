
#include <stdio.h>

#define TABSTOP	8

void tab_expand(int tab_len) {
	int i;

	for (i = 0; i < tab_len; i++) {
		putchar(' ');
	}

	return;
}

int main(void) {
	char	c;
	int	count;
	int	padding;

	count = 0;

	while((c = getchar()) != EOF) {
		if (c == '\t') {
			padding = TABSTOP - count % TABSTOP;
			tab_expand(padding);
			count += padding;
		} else  if (c == '\n') {
			count = 0;
			putchar('\n');
		} else {
			count++;
			putchar(c);
		}
	}

	return(0);
}
