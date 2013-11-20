/**
 * population.c - Implements the main actions performed in a population and its
 * individuals (solutions), such as, creation, crossover, mutation.
 * */


#include "include/print.h"
#include "include/errno.h"
#include "include/population.h"
#include "include/mst.h"


/* The default size for a population */
#ifndef POP_SIZE
#define POP_SIZE 10
#endif


static struct population *alloc_population()
{
	struct population *p = NULL;
	p = malloc(sizeof(*p));
	return p;
}


static LIST_HEAD(_pop_head);
/**
 * get_population_from_mst - Retrieve the MST from terminals and replicate it
 * POP_SIZE times. The population list head is returned.
 *
 * @stein: Stein struct to retrieve the MST.
 * */
static struct list_head *get_population_from_mst(struct stein *stein)
{
	int i;
	struct list_head *common_ancestor = NULL;
	struct list_head *s_head = NULL;
	struct list_head *p_head = NULL;
	struct population *p;
	struct solution *s, *err_s;


	if(!(common_ancestor = retrieve_mst(stein)))
		goto fail_mst_create;


	s = list_entry(common_ancestor, struct solution, list);
	pr_debug("Common ancestor with %d edges was created at %p.\n",
			list_size(common_ancestor), *s);

	p_head = &_pop_head;
	for(i = 0; i < POP_SIZE; i++) {
		if(!(p = alloc_population()))
			goto fail_pop_create;

		s_head = copy_solution(common_ancestor);
		p->solution = *s_head;
		list_add_tail(&p->list, p_head);
	}
	return p_head;

fail_pop_create:
	free_list_entry(&_pop_head, err_s);
	ERRNO = ERRNO != 0 ? ERRNO : ENOMEM;
fail_mst_create:
	return NULL;
}


/**
 * create_initial_population - From a common ancestor, create a population of
 * solutions based on this ancestor random mutations.
 *
 * @stein: Stein structure used to create a common ancestor.
 * */
struct list_head *create_initial_population(struct stein *stein)
{
	struct list_head *p_head = NULL;
	int n_mutations, i;

	if(!(p_head = get_population_from_mst(stein)))
		goto fail_create_pop;

	/* TODO: Temporary random number. It must be changed. */
	n_mutations = rand() % ((int)(stein->n_edges - stein->n_terminals));
	for(i = 0; i < n_mutations; i++) {
		/* perform mutations */

	}

	return p_head;
fail_create_pop:
	return NULL;
}

/**
 * Mutations are based on a triangle inequality, i.e., when a mutation is
 * performed, it will add a non-terminal vertex to the solution as a replacement
 * to a direct edge between two terminals.
 * */
void mutation(struct solution *population)
{
}


/**
 * Crossover is based on an exchange of partial solutions of a population, i.e.,
 * given two solutions, part of the solution structure - the use or not of an
 * intermediate terminal - is exchanged, forming a new population.
 * */
void crossover(struct solution *population)
{
}

