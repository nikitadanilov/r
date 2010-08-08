/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#define _GNU_SOURCE 1 /* asprintf */
#include <stdio.h> /* asprintf */
#include <stdarg.h>

#include "misc.h"
#include "fail.h"

char *r_name_make(const char *fmt, ...)
{
	va_list args;
	char   *ret;
	int     nob;

	va_start(args, fmt);
	nob = vasprintf(&ret, fmt, args); 
	R_ASSERT(nob >= 0);
	va_end(args);
	return ret;
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
