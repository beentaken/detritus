/* $Id: main.c 2 2007-07-19 13:00:48Z pdezwart $ */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"
#include "io.h"

#define MAXOP 100

int main(int argc, char *argv[]) {

	int	type;
	double	op1, op2;
	char	s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
			case NUMBER:
				push(atof(s));
				break;
			
			case '+':
				push(pop() + pop());
				break;

			case '*':
				push(pop() * pop());
				break;

			case '-':
				op2 = pop();
				push(pop() - op2);
				break;

			case '/':
				op2 = pop();
				if (op2 != 0.0) {
					push(pop() / op2);
				} else {
					printf("error: divisor is zero\n");
				}

				break;

			case '%':
				op2 = pop();
				push((int)pop() % (int)op2);
				break;

			case '=':
				printf("\t%.8g\n", pop());
				break;

			/* Print the top two stack elements */
			case 'p':
				op2 = pop();
				op1 = pop();

				printf("\t%.8g\t%.8g\n", op1, op2);
				push(op1);
				push(op2);
				break;
				
			/* Duplicate the top two stack elements */
			case 'd':
				op2 = pop();
				op1 = pop();

				push(op1);
				push(op2);
				push(op1);
				push(op2);
				break;

			/* Swap the top two stack elements */
			case 's':
				op2 = pop();
				op1 = pop();

				push(op2);
				push(op1);
				break;

			/* Clear the stack */
			case 'C':
				clear();
				break;

			/* NOOP */
			case '\n':
				break;

			/* Sin */
			case 'S':
				push(sin(pop()));
				break;

			/* Exp */
			case 'E':
				push(exp(pop()));
				break;

			/* Pow */
			case 'P':
				op2 = pop();

				push(pow(pop(), op2));
				break;

			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}

	return(0);
}
