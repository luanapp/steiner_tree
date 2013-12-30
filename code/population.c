/**
 * population.c - Implements the main actions performed in a population and its
 * individuals (solutions), such as, creation, crossover, mutation.
 * */


#include "include/print.h"
#include "include/errno.h"
#include "include/misc.h"
#include "include/population.h"
#include "include/mst.h"


/* The default size for a population */
#ifndef POP_SIZE
#define POP_SIZE 10
#endif



/**
 * get_population_from_mst - Retrieve the MST from terminals and replicate it
 * POP_SIZE times. The population list head is returned.
 *
 * @stein: Stein struct to retrieve the MST.
 * */
static struct list_head *get_population_from_mst(struct stein *stein)
{
	int i;
	struct list_head *common_ancestor = NULL, *_pop_head = NULL;
	struct solution *tmp_s, *err_s;


	if(!(common_ancestor = retrieve_mst(stein))) {
		pr_error("Could not allocate population. common_ancestor=0x%p.\n\n", *common_ancestor);
		goto fail_mst_create;
	}


	pr_debug("Common ancestor with %d edges was created at 0x%p.\n",
			list_size(common_ancestor), *common_ancestor);

	_pop_head = malloc(sizeof(*_pop_head));
	INIT_LIST_HEAD(_pop_head);
	for(i = 0; i < POP_SIZE; i++) {
		struct population *p = NULL;

		if(!(p = alloc_population())) {
			pr_error("Could not allocate population. p head=0x%p.\n\n", *_pop_head);
			ERRNO = ERRNO != 0 ? ERRNO : ENOMEM;
			goto fail_pop_create;
		}

		INIT_LIST_HEAD(&p->solution);
		p->list.next = NULL;
		p->list.prev = NULL;
		copy_solution(common_ancestor, &p->solution);

		list_add_tail(&p->list, _pop_head);
		pr_debug("Solution copied at 0x%p.\n", &(p->solution));

	}

	/* Free the memory allocated for the common ancestor */
	free_list_entry(common_ancestor, tmp_s, list);

	return _pop_head;

fail_pop_create:
	free_list_entry(_pop_head, err_s, list);
	free(_pop_head);
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
	struct list_head *p_head;
	struct solution *s, *n;
	struct population *p;

	if(!(p_head = get_population_from_mst(stein))) {
		pr_error("Initial population creation has failed. p_head=0x%p\n\n", *p_head);
		goto fail_create_pop;
	}

	pr_debug("Population with size %d created at 0x%p.\n", list_size(p_head)
			, *p_head);

	list_for_each_entry(p, p_head, list) {

		pr_debug("Current population at 0x%p. p->solution at 0x%p\n",
				*p, &(p->solution));
		list_for_each_entry_safe(s, n, &p->solution, list) {

			/**
			 * TODO: create a way to put the solution weight into
			 * account.
			 * */
			if(range_rand(0, RAND_MAX) % 4 == 0) {
				pr_debug("Mutating (%u, %u).\n", s->edge[0] + 1u
						, s->edge[1] + 1u);
				mutation(s, stein, &p->solution);
			}
		}
	}



	return p_head;
fail_create_pop:
	return NULL;
}

/**
 * get_new_v - Select a vertex that is not yet in the solution.
 *
 * @stein: stein struct
 * @s_head: solution to check
 * */
static unsigned int get_new_v(struct stein *stein, struct list_head *s_head)
{
	unsigned int v;
	do {
		v = range_rand(0, stein->n_nodes - 1);
	} while (solution_has_v(s_head, v));


	return v;
}


/**
 * add_new_v - Adds the given vertex in the given solution.
 * The new edges are create using the vertex "v", connecting it to the solution
 * "s" edge vertexes. This, therefore, removes the current edge from solution.
 * */
static void add_new_v(struct stein *stein, struct solution *s, unsigned int v,
		struct list_head *s_head)
{
	struct solution *s1, *s2;
	unsigned int new_w, new_w1, new_w2, old_w;

	if(!(s1 = alloc_solution()) || !(s2 = alloc_solution())) {
		ERRNO = ENOMEM;
		pr_error("There is no memory left to allocate. ERRNO=%d\n"
				, ERRNO);
		pr_error("Solution edge (%u, %u) not changed.\n\n", s->edge[0] +
				1u, s->edge[1] + 1u);
		return;
	}
	s1->edge[0] = s->edge[0];
	s2->edge[1] = s->edge[1];

	s1->edge[1] = v;
	s2->edge[0] = v;

	/* Calculate the new solution weight */
	old_w = stein->adj_m[s->edge[0]][s->edge[1]];
	new_w1 = stein->adj_m[s1->edge[0]][s1->edge[1]];
	new_w2 = stein->adj_m[s2->edge[0]][s2->edge[1]];
	new_w = s->w - old_w + new_w1 + new_w2;

	/* Update the solution list */
	list_add_tail(&s1->list, s_head);
	list_add_tail(&s2->list, s_head);
	list_del(&s->list);

	/* Update the solution weight */
	update_solution_weight(s_head, new_w);

	pr_debug("Solution updated: weight=%u, old weight=%u, s=%u, w1=%u, w2=%u.\n",
			new_w, s->w, old_w, new_w1, new_w2);
	free(s);
}

/**
 * Mutations are based on a triangle inequality, i.e., when a mutation is
 * performed, it will add a non-terminal vertex to the solution as a replacement
 * to a direct edge between two terminals. The method, thus, adds two edges to the
 * solution and removes one.
 *
 * @s: Solution which will mutate.
 * @stein: Stein struct.
 * @s_head: Solution list head.
 * */
void mutation(struct solution *s, struct stein *stein, struct list_head *s_head)
{
	int v;

	pr_debug("Getting new vertex for the edge (%u, %u).\n", s->edge[0] + 1u,
			s->edge[1] + 1u);

	/* TODO: Check how much the function bellow affects the performance */
	v = get_new_v(stein, s_head);
	pr_debug("Selected vertex: %d\n", v);

	add_new_v(stein, s, v, s_head);
}


/**
 * Crossover is based on an exchange of partial solutions of a population, i.e.,
 * given two solutions, part of the solution structure - the use or not of an
 * intermediate terminal - is exchanged, forming a new population.
 * */
void crossover(struct solution *s1, struct solution *s2)
{
}

