/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include "meta.h"
#include "epsilon.h"

struct r_eps_rel r_meta_rel;

void r_meta_init(void)
{
	r_eps_rel_init(&r_meta_rel, "-meta-");
}

void r_meta_fini(void)
{
	r_eps_rel_fini(&r_meta_rel);
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
