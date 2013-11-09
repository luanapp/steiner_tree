/*
 * types.c
 *
 * Contains methods wich deal with the stein data structure.
 *
 * */

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
		THIS_STEIN->adj_m = malloc(sizeof(**(THIS_STEIN->adj_m)) *
				THIS_STEIN->n_nodes);

		/* Allocate memory for the columns */
		for(i = 0; i < THIS_STEIN->n_nodes; i++) {
			THIS_STEIN->adj_m[i] =
				malloc(sizeof(*(THIS_STEIN->adj_m)) *
						THIS_STEIN->n_nodes);
		}
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
}
