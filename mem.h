/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_MEM_H__
#define __R_MEM_H__

#include <stdint.h>

void *r_alloc(uint32_t size) __attribute__((malloc));
void  r_free(void *ptr);

/* __R_MEM_H__ */
#endif

/* 
 *  Local variables:
 *  c-indentation-style: "K&R"
 *  c-basic-offset: 8
 *  tab-width: 8
 *  fill-column: 80
 *  scroll-step: 1
 *  End:
 */
