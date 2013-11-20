

#include <stdio.h>

#include "include/errno.h"
#include "include/print.h"
#include "include/file_reader.h"
#include "include/population.h"

int main(int argc, char *argv[])
{
	char *filename;
	struct stein *stein_data;
	struct list_head *s = NULL;

	if(!(filename = argv[1])) {
		ERRNO = EFILENAME_MISSING;
		pr_error("No filename was provided. Exiting. Error code = %d.\n"
				, ERRNO);
		goto missing_file;
	}

	if(!(stein_data = get_stein_from_file(filename)))
		goto reset_stein;

	if (!(s = create_initial_population(stein_data)))
		goto initial_population_error;

initial_population_error:
	free_stein();
reset_stein:
missing_file:
	return -ERRNO;
}
