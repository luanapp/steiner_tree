#ifndef _MISC_H_
#define _MISC_H_


#include <time.h>
#include <libio.h>
#include <stdlib.h>
#include <limits.h>


/**
 * time_seed - Defines a seed for the srand function based on the system time.
 * Source:
 * - http://eternallyconfuzzled.com/arts/jsw_art_rand.aspx
 * */
static inline unsigned time_seed()
{
	time_t now = time ( 0 );
	unsigned char *p = (unsigned char *)&now;
	unsigned seed = 0;
	size_t i;

	for(i = 0; i < sizeof now; i++)
		seed = seed * (UCHAR_MAX + 2U) + p[i];

	return seed;
}


/**
 * range_rand - Selects a random number between the given bounds.
 *
 * @upper: the range will be lesser or equal to this number.
 * @bottom: the range will be greater or equal to this number.
 * */
static inline int range_rand(int upper, int bottom)
{
	int r;
	do {
		r = rand();
	} while (r <= upper || r >= bottom);
	return r;
}

#endif /* _MISC_H */
