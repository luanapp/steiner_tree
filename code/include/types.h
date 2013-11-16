/**
 * types.h
 *
 * Contains general structures and definitions
 * which can be used in any file
 * */

#ifndef _TYPES_H_
#define _TYPES_H_




#include <stdlib.h>

#include "list.h"

struct stein {
	/* Number of nodes on the graph - retrieved directly from the initial file. */
	unsigned int n_nodes;

	/* Number of edges on the graph - retrieved directly from the initial file. */
	unsigned int n_edges;

	/* Number of stein graph terminals - retrieved directly from the initial file. */
	unsigned int n_terminals;

	/* Vector indicating which nodes are terminals */
	unsigned int *terminals;

	/* Graph adjacency matrix */
	unsigned int **adj_m;
};
typedef struct stein stein_t;


typedef struct edge {
	unsigned int v[2] __attribute__ ((packed));
} edge_t;


/* Solution list for the problem. It will be used to build a initial population
 * based in a common ancestor: a MST solution for the terminals.
 * */
typedef struct solution {
	struct list_head list;
	edge_t *edge;
} solution_t;



/**
 * Defines a common variable statically linked to a specific region,
 *
 * */
#ifdef STEIN_MODULE
extern stein_t __current;
#define THIS_STEIN (&__current)
#endif


/**
 * Return the current stein structure without requiring the -DSTEIN_MODULE flag
 * at compile time.
 *  */
stein_t *get_stein();


/**
 * alloc_adj_m - Allocate memory for the adjacency matrix acording to the values
 * of n_nodes and n_edges.
 * */
void alloc_adj_m();


/**
 * alloc_terminals - Allocate memory for the terminals vector acording 
 * to the value of n_terminals.
 * */
void alloc_terminals();


/**
 * free_stein - Free the current stein struct allocatted memory.
 * */
void free_stein();


/**
 * alloc_solution - Allocate memory for the solution and its edge.
 * */
solution_t *alloc_solution();


/**
 * free_solution - Deallocate the memory of the given solution_t and its edge.
 *
 * @solution: solution_t memory to free.
 * */
void free_solution(solution_t *solution);


#endif /* _TYPES_H */
