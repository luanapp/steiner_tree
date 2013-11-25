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
	struct solution *s, *err_s;


	if(!(common_ancestor = retrieve_mst(stein)))
		goto fail_mst_create;


	s = list_entry(common_ancestor, struct solution, list);
	pr_debug("Common ancestor with %d edges was created at %p.\n",
			list_size(common_ancestor), *s);

	p_head = &_pop_head;
	for(i = 0; i < POP_SIZE; i++) {
		struct population *p;

		if(!(p = alloc_population()))
			goto fail_pop_create;

		s_head = malloc(sizeof(*s_head));
		INIT_LIST_HEAD(s_head);
		copy_solution(common_ancestor, s_head);
		p->solution = s_head;
		list_add_tail(&p->list, p_head);
		pr_debug("Solution copied at %p.\n", *s_head);

	}
	return p_head;

fail_pop_create:
	pr_error("Could not allocate population. p head=%p.\n\n", &_pop_head);
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
	struct list_head *p_head;
	struct solution *s, *n;
	struct population *p, *np;

	if(!(p_head = get_population_from_mst(stein)))
		goto fail_create_pop;

	pr_debug("Population with size %d created at %p.\n", list_size(p_head),
			*p_head);

	list_for_each_entry_safe(p, np, p_head, list) {

		pr_debug("Current population at %p. p->solution at %p\n",
				*p, *p->solution);
		list_for_each_entry_safe(s, n, p->solution, list) {

			/**
			 * TODO: create a way to put the solution weight into
			 * account.
			 * */
			if(range_rand(0, RAND_MAX) % 4 == 0) {
				pr_debug("Mutating (%u, %u).\n", s->edge[0] + 1u
						, s->edge[1] + 1u);
				mutation(s, stein);
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
 * @s: solution to check
 * */
static unsigned int get_new_v(struct stein *stein, struct solution *s)
{
	unsigned int v;
	do {
		v = range_rand(0, stein->n_nodes - 1);
	} while (solution_has_v(&s->list, v));


	return v;
}


/**
 * add_new_v - Adds the given vertex in the given solution.
 * The new edges are create using the vertex "v", connecting it to the solution
 * "s" edge vertexes. This, therefore, removes the current edge from solution.
 * */
static void add_new_v(struct stein *stein, struct solution *s, unsigned int v)
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
	list_add_tail(&s1->list, &s->list);
	list_add_tail(&s2->list, &s->list);
	list_del(&s->list);

	/* Update the solution weight */
	update_solution_weight(&s1->list, new_w);

	pr_debug("Solution updated: weight=%u, old weight=%u, s=%u, w1=%u, w2=%u.\n",
			new_w, s->w, old_w, new_w1, new_w2);
	free_solution(s);
}

/**
 * Mutations are based on a triangle inequality, i.e., when a mutation is
 * performed, it will add a non-terminal vertex to the solution as a replacement
 * to a direct edge between two terminals. The method, thus, adds two edges to the
 * solution and removes one.
 *
 * @s: Solution which will mutate.
 * @not_t: Number of edges that aren't terminals.
 * */
void mutation(struct solution *s, struct stein *stein)
{
	int v;

	pr_debug("Getting new vertex for the edge (%u, %u).\n", s->edge[0] + 1u,
			s->edge[1] + 1u);

	/* TODO: Check how much the function bellow affects the performance */
	v = get_new_v(stein, s);
	pr_debug("Selected vertex: %d\n", v);

	add_new_v(stein, s, v);
}


/**
 * Crossover is based on an exchange of partial solutions of a population, i.e.,
 * given two solutions, part of the solution structure - the use or not of an
 * intermediate terminal - is exchanged, forming a new population.
 * */
void crossover(struct solution *s1, struct solution *s2)
{
}

