/*
 * types.c
 *
 * Contains methods wich deal with the stein data structure.
 *
 * */

#include <limits.h>

#include "include/types.h"
#include "include/print.h"

/**
 * This is a common variable statically linked to a specific region,
 * defined as ___stein_scope.
 *
 * */
#ifdef STEIN_MODULE
stein_t __current
	__attribute((__used__))
	__attribute((__section__("___stein_module")));
#define THIS_STEIN (&__current)
#endif


/**
 * alloc_adj_m - Allocate memory for the adjacency matrix acording to the values
 * of n_nodes and n_edges.
 * */
void alloc_adj_m() {
	int i;

	if(THIS_STEIN->n_nodes > 0 && THIS_STEIN->n_edges > 0) {

		/* Allocate memory for the lines */
		THIS_STEIN->adj_m = malloc(sizeof(*(THIS_STEIN->adj_m)) *
				THIS_STEIN->n_nodes);

		/* Allocate memory for the columns */
		for(i = 0; i < THIS_STEIN->n_nodes; i++) {
			THIS_STEIN->adj_m[i] =
				malloc(sizeof(**(THIS_STEIN->adj_m)) *
						THIS_STEIN->n_nodes);

			/* Initialize the [i][i] values with a high value,
			 * since there is no edge starting and ending in the
			 * same vertex.
			 * */
			THIS_STEIN->adj_m[i][i] = UINT_MAX;
		}
	}
}


/**
 * alloc_terminals - Allocate memory for the terminals vector acording 
 * to the value of n_terminals.
 * */
void alloc_terminals() {
	if(THIS_STEIN->n_terminals > 0) {
		THIS_STEIN->terminals =
			malloc(sizeof(*(THIS_STEIN->terminals)) *
				THIS_STEIN->n_terminals);
	}
}


/**
 * Return the current stein structure without requiring the -DSTEIN_MODULE flag
 * at compile time.
 *  */
stein_t *get_stein() {
	return THIS_STEIN;
}


/**
 * free_stein - Free the current stein struct allocatted memory.
 * */
void free_stein() {
	int i = THIS_STEIN->n_nodes - 1;
	if(THIS_STEIN->adj_m != NULL) {
		for(; i >= 0; i--) {
			free((THIS_STEIN->adj_m)[i]);
		}
		free(THIS_STEIN->adj_m);
	}

	if(THIS_STEIN->terminals != NULL)
		free(THIS_STEIN->terminals);
}



/**
 * alloc_solution - Allocate memory for the solution_t and its edge.
 * */
solution_t *alloc_solution() {
	solution_t *s;

	s = malloc(sizeof(solution_t));
	s->edge = malloc(sizeof(edge_t));
	return s;
}


/**
 * free_solution - Deallocate the memory of the given solution_t and its edge.
 *
 * @solution: solution_t memory to free.
 * */
void free_solution(solution_t *solution) {
	free(solution->edge);
	free(solution);
}


