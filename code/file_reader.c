/**
 * file_reader.c
 *
 * Contains only one exported function: get_stein_from_file.
 * All the others are only used internally.
 *
 *
 * */

#include <string.h>
#include <errno.h>
#include <limits.h>

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
static char _terminals[] = "Terminals";
static char _edge_prefix[] = "E";
static char _terminal_prefix[] = "T";


/**
 * Internal method to get the file lines and increment the line counter to keep
 * track of the current line number.
 * */
static int FILE_LINE = 0;
static inline char *__fgets(char *buffer, int buff_size, FILE *file)
{
	FILE_LINE++;
	return fgets(buffer, buff_size, file);
}


/**
 * strtous - Converts the nptr string into a unsigned int,
 * using the function strtoul with error verification during the convertion.
 *
 * @nptr: String to convert
 * @endptr: Pointer to a pointer to the address of the first invalid character.
 * @base: Convertion base, e.g, decimal(10), hexadecimal(16), etc.
 * */
static unsigned int strtous(char *nptr, char **endptr, int base)
{
	unsigned long tmp_value;
	unsigned int value = UINT_MAX;

	errno = 0;
	tmp_value = strtoul(nptr, endptr, base);
	if(errno == 0 && tmp_value <= UINT_MAX) {
		value = (unsigned int) tmp_value;
	}
	check_error();
	return value;
}

/**
 * set_field - Set the given field with the value retrieved from the file.
 * If the field was not found, the method returns an error number (!= 0).
 *
 * @file: file with the field to retrieve, with stream position in the begining
 * of the line.
 * @field_name: field to search as the first word in the line.
 * @member_to_set: address to the field which will be set with the retrieved
 * value.
 */
static int set_field(FILE *file, char *field_name, unsigned int *member_to_set)
{

	if(__fgets(buffer, BUFFER_SIZE, file) == NULL)
		return EINVALID_FILE_FORMAT;

	_str_token = strtok(buffer, _token);
	if(strcmp(_str_token, field_name) != 0)
		return EINVALID_FILE_FORMAT;

	_str_token = strtok(NULL, _token);
	*member_to_set = strtous(_str_token, NULL, 0);
	return 0;
}

/**
 * set_matrix_values - Read the next n=stein->n_edges lines of 
 * the file getting the edges weights.
 * Each line with the edge information has the following format:
 * "E V1 V2 W\n" where "E" is a prefix, V1 and V2
 * are the vertex of the edge and W is the edge weight.
 *
 * @file: file with the field to retrieve, with stream position in the begining
 * of the line.
 * @prefix: prefix to be checked as the first character in the line.
 * @stein: current stein structure.
 * */
static int set_matrix_values(FILE *file, char *prefix, struct stein *stein)
{
	unsigned int i, j, w, x;

	for(x = 0; x < stein->n_edges; x++) {
		if(__fgets(buffer, BUFFER_SIZE, file) == NULL)
			return EUNEXPECTED_FILE_FORMAT;

		_str_token = strtok(buffer, _token);
		if(strcmp(_str_token, prefix) != 0)
			return EUNEXPECTED_FILE_FORMAT;

		 /* The vertex indexes in the files are numbers from 1 to V 
		  * (number of vertexes), therefore the indexes are decreased by 1.
		 * */
		_str_token = strtok(NULL, _token);
		i = strtous(_str_token, NULL, 0) - 1u;

		_str_token = strtok(NULL, _token);
		j = strtous(_str_token, NULL, 0) - 1u;

		_str_token = strtok(NULL, _token);
		w = strtous(_str_token, NULL, 0);

		/**
		 * As the graph is complete and undirected, the weight of (i, j)
		 * is the same as the weight of (j, i). And the edges are only
		 * once in the file.
		 */
		stein->adj_m[i][j] = w;
		stein->adj_m[j][i] = w;
		pr_debug("Edge(%d,%d) weight value: %u.\n", i + 1, j + 1,
				stein->adj_m[i][j]);
	}

	return 0;
}


/**
 * set_terminals - Read stein->n_terminals lines to get all the graph terminals.
 *
 * @file: file with the field to retrieve, with stream position in the begining
 * of the line.
 * @prefix: prefix to be checked as the first character in the line.
 * @stein: current stein structure.
 * */
static int set_terminals(FILE *file, char *prefix, struct stein *stein)
{
	unsigned int i, v;

	/* Allocate memory for the terminals */
	alloc_terminals();

	for(i = 0; i < stein->n_terminals; i++) {
		if(__fgets(buffer, BUFFER_SIZE, file) == NULL)
			return EUNEXPECTED_FILE_FORMAT;

		_str_token = strtok(buffer, _token);
		if(strcmp(_str_token, prefix) != 0)
			return EUNEXPECTED_FILE_FORMAT;

		_str_token = strtok(NULL, _token);
		v = strtous(_str_token, NULL, 0);

		 /* The vertex indexes in the files are numbers from 1 to V 
		  * (number of vertexes), therefore the indexes are decreased by 1.
		 * */
		stein->terminals[i] = v - 1u;
		pr_debug("Terminal %d added.\n", v);
	}
	return 0;
}


/**
 * get_stein_from_file - Retrieve the data from the given file and returns the stein structure
 * associated with the extracted data.
 * @filename: path to the file from where to retrieve the data.
 * */
struct stein *get_stein_from_file(char *filename)
{
	FILE *file;
	struct stein *stein_data;
	char *chk_eof;

	if(!(file = fopen(filename, "r+"))) {
		ERRNO = EFILE_NOT_FOUND;
		pr_error("\nInvalid file. errno=%d\n\n", ERRNO);
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
		pr_error("\nWrong file format at line %d.\n\n", FILE_LINE);
		return NULL;
	}
	pr_debug("Fields changed: n_nodes=%u; n_edges=%u.\n",
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
	if(set_matrix_values(file, _edge_prefix, stein_data) != 0) {
		ERRNO = EUNEXPECTED_FILE_FORMAT;
		pr_error("\nWrong file format at line %d.\n\n", FILE_LINE);
		return NULL;
	}

	/* There is an empty line afer getting the edges
	 * */
	chk_eof = __fgets(buffer, BUFFER_SIZE, file);
	if(chk_eof == NULL || strcmp(chk_eof, "\n") != 0) {
		ERRNO = EUNEXPECTED_FILE_FORMAT;
		pr_error("\nWrong file format. Missing empty line at line %d. check_eof=%s.\n\n", FILE_LINE, chk_eof);
		return NULL;
	}


	/* Retrieve and check the number of terminals given in the file.
	 * */
	if(set_field(file, _terminals, &(stein_data->n_terminals)) != 0) {
		ERRNO = EINVALID_FILE_FORMAT;
		pr_error("\nWrong file format at line %d.\n\n", FILE_LINE);
		return NULL;
	}
	stein_data->not_t = stein_data->n_edges - stein_data->n_terminals;
	pr_debug("Fields changed: n_terminals=%u, not_t=%u.\n",
			stein_data->n_terminals, stein_data->not_t);


	/* The next stein_data->n_terminals lines contains the edges that are
	 * terminals in the stein tree.
	 * */
	if(set_terminals(file, _terminal_prefix, stein_data) != 0) {
		ERRNO = EUNEXPECTED_FILE_FORMAT;
		pr_error("\nWrong file format at line %d.\n\n", FILE_LINE);
		return NULL;
	}

	return stein_data;
}

