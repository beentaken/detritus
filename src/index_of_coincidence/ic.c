
#define ENGLISH	0.069

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "ic.h"

static Node *
alphebetize(char *cypher) {
	Node	*head = NULL;

	while(*cypher) {
		head = addLetter(*cypher, head);
		cypher++;
	}

	return (head);
}

double
rf(int count, int total) {
	double	rf = ((double)count * (100.0 / (double)total))/100.0;

	fprintf(stderr, "%e\t%d\t%d\n", rf, count, total);

	return(rf);
}

int
tf(Node *head) {
	int	count = 0;

	while (head) {
		count += head->count;
		head = head->next;
	}

	return(count);
}

double
ic(char *cypher) {
	Node	*head = alphebetize(cypher);
	Node	*temp = head->next;

	int	length = tf(head);

	double	index = 0.0;
	double	prev = rf(head->count, length);
	double	curr = 0.0;

	while (temp) {
		curr = rf(temp->count, length);
		index += curr * (rf(prev, length) - 1.0);
		prev = temp->count;
		temp = temp->next;
	}

	pruneNodes(head);
	return(index);
}
