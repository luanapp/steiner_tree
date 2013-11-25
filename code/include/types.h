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

	/* Contains the number of nodes that aren't terminals
	 * (n_nodes - n_terminals) */
	unsigned int not_t;

	/* Vector indicating which nodes are terminals */
	unsigned int *terminals;

	/* Graph adjacency matrix */
	unsigned int **adj_m;
};


/* Solution list for the problem. It will be used to build a initial population
 * based in a common ancestor: a MST solution for the terminals.
 * */
struct solution {
	struct list_head list;
	int w;
	unsigned int edge[2] __attribute__ ((packed));
};

struct population {
	struct list_head *solution;
	struct list_head list;
};



/**
 * Defines a common variable statically linked to a specific region,
 *
 * */
#ifdef STEIN_MODULE
extern struct stein __current;
#define THIS_STEIN (&__current)
#endif


/**
 * Return the current stein structure without requiring the -DSTEIN_MODULE flag
 * at compile time.
 *  */
struct stein *get_stein();


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
struct solution *alloc_solution();


/**
 * free_solution - Deallocate the memory of the given solution_t and its edge.
 *
 * @s: solution_t memory to free.
 * */
void free_solution(struct solution *s);


/**
 * copy_solution - Use this function to create a copy of an entire solution
 * list. It will walk down the solution list allocating a new memory chunk for
 * every solution.
 *
 * @source: Solution list head to be copied.
 * @s_head: Solution list head for the new solution.
 * */
void copy_solution(struct list_head *source, struct list_head *s_head);


/**
 * solution_has_v - Search for the vertex v in the solution.
 *
 * @h: solution list head.
 * @v: vertex to search.
 * */
int solution_has_v(struct list_head *h, unsigned int v);


/**
 * update_solution_weight - Update the given solution list weight.
 *
 * @s_head: solution list head pointer.
 * @w: new weight.
 * */
void update_solution_weight(struct list_head *s_head, unsigned int w);

#endif /* _TYPES_H */
