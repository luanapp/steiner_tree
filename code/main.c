

#include <stdio.h>

#include "include/errno.h"
#include "include/print.h"
#include "include/file_reader.h"
#include "include/mst.h"


int main(int argc, char *argv[]) {
	char *filename;
	stein_t *stein_data;
	struct list_head *l;
	solution_t *s = NULL;

	if(!(filename = argv[1])) {
		ERRNO = EFILENAME_MISSING;
		pr_error("No filename was provided. Exiting. Error code = %d.\n"
				, ERRNO);
		goto missing_file;
	}

	if(!(stein_data = get_stein_from_file(filename)))
		goto reset_stein;

	l = retrieve_mst(stein_data);

	return 0;
reset_stein:
	free_stein();
missing_file:
	return -ERRNO;
}
