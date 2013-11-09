/**
 * file_reader.h
 *
 * Contains only one exported function: get_stein_from_file.
 * All the others are only used internally.
 *
 *
 * */


#ifndef _FILE_READER_H_
#define _FILE_READER_H_
#endif

#include <stdlib.h>

#include "types.h"



/**
 * get_stein_from_file - Retrieve the data from the given file and returns the stein structure
 * associated with the extracted data.
 * @filename: path to the file from where to retrieve the data.
 * */
stein_t *get_stein_from_file(char *);

