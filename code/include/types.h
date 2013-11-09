/**
 * types.h
 *
 * Contains general structures and definitions
 * which can be used in any file
 * */

#ifndef _TYPES_H_
#define _TYPES_H_
#endif

#include <stdlib.h>

struct stein {
	/* Number of nodes on the graph - retrieved directly from the initial file. */
	int n_nodes;

	/* Number of edges on the graph - retrieved directly from the initial file. */
	int n_edges;

	/* Number of stein graph terminals - retrieved directly from the initial file. */
	int n_terminals;

	/* Vector indicating which nodes are terminals */
	int *terminals;

	/* Graph adjacency matrix */
	int **adj_m;
};
typedef struct stein stein_t;



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
 * free_stein - Free the current stein struct allocatted memory.
 * */
void free_stein();

