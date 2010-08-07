/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <stdlib.h> /* abort */
#include <stdarg.h> /* va_* */
#include <stdio.h>  /* vfprintf, fflush */

#include "fail.h"

void r_panic(const char *fmt, 
	     const char *file, int lineno, const char *func, ...)
{
	va_list args;

	va_start(args, func);
	fprintf(stderr, "R panicked ar %s:%s:%i ", file, func, lineno);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\nAborting.\n");
	fflush(stderr); /* not exactly necessary: abort() flushes streams. */
	abort();
}

/* 
 *  Local variables:
 *  c-indentation-style: "K&R"
 *  c-basic-offset: 8
 *  tab-width: 8
 *  fill-column: 80
 *  scroll-step: 1
 *  End:
 */
