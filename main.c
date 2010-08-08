/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <stdlib.h> /* NULL */
#include <stdio.h> /* printf */

#include "r.h"
#include "fail.h"
#include "epsilon.h"

static void print(const struct r_rel *rel, const struct r_ent *ent)
{
	struct r_duo *duo;
	struct r_ptr *ptr;
	uint32_t i;

	ptr = r_ptr_find(ent, rel);
	R_PRE(ptr != NULL);
	printf("%s = { ", r_name(ent));
	for (i = 0; ; i++) {
		duo = ptr->p_ops->po_right(ptr, i);
		if (duo != NULL) {
			R_ASSERT(duo->d_left == ptr);
			printf("\"%s\" ", r_name(duo->d_right->p_ent));
		} else
			break;
	}
	printf(" } : } ");
	for (i = 0; ; i++) {
		duo = ptr->p_ops->po_left(ptr, i);
		if (duo != NULL) {
			R_ASSERT(duo->d_right == ptr);
			printf("\"%s\" ", r_name(duo->d_left->p_ent));
		} else
			break;
	}
	printf("{\n");
}

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

	print(&in.er_rel, A);
	print(&in.er_rel, B);
	// r_fini();
	return 0;
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
