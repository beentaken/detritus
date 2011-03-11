/* $Id: ass4.c 2 2007-07-19 13:00:48Z pdezwart $ */

/*
	Written by:	Peter de Zwart.
	Login:		pntdz31		(don't ask)
	Student #:	9840642
	Subject:	CSCI203
	Assignment:	3
*/

/* NOTE: Any negative value of an edge denotes that this edge is not in the spanning tree */
#define NOT_IN_SPANNING_TREE	-1	

#define MIN(x,y)	((x) < (y) ? (x) : (y))
#define ABS(x)		(((x) <  0) ? (x) * -1 : (x))

#include "ass4.h"
#include <stdio.h>
#include <stdlib.h>

/*
	Error checking has been omitted to reduce the verbage and ominous qualities of my code.
	Unfortunatly, because of the complexity of the code, there are still a lot of comments.
*/

int	numNodes;
int	***Graph;
int	root_child_count;

int main(int argc, char **argv) {
	int	i, j;
	int	node_1, node_2;

	root_child_count = 0;

	/* Get the number of nodes */
	scanf("%i", &numNodes);

	/* Create the matrix that will hold the Graph */
	Graph = (int ***)calloc(numNodes, sizeof(int *));

	for (i = 0; i < numNodes; i++) {
		Graph[i] = (int **)calloc(numNodes, sizeof(int));
	}

	/* Get the list of edges and put them in the matrix */
	while (scanf("%i %i", &node_1, &node_2) && (node_1 || node_2)) {
		node_1--;
		node_2--;

		/* Any non NULL value denotes a edge between node_1 & node_2 */
		Graph[node_1][node_2] = (int *)malloc(sizeof(int));
		*(Graph[node_1][node_2]) = NOT_IN_SPANNING_TREE;
		Graph[node_2][node_1] = Graph[node_1][node_2];
	}

	/* The identity matrix Graph[x][x] is synonymous for prenum[x] in the assignment spec */
	/* If prenum[x] is negative, then node x is an articulation point */
	/* Graph[x][0] holds the higest[x] array */

	DepthFirstTraversal(0);
		
	/* Output */

	/* Handle the root node */
	printf("1\t1\t1\t%c\n", (root_child_count > 1) ? '*' : '\0');

	/* Do the rest */
	for (i = 1; i < numNodes; i++) {
		printf("%d\t%d\t%d\t%c\n", i + 1, ABS(*(Graph[i][i])) + 1, *(Graph[i][0]) + 1, (*(Graph[i][i]) < 0) ? '*' : '\0');
	}
		
	/* Ok, lets clean up after ourselves */
	for (i = 0; i < numNodes; i++) {
		for (j = 0; j < numNodes; j++) {
			if (Graph[i][j]) {
				free (Graph[i][j]);

				if (j > i) {
					Graph[j][i] = NULL;
				}
			}
		}

		free(Graph[i]);
	}

	free(Graph);

	/* Mmmm, implicit success, ahhhhhhh */
	return(0);
}

int
DepthFirstTraversal(int node) {
	static int	prenum_count = 0;
	int		*prenum, *child_prenum, *edge;
	int		i, higest, child_higest, sign;
	int		articulate = 0;
	
	/* Show that this node has been visited */
	prenum = Graph[node][node] = (int *)malloc(sizeof(int));

	/* Set the nodes sequence number in the spanning tree */
	*prenum = higest = prenum_count++;

	for (i = 0; i < numNodes; i++) {
		if (i != node) {
			edge = Graph[node][i];

			/* Check for an edge, node->i */
			if (edge) {
				child_prenum = Graph[i][i];

				/* Check to see if this child node has not been visited */
				if (!child_prenum) {
					/* cheap and nasty hack to calculate if the root node is an articulation point */

					if (!node) {
						root_child_count++;
					}

					/* Denote that this edge was visted by this node by stamping it with our node sequence number */
					*edge = *prenum;

					/* DepthFirstTraversal will return the calculated "higest" value for this child node */
					child_higest = DepthFirstTraversal(i);

					/* Since the root node is a special case, the "higest" value can be kept in the edge field for i->0. */

					sign = 1;

					/* Handle the case where there is no edge between i and the root node */
					if (!Graph[i][0]) {
						Graph[i][0] = (int *)malloc(sizeof(int));
					} else if (*(Graph[i][0]) < 0) {
						 /* If there is an edge between i and the root node and it is not a part of the spanning tree, then we have to make the
							resulting edge value negative. */
						 sign = -1;
					}

					/* If there is an edge between i and the root node, but it is in the spanning tree, we don't have to worry, the way that the
						root node calcuates if it is an articulation point is done on if it has two or more child nodes */

					*(Graph[i][0]) = child_higest * sign;

				} else {
					/* See if the edge to the node i is not in the spanning tree and has a lower "prenum" value */
					if (*edge < 0) {
						higest = MIN(higest, ABS(*child_prenum));
					}
				}

				/* Is i a child of node, see footnote 0xdeadbeef */
				if (*edge == *prenum) {
					/* See if this child's "highest" value is smaller than the current nodes "higest" value */
					higest = MIN(higest, child_higest);

					/* Check to see if we have an articulation point, note: It has to be done only once. */
					if (!articulate && (child_higest >= *prenum)) {
						/* Yes we do! Denote this by inverting the polarity of the prenum */
						articulate = 1;
					}
				}
			}
		}
	}

	if (articulate) {
		*prenum *= -1;
	}

	return(higest);
}

/*
	Footnote: 0xdeadbeef

	You may be thinking that there is a wonderful situation where if the current node is the child node of the root node and the resulting edge is a part of the
	spanning tree, the evaluation (*edge == *prenum) is TRUE, since edge == Graph[node][0]. Meaning that the current node will test to see if root has a lower
	"higest" value, which it would.

	This will not happen since the setting of the current node's "higest" value in Graph[node][0] is performed after this invocation of DepthFirstTraversal(foo).

	Meaning that ((*edge == 0) && (*edge != *prenum)

*/
