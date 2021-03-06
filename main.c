/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <stdlib.h> /* NULL */
#include <stdio.h> /* printf */

#include "r.h"
#include "meta.h"
#include "misc.h"
#include "fail.h"
#include "file.h"
#include "epsilon.h"

static void print_in(const struct r_rel *rel, const struct r_ent *ent)
{
	struct r_duo *duo;
	struct r_ptr *ptr;
	uint32_t i;

	ptr = r_ptr_find(ent, rel);
	R_PRE(ptr != NULL);
	printf("%s in %s = right: { ", r_name(ent), r_name(&rel->r_ent));
	for (i = 0; ; i++) {
		duo = ptr->p_ops->po_right(ptr, i);
		if (duo != NULL) {
			R_ASSERT(duo->d_left == ptr);
			printf("\"%s\" ", r_name(duo->d_right->p_ent));
		} else
			break;
	}
	printf(" } :: left: { ");
	for (i = 0; ; i++) {
		duo = ptr->p_ops->po_left(ptr, i);
		if (duo != NULL) {
			R_ASSERT(duo->d_right == ptr);
			printf("\"%s\" ", r_name(duo->d_left->p_ent));
		} else
			break;
	}
	printf("}\n");
}

static void print(const struct r_ent *ent)
{
	struct r_ptr *ptr;

	r_list_for(&ent->e_ptr, ptr, p_linkage)
		print_in(ptr->p_rel, ent);
}

int main(int argc, char **argv)
{
	struct r_eps_rel in;
	struct r_ent *A;
	struct r_ent *B;
	int result;

	r_init();

	r_eps_rel_init(&in, "IN", r_name_make("-in-"));

	A = r_ent_find("haha", r_name_make("A"));
	B = r_ent_find("hehe", r_name_make("B"));
	R_ASSERT(A != NULL);
	R_ASSERT(B != NULL);
	R_ASSERT(A != B);

	r_ent_add(A, &in.er_rel);
	r_ent_add(B, &in.er_rel);
	r_eps_add(&in, A, B);
	r_eps_add(&in, A, A);

	R_ASSERT(r_ents_are_in(&in.er_rel, A, B));

	print(A);
	print(B);
	print(&in.er_rel.r_ent);
	print(&r_meta_rel.er_rel.r_ent);

	result = file_rel_build(argv[1], &in);
	R_ASSERT(result == 0);

	r_eps_ptr_iter(&in, LAMBDA(bool, (const struct r_ptr *ptr) {
				printf("%s\n", r_name(&ptr->p_self));
				return false;
			}));
	r_eps_duo_iter(&in, LAMBDA(bool, (const struct r_duo *duo) {
				printf("%s-[%s]->%s\n", 
				       r_name(duo->d_left->p_ent),
				       r_name(&duo->d_ent), 
				       r_name(duo->d_right->p_ent));
				return false;
			}));

	// r_eps_rel_fini(&in);
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
