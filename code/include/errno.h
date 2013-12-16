/**
 * errno.h
 *
 * Common error numbers and a global error variable to identify the execution exit error.
 *
 *
 * */

#ifndef _ERRNO_H_
#define _ERRNO_H_


#define EFILENAME_MISSING 101;
#define EFILE_NOT_FOUND 102;
#define EINVALID_FILE_FORMAT 103;
#ifndef ENOMEM
#define ENOMEM 105;
#endif
#define EUNEXPECTED_ERROR 106;



#define check_error() \
	do { \
		if(errno != 0) \
			pr_error("\n\nThere was an error in the line %d.\n\n", \
				__LINE__); \
	} while(0)


/**
 * Jumps to the given label if an error was detected.
 * */
#define error_goto(jump) \
	do { \
		if(ERRNO != 0) { \
			pr_error("\n\nThere was an error in the line %d.\n\n", \
					__LINE__); \
			goto jump; \
		} \
	} while(0)

static int ERRNO = 0;


#endif
