/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_EPSILON_H__
#define __R_EPSILON_H__

#include "r.h"
#include "list.h"

struct r_eps_rel {
	struct r_rel  er_rel;
	struct r_list er_ptr;
	struct r_list er_duo;
};

void r_eps_rel_init(struct r_eps_rel *er, char *name);
void r_eps_rel_fini(struct r_eps_rel *er);

struct r_duo *r_eps_add(struct r_eps_rel *er, 
			struct r_ent *a, struct r_ent *b);

void r_eps_ptr_iter(const struct r_eps_rel *er, 
		    bool (*f)(const struct r_ptr *));
void r_eps_duo_iter(const struct r_eps_rel *er, 
		    bool (*f)(const struct r_duo *));

/* __R_EPSILON_H__ */
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
