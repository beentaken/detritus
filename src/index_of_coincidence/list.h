/* $Id: list.h 2 2007-07-19 13:00:48Z pdezwart $ */

#ifndef __LIST_H
#define __LIST_H

struct _Node {
        char    letter;
	int	count;
        struct _Node    *next;
} typedef Node;

Node *
addLetter(char letter, Node *head);

int
listLength(Node *head);

void
pruneNodes(Node *head);

#endif
