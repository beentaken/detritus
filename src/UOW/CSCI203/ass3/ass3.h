
#ifndef __ASS3_H
#define __ASS3_H

typedef struct {
	int	*dist;
	int	*paths;
	int	*nodes;
} data_set;

void
mod_Dijkstra(int source, int destination);

void
imp_Dijkstra(int source, int destination);

void
Dijkstra_step(int *distances, int *paths, int * nodes);

data_set *
init_Dijkstra(int src);

void
path_print_recursive(int *soln, int *paths, int src, int dest);
 
#endif 
