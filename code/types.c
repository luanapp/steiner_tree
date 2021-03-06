/*
 * types.c
 *
 * Contains methods wich deal with the stein data structure.
 *
 * */

#include <limits.h>

#include "include/types.h"
#include "include/list.h"
#include "include/print.h"
#include "include/errno.h"

/**
 * This is a common variable statically linked to a specific region,
 * defined as __stein_section.
 *
 * */
#ifdef STEIN_MODULE
struct stein __current
	__attribute__((used))
	__attribute__((section("__stein_section")));
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
 * alloc_population - Allocate memory for a population.
 * */
struct population *alloc_population()
{
	struct population *p = NULL;
	p = malloc(sizeof(*p));
	return p;
}


/**
 * free_population - iterates over the population list and free its solution and
 * the population struct itself.
 *
 * @head: population list head.
 * */
void free_population_list(struct list_head *head)
{
	struct list_head *p_list, *tmp;
	struct solution *s;

	list_for_each_safe(p_list, tmp, head) {
		struct population *p;
		p = list_entry(p_list, struct population, list);
		free_list_entry(&p->solution, s, list);
		free(p);
	}

}

/**
 * copy_solution - Use this function to create a copy of an entire solution
 * list. It will walk down the solution list allocating a new memory chunk for
 * every solution.
 *
 * @source: Solution to be copied.
 * @s_head: Solution list head for the new solution.
 * */
void copy_solution(struct list_head *source, struct list_head *s_head)
{
	struct solution *tmp = NULL, *err_p = NULL;

	list_for_each_entry(tmp, source, list) {
		struct solution *new_s = NULL;

		if(!(new_s = alloc_solution()))
			goto free_list;

		new_s->edge[0] = tmp->edge[0];
		new_s->edge[1] = tmp->edge[1];
		new_s->w = tmp->w;
		new_s->list.prev = NULL;
		new_s->list.next = NULL;
		list_add_tail(&(new_s->list), s_head);
		pr_debug("Copying edge (%u,%u) at %p.\n", new_s->edge[0] + 1u,
				new_s->edge[1] + 1u, *new_s);
	}
	return;
free_list:
	pr_error("Solution was not copied. Head at %p.\n\n", *s_head);
	free_list_entry(s_head, err_p, list);
	free(s_head);
	ERRNO = ENOMEM;
	return;
}


/**
 * solution_has_v - Search for the vertex v in the solution.
 *
 * @h: solution list head.
 * @v: vertex to search.
 * */
int solution_has_v(struct list_head *h, unsigned int v)
{
	struct solution *s;
	list_for_each_entry(s, h, list) {
		if(s->edge[0] == v || s->edge[1] == v)
			return 1;
	}
	return 0;
}


/**
 * update_solution_weight - Update the given solution list weight.
 *
 * @s_head: solution list head pointer.
 * @w: new weight.
 * */
void update_solution_weight(struct list_head *s_head, unsigned int w)
{
	struct solution *s;
	list_for_each_entry(s, s_head, list) {
		s->w = w;
	}
}
