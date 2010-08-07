/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <stdlib.h> /* malloc, free, NULL */
#include <string.h> /* memset */

#include "mem.h"
#include "fail.h"

void *r_alloc(uint32_t size)
{
	void *p;

	p = malloc(size);
	R_ASSERT(p != NULL);
	memset(p, 0, size);
	return p;
}

void r_free(void *ptr)
{
	free(ptr);
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
