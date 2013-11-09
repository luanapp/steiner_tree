/**
 * file_reader.c
 *
 * Contains only one exported function: get_stein_from_file.
 * All the others are only used internally.
 *
 *
 * */

#include <string.h>

#include "include/errno.h"
#include "include/print.h"
#include "include/file_reader.h"

#define BUFFER_SIZE 128

static char buffer[BUFFER_SIZE];

static char *_str_token = NULL;

/* Field separator in the files */
static const char _token[2] = " ";

static char _nodes[] = "Nodes";
static char _edges[] = "Edges";
static char _edge_prefix[] = "E";


/**
 * set_field - Set the given field with the value retrieved from the file.
 * If the field was not found, the method returns an error number (!= 0).
 * @field_name: field to search as the first word in the line.
 * @member_to_set: address to the field which will be set with the retrieved
 * value.
 */
int set_field(FILE *file, char *field_name, int *member_to_set) {

	if(fgets(buffer, BUFFER_SIZE, file) == NULL)
		return EINVALID_FILE_FORMAT;

	_str_token = strtok(buffer, _token);
	if(strcmp(_str_token, field_name) != 0)
		return EINVALID_FILE_FORMAT;

	_str_token = strtok(NULL, _token);
	*member_to_set = atoi(_str_token);
	return 0;
}

/**
 * Read the next n=stein->n_edges lines of the file getting the edges weights.
 * Each line with the edge information has the following format:
 * "E V1 V2 W\n" where "E" is a prefix, V1 and V2
 * are the vertex of the edge and W is the edge weight.
 * */
int set_matrix_value(FILE *file, char *prefix, stein_t *stein) {
	int i,j,w,x;

	for(x = 0; x < stein->n_edges; x++) {
		if(fgets(buffer, BUFFER_SIZE, file) == NULL)
			return EUNEXPECTED_FILE_FORMAT;

		_str_token = strtok(buffer, _token);
		if(strcmp(_str_token, prefix) != 0)
			return EUNEXPECTED_FILE_FORMAT;

		_str_token = strtok(NULL, _token);
		i = atoi(_str_token) - 1;

		_str_token = strtok(NULL, _token);
		j = atoi(_str_token) - 1;

		_str_token = strtok(NULL, _token);
		w = atoi(_str_token);

		/**
		 * As the graph is complete and undirected, the weight of (i, j)
		 * is the same as the weight of (j, i). And the edges are only
		 * once in the file.
		 * And the vertex indexes in the files are numbers from 1 to V
		 * (number of vertexes), therefore the indexes are decreased by
		 * 1.
		 */
		stein->adj_m[i][j] = w;
		stein->adj_m[j][i] = w;
		pr_debug("Edge(%d,%d) weight value: %d.\n", i + 1, j + 1,
				stein->adj_m[i][j]);
	}

	return 0;
}


/**
 * get_stein_from_file - Retrieve the data from the given file and returns the stein structure
 * associated with the extracted data.
 * @filename: path to the file from where to retrieve the data.
 * */
stein_t *get_stein_from_file(char *filename) {
	FILE *file;
	stein_t *stein_data;

	if(!(file = fopen(filename, "r+"))) {
		ERRNO = EFILE_NOT_FOUND;
		return NULL;
	}

	stein_data = get_stein();
	pr_debug("Getting stein_data at %p...\n", stein_data);

	/* Retrieve and check if the nodes and edge totals
	 * were retrieved correctly.
	 * */
	if(set_field(file, _nodes, &(stein_data->n_nodes)) != 0 ||
		set_field(file, _edges, &(stein_data->n_edges)) != 0) {
		ERRNO = EINVALID_FILE_FORMAT;
		return NULL;
	}
	pr_debug("Fields changed: n_nodes=%d; n_edges=%d.\n",
			stein_data->n_nodes, stein_data->n_edges);

	/* Allocatte memory for the adjacency matrix and retrieve the nodes from
	 * file. The next 'stein_data->n_edges' lines have information for all
	 * edges.
	 * */
	alloc_adj_m();
	pr_debug("Adjacency matrix created at=%p\n", stein_data->adj_m);


	/* The next stein_data->n_edges lines describes all the graph edges in
	 * the following format: "E V1 V2 W\n" where "E" is a prefix, V1 and V2
	 * are the vertex of the edge and W is the edge weight.
	 * */
	if(set_matrix_value(file, _edge_prefix, stein_data) != 0) {
		ERRNO = EUNEXPECTED_FILE_FORMAT;
		return NULL;
	}

	return stein_data;
}

