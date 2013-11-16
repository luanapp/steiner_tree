/**
 * mst.c - Prim's implementation for the Minimum Spaning Tree problem.
 * The implementation saves the solution as a set of edges representing the
 * tree. This structure is defined at include/types.h, as the solution is global
 * for the whole program.
 * */

#include <limits.h>

#include "include/mst.h"
#include "include/errno.h"
#include "include/print.h"

static LIST_HEAD(terminal_head);
static LIST_HEAD(terminal_solution_head);
static LIST_HEAD(solution_head);

/**
 * This is a temporary structure to store the terminals not yet added to the
 * MST.
 * */
typedef struct _terminal_list {
	struct list_head list;
	unsigned int v;
} _terminal_list_t;


/**
 * create_terminal - Allocate memory for a _terminal_list_t and set the vertex
 * with the given value.
 *
 * @value: Value to set in the vertex(v).
 * */
_terminal_list_t *create_terminal(unsigned int value) {
	_terminal_list_t *tl;
	tl = malloc(sizeof(_terminal_list_t));
	tl->v = value;
	return tl;
}


/**
 * create_list_from_terminals - Create the linked list with all the given
 * terminals. The list is not ordered.
 * The global variable terminal_list_head is set with the linked list head.
 * Thus this method must be called before operating with the
 * terminal_list_head list.
 *
 * @terminals: The terminals array in the stein struct.
 * @size: the size of the given array.
 * */
void create_list_from_terminals(unsigned int *terminals, unsigned int size) {
	unsigned int i;

	for(i = 0; i < size; i++) {
		_terminal_list_t *_tl;
		_tl = create_terminal(terminals[i]);
		list_add_tail(&_tl->list, &terminal_head);

		pr_debug("Added terminal: %u.\n", terminals[i] + 1u);
	}
}

/**
 * retrieve_mst - Builds a maximum spanning tree with the vertexes in
 * stein->terminals, returning a pointer to the solution list head.
 *
 * @stein: stein structure with the graph representation.
 * */
 struct list_head *retrieve_mst(stein_t *stein) {
	struct list_head *t_head, *ts_head, *tmp;
	struct list_head *s_head = &solution_head;

	pr_debug("Creating terminal list to retrieve the mst.\n", 0);

	/* terminal_list_head is now set */
	create_list_from_terminals(stein->terminals, stein->n_terminals);

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
		_terminal_list_t *terminal_out = NULL, *terminal_in = NULL;
		_terminal_list_t *selected_v = NULL, *selected_u = NULL;
		solution_t *s;


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
			s = alloc_solution();
			s->edge->v[0] = selected_v->v;
			s->edge->v[1] = selected_u->v;
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

	return s_head;
}
