/* $Id: ass3.c 2 2007-07-19 13:00:48Z pdezwart $ */

/*
	Written by:	Peter de Zwart.
	Student #:	9840642
	Subject:	CSCI203
	Assignment:	3
*/

#include "ass3.h"
#include <stdio.h>
#include <stdlib.h>

/* Error checking has been omitted to reduce the verbage and ominous qualities
	of my code. */

int	num_nodes;
int	**lengths;

int main(int argc, char **argv) {
	int	i, j;
	int	node_1, node_2, cost;
	int	source, destination;

	/* Get the number of nodes */
	scanf("%i", &num_nodes);

	/* Create the matrix that will hold the lengths of the edeges */
	lengths = (int **)calloc(num_nodes, sizeof(int *));

	for (i = 0; i < num_nodes; i++) {
		lengths[i] = (int *)calloc(num_nodes, sizeof(int));
		for (j = 0; j < num_nodes; j++) {
			lengths[i][j] = -1;
		}
	}

	/* Get the list of edges and put them in the matrix */
	while (scanf("%i %i %i", &node_1, &node_2, &cost) && (node_1 || node_2
		|| cost)) {
		node_1--;
		node_2--;
		lengths[node_1][node_2] = cost;
		lengths[node_2][node_1] = cost;
	}
		
	/* Get the source and destination nodes */
	scanf("%i %i", &source, &destination);
	source--;
	destination--;

	mod_Dijkstra(source, destination);
	imp_Dijkstra(source, destination);

	/* Ok, lets clean up after ourselves */
	for (i = 0; i < num_nodes; i++) {
		free(lengths[i]);
	}

	free(lengths);

	/* Mmmm, implicit success, ahhhhhhh */
	return(0);
}

data_set *
init_Dijkstra(int src) {
	data_set	*data = (data_set *)malloc(sizeof(data_set));
	int		i;

	/* Create the distances vector */
	data->dist = (int *)calloc(num_nodes, sizeof(int));

	/* Create the paths array */
	data->paths = (int *)calloc(num_nodes, sizeof(int));

	/* Create the nodes array */
	data->nodes = (int *)calloc(num_nodes, sizeof(int));

	/* Initalize the nodes array */
	for (i = 0; i < num_nodes; i++) {
		if (i == src) {
			data->nodes[i] = 0;	/* nodes[i] E of the set S */
		} else {
			data->nodes[i] = 1;	/* nodes[i] E of the set C */
		}
	}

	/* Initalize the paths array and distances vector */
	for (i = 0; i < num_nodes; i++) {
		if (lengths[src][i]) {
			data->dist[i] = lengths[src][i];
		} else {
			data->dist[i] = -1;
		}

		data->paths[i] = src;
	}

	return(data);
}

void
mod_Dijkstra(int source, int destination) {
	/* Create a basic data set */
	data_set	*data = init_Dijkstra(source);
	int		*soln = (int *)calloc(num_nodes, sizeof(int));
	int	*distances = data->dist;
	int	*paths = data->paths;
	int	*nodes = data->nodes;
	int	i;


	/* Now run the Dijkstra algorithm */
	for (i = 1; i < num_nodes && nodes[destination]; i++) {

		Dijkstra_step(distances, paths, nodes);
	}

	/* Print the length of the path from source to destination */
	printf("%i\n", distances[destination]);

	/* The source and destination nodes are implicitly in the solution */
	soln[destination] = 1;
	soln[source] = 1;

	path_print_recursive(soln, paths, source, destination);
	putchar('\n');
	fflush(stdout);

	/* Print the nodes not in the shortest path */
	for (i = 0; i < num_nodes; i++) {
		if (!soln[i]) {
			printf("%i ", i + 1);
		}
	}
	putchar('\n');

	/* Cleanup */
	free(data->dist);
	free(data->paths);
	free(data->nodes);
	free(data);
	free(soln);
}

void
imp_Dijkstra(int source, int destination) {
	/* Create a basic data set */
	data_set	*data = init_Dijkstra(source);
	data_set	*data_inv = init_Dijkstra(destination);
	int		*soln = (int *)calloc(num_nodes, sizeof(int));
	int		i = 1, j;
	int		common_node;

	while (i) {
		if (i++ % 2) {
			Dijkstra_step(data->dist, data->paths, data->nodes);
		} else {
			Dijkstra_step(data_inv->dist, data_inv->paths, data_inv->nodes);
		}

		for (j = 0; j < num_nodes; j++) {
			/* Check for a common node that has been selected in
				both data sets */
			if (!data->nodes[j] && !data_inv->nodes[j]) {
				common_node = j;
				i = 0;
			}
		}
	}

	/* OK, lets print the results */

	/* Print the length of the path from source to destination */
	printf("%i\n", data->dist[common_node] + data_inv->dist[common_node]);

	/* Print the list of nodes */
	soln[source] = 1;
	soln[destination] = 1;

	path_print_recursive(soln, data->paths, source, common_node);

	for (i = data_inv->paths[common_node]; i != destination; i = data_inv->paths[i]) {
		printf("%i ", i + 1);
		/* This node is in the solution */
		soln[i] = 1;
	}
	printf("%i\n", destination + 1);

	/* Print the nodes not in the shortest path */
	for (i = 0; i < num_nodes; i++) {
		if (soln[i] == 0) {
			printf("%i ", i + 1);
		}
	}
	putchar('\n');

	/* Cleanup */
	free(data->dist);
	free(data->paths);
	free(data->nodes);
	free(data);
	free(data_inv->dist);
	free(data_inv->paths);
	free(data_inv->nodes);
	free(data_inv);
	free(soln);
}

void
Dijkstra_step(int *distances, int *paths, int *nodes) {
	int	v = -1;
	int	i, w;

	/* Find the shortest edge length in the distances vector */
	for (i = 0; i < num_nodes; i++) {
		if (nodes[i]) {
			if (distances[i] > -1) {
				if (v > -1) {
					if (distances[v] > distances[i]) {
						v = i;
					}
				} else {
					v = i;
				}
			}
		}
	}

	nodes[v] = 0;	/* remove this node from C and place in to S */

	/* for each node w E C */
	for (w = 0; w < num_nodes; w++) {
		if (nodes[w]) {
			if (distances[w] > -1) {
				if (lengths[v][w] > -1) {
					if ((distances[v] + lengths[v][w]) <
						distances[w]) {
						distances[w] = distances[v] +
							lengths[v][w];
						paths[w] = v;
					}
				}
			/* Check to see if there now a edge between v and w */
			} else if (lengths[v][w] > -1) {
				distances[w] = distances[v] + lengths[v][w];
				paths[w] = v;
			}
		}
	}
}

void
path_print_recursive(int *soln, int *paths, int src, int dest) {
	if (src != dest) {
		path_print_recursive(soln, paths, src, paths[dest]);
	}
	printf("%i ", dest + 1);
	soln[dest] = 1;
}
