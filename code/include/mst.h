/**
 * mst.h - Prim's implementation for the Minimum Spaning Tree problem.
 * The implementation saves the solution as a set of edges representing the
 * tree. This structure is defined at include/types.h, as the solution is global
 * for the whole program.
 * */


#ifndef _MST_H_
#define _MST_H_


#include "types.h"



/**
 * retrieve_mst - Builds a maximum spanning tree with the vertexes in
 * stein->terminals, returning a pointer to the solution list head.
 *
 * @stein: stein structure with the graph representation.
 * */
struct list_head *retrieve_mst(struct stein *stein);

#endif /* _MST_H_ */
