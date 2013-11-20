/*
 * types.c
 *
 * Contains methods wich deal with the stein data structure.
 *
 * */

#include <limits.h>

#include "include/types.h"
#include "include/print.h"
#include "include/errno.h"

/**
 * This is a common variable statically linked to a specific region,
 * defined as ___stein_scope.
 *
 * */
#ifdef STEIN_MODULE
struct stein __current
	__attribute__((__used__))
	__attribute__((__section__("___stein_module")));
#define THIS_STEIN (&__current)
#endif


/**
 * alloc_adj_m - Allocate memory for the adjacency matrix acording to the values
 * of n_nodes and n_edges.
 * */
void alloc_adj_m()
{
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
void alloc_terminals()
{
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
struct stein *get_stein()
{
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
struct solution *alloc_solution()
{
	struct solution *s;
	s = malloc(sizeof(*s));
	return s;
}


/**
 * free_solution - Deallocate the memory of the given solution_t and its edge.
 *
 * @s: solution_t memory to free.
 * */
void free_solution(struct solution *s)
{
	free(s->edge);
	free(s);
}


static LIST_HEAD(_s_head);
/**
 * copy_solution - Use this function to create a copy of an entire solution
 * list. It will walk down the solution list allocating a new memory chunk for
 * every solution.
 *
 * @source: Solution to be copied.
 * */
struct list_head *copy_solution(struct list_head *source)
{
	struct solution *new_s, *tmp, *err_p;

	list_for_each_entry(tmp, source, list) {
		if(!(new_s = alloc_solution()))
			goto free_list;

		*new_s = *tmp;
		memcpy(new_s->edge, tmp->edge, sizeof(tmp->edge));
		new_s->list.prev = new_s->list.next = NULL;
		list_add_tail(&new_s->list, &_s_head);
	}
	return (&_s_head);

free_list:
	free_list_entry(&_s_head, err_p);
	ERRNO = ENOMEM;
	return NULL;
}
