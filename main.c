/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <stdlib.h> /* NULL */

#include "r.h"
#include "fail.h"
#include "epsilon.h"

int main(int argc, char **argv)
{
	struct r_eps_rel in;
	struct r_ent *A;
	struct r_ent *B;
	const struct r_id iA = { .id_name = "haha" };
	const struct r_id iB = { .id_name = "hehe" };

	r_init();

	r_eps_rel_init(&in);

	A = r_ent_find(&iA);
	B = r_ent_find(&iB);
	R_ASSERT(A != NULL);
	R_ASSERT(B != NULL);
	R_ASSERT(A != B);

	r_ent_add(A, &in.er_rel);
	r_ent_add(B, &in.er_rel);
	r_eps_add(&in, A, B);

	R_ASSERT(r_ents_are_in(&in.er_rel, A, B));
	r_eps_rel_fini(&in);

	// r_fini();
	return 1;
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
