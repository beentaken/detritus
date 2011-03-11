
#include <stdio.h>
#include "stack.h"

#define MAXVAL 100	/* Maximum depth of val stack */

int	sp = 0;		/* next free stack poisition */
double	val[MAXVAL];	/* value stack */

/* clear: clear the stack */
void clear(void) {
	sp = 0;
}

/* push: push f on to value stack */
void push(double f) {
	if (sp < MAXVAL) {
		val[sp++] = f;
	} else {
		printf("error: stack full, can't push %g\n", f);
	}
}

/* pop:	pop and return top value from the value stack */
double pop(void) {
	if (sp > 0) {
		return(val[--sp]);
	} else {
		printf("error: stack empty\n");
		return(0.0);
	}
}
