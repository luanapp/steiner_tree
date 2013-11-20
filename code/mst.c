/**
 * mst.c - Prim's implementation for the Minimum Spaning Tree problem.
 * The implementation saves the solution as a set of edges representing the
 * tree. This structure is defined at include/types.h, as the solution is global
 * for the whole program.
 * */

#include <limits.h>

#include "include/mst.h"
#include "include/print.h"
#include "include/errno.h"

static LIST_HEAD(terminal_head);
static LIST_HEAD(terminal_solution_head);
static LIST_HEAD(solution_head);

/**
 * This is a temporary structure to store the terminals not yet added to the
 * MST.
 * */
struct _terminal_list {
	struct list_head list;
	unsigned int v;
};


/**
 * create_terminal - Allocate memory for a _terminal_list_t and set the vertex
 * with the given value.
 *
 * @value: Value to set in the vertex(v).
 * */
static inline struct _terminal_list *create_terminal(unsigned int value)
{
	struct _terminal_list *tl;
	tl = malloc(sizeof(*tl));
	tl->v = value;
	return tl;
}


/**
 * get_list_from_terminals - Create the linked list with all the given
 * terminals. The list is not ordered.
 * The global variable terminal_list_head is set with the linked list head.
 * Thus this method must be called before operating with the
 * terminal_list_head list.
 *
 * @terminals: The terminals array in the stein struct.
 * @size: the size of the given array.
 * */
static inline void get_list_from_terminals(unsigned int *terminals,
		unsigned int size)
{
	unsigned int i;
	struct _terminal_list *err_tl;

	for(i = 0; i < size; i++) {
		struct _terminal_list *_tl;

		if(!(_tl = create_terminal(terminals[i])))
			goto free_list;

		list_add_tail(&_tl->list, &terminal_head);

		pr_debug("Added terminal: %u.\n", terminals[i] + 1u);
	}
	return;
free_list:
	free_list_entry(&terminal_head, err_tl);
	ERRNO = ENOMEM;
	return;
}

/**
 * retrieve_mst - Builds a maximum spanning tree with the vertexes in
 * stein->terminals, returning a pointer to the solution list head.
 *
 * @stein: stein structure with the graph representation.
 * */
struct list_head *retrieve_mst(struct stein *stein)
{
	struct solution *err_s;
	struct list_head *t_head, *ts_head, *tmp;

	pr_debug("Creating terminal list to retrieve the mst.\n", 0);

	/* terminal_list_head is now set */
	get_list_from_terminals(stein->terminals, stein->n_terminals);
	error_goto(fail_get_terminals);

	/* ts_head contains all terminals not yet added in the mst
	 * t_head contains the terminals already added.
	 * */
	ts_head = &terminal_head;
	t_head = &terminal_solution_head;

	pr_debug("List created: size=%u.\n", list_size(ts_head));

	/* Select the first vertex - the last in the list and
	 * remove it from the not added vertex list. The vertex is then added to
	 * the solution vertex list. */
	tmp = ts_head->prev;
	list_del(ts_head->prev);
	list_add_tail(tmp, t_head);

	while(ts_head->next != ts_head) {
		unsigned int v = UINT_MAX;
		unsigned int min_cost =  UINT_MAX;
		struct _terminal_list *terminal_out = NULL;
		struct _terminal_list *terminal_in = NULL;
		struct _terminal_list *selected_v = NULL;
		struct _terminal_list *selected_u = NULL;
		struct solution *s;


		/**
		 * For each vertex added in the MST, it will be searched the
		 * vertex with minimum cost to be added in the MST.
		 * */
		list_for_each_entry(terminal_in, t_head, list) {
			v = terminal_in->v;

			list_for_each_entry(terminal_out, ts_head, list) {
				unsigned int u = terminal_out->v;
				unsigned int w = stein->adj_m[v][u];

				pr_debug("Current data: v=%u; u=%u; w=%u; min_cost=%u\n",
						v + 1u, u + 1u, w, min_cost);

				/**
				 * Select the edge with minimum cost in the
				 * current list.
				 * */
				if(w < min_cost) {
					selected_v = terminal_in;
					selected_u = terminal_out;
					min_cost = w;
				}
				if(terminal_out->list.next == ts_head) break;
			}
			if(terminal_in->list.next == t_head) break;
		}

		if(selected_v != selected_u && min_cost != UINT_MAX) {
			struct list_head *u_tmp = NULL;


			/* Add the selected edge to the solution */
			if(!(s = alloc_solution()))
				goto fail_alloc_sol;

			s->edge[0] = selected_v->v;
			s->edge[1] = selected_u->v;
			list_add_tail(&s->list, &solution_head);

			/* Remove the u vertex from the not selected list 
			 * and add it to the selected vertex list. */
			u_tmp = &selected_u->list;
			list_del(u_tmp);
			list_add_tail(u_tmp, t_head);

			pr_debug("Selected edge:(%u, %u), w=%u.\n",
					selected_v->v + 1u, selected_u->v + 1u,
					min_cost);
		}
	}

	return (&solution_head);
fail_alloc_sol:
	free_list_entry(&solution_head, err_s);
	ERRNO = ERRNO != 0 ? ERRNO : ENOMEM;
fail_get_terminals:
	return NULL;
}
