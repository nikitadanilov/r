/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <stdlib.h> /* NULL */
#include <string.h> /* memset, strcpy */

//#include <stdio.h>  /* XXX printf */

#include "hash.h"
#include "fail.h"
#include "misc.h"
#include "meta.h"
#include "mem.h"
#include "r.h"

static struct r_hash_table ent_hash;
static struct r_list ent_list;

void r_ent_init(struct r_ent *ent, const char *id, char *name)
{
	R_PRE(strlen(id) < sizeof_array(ent->e_linkage.hl_id.id_name));

	//printf("%s: (%s, %s) -> %p\n", __func__, id, name, ent);

	memset(ent, 0, sizeof *ent);
	r_list_init(&ent->e_ptr);
	ent->e_name = name;
	strcpy(ent->e_linkage.hl_id.id_name, id);
	r_hash_add(&ent_hash, &ent->e_linkage);
	r_list_add(&ent_list, &ent->e_ent_list);
}

void r_ent_fini(struct r_ent *ent)
{
	r_hash_del(&ent_hash, &ent->e_linkage);
	r_free(ent->e_name);
	r_list_fini(&ent->e_ptr);
	r_link_del(&ent->e_ent_list);
}

struct r_ent *r_ent_find(const char *id, char *name)
{
	struct r_ent       *ent;
	struct r_hash_link *link;

	link = r_hash_lookup(&ent_hash, id);
	if (link == NULL) {
		ent = r_alloc(sizeof *ent);
		r_ent_init(ent, id, name);
	} else
		ent = container_of(link, struct r_ent, e_linkage);
	r_ent_get(ent);
	return ent;
}

void r_ent_get(struct r_ent *ent)
{
	ent->e_ref++;
	R_ASSERT(ent->e_ref != 0);
}

void r_ent_put(struct r_ent *ent)
{
	R_PRE(ent->e_ref > 0);
	ent->e_ref--;
	if (ent->e_ref == 0) {
#if 0
		ent->e_ops->eo_free(ent);
		r_hash_del(&ent_hash, &ent->e_linkage);
		r_ent_fini(ent);
		r_free(ent);
#endif
	}
}

int r_ent_add(struct r_ent *ent, struct r_rel *rel)
{
	int           result;
	struct r_ptr *ptr;

	if (r_ptr_find(ent, rel) == NULL) {
		result = rel->r_ops->ro_ent_add(rel, ent, &ptr);
		if (result == 0) {
			r_ptr_add(ptr, ent, rel);
			/* relate ent and rel by meta-relation. */
			r_ent_add(ent, &r_meta_rel.er_rel);
			r_ent_add(&rel->r_ent, &r_meta_rel.er_rel);
			r_eps_add(&r_meta_rel, ent, &rel->r_ent);
		}
	} else
		/* already in relation */
		result = 0;
	return result;
}

bool r_ents_are_in(struct r_rel *rel, struct r_ent *a, struct r_ent *b)
{
	struct r_ptr *A;
	struct r_ptr *B;

	A = r_ptr_find(a, rel);
	B = r_ptr_find(b, rel);

	R_PRE(A != NULL);
	R_PRE(B != NULL);

	return rel->r_ops->ro_ptr_are_in(rel, A, B);
}

const char *r_name(const struct r_ent *ent)
{
	return ent->e_name;
}

void r_name_set(struct r_ent *ent, char *name)
{
	if (ent->e_name != NULL)
		r_free(ent->e_name);
	ent->e_name = name;
}

void r_rel_init(struct r_rel *rel, const char *id, char *name)
{
	memset(rel, 0, sizeof *rel);
	r_ent_init(&rel->r_ent, id, name);
}

void r_rel_fini(struct r_rel *rel)
{
	r_ent_fini(&rel->r_ent);
}

void r_ptr_init(struct r_ptr *ptr, const char *id, char *name)
{
	memset(ptr, 0, sizeof *ptr);
	r_ent_init(&ptr->p_self, id, name);
	r_link_init(&ptr->p_linkage);
}

void r_ptr_fini(struct r_ptr *ptr)
{
	r_link_fini(&ptr->p_linkage);
	r_ent_fini(&ptr->p_self);
}

struct r_ptr *r_ptr_find(const struct r_ent *ent, const struct r_rel *rel)
{
	struct r_ptr *ptr;

	r_list_for(&ent->e_ptr, ptr, p_linkage) {
		if (ptr->p_rel == rel)
			return ptr;
	}
	return NULL;
}

void r_ptr_add(struct r_ptr *ptr, struct r_ent *ent, struct r_rel *rel)
{
	ptr->p_ent = ent;
	ptr->p_rel = rel;
	r_ent_get(&rel->r_ent);
	r_list_add(&ent->e_ptr, &ptr->p_linkage);
}

void r_ptr_del(struct r_ptr *ptr)
{
	ptr->p_ops->po_free(ptr);
	r_link_del(&ptr->p_linkage);
	r_ent_put(&ptr->p_rel->r_ent);
}

void r_duo_init(struct r_duo *duo, const char *id, char *name)
{
	r_ent_init(&duo->d_ent, id, name);
}

void r_duo_fini(struct r_duo *duo)
{
	r_ent_fini(&duo->d_ent);
}

void r_init(void)
{
	r_list_init(&ent_list);
	r_hash_init(&ent_hash, 14);
	r_meta_init();
}

void r_fini(void)
{
	r_hash_stats(&ent_hash);
	r_meta_fini();
	r_hash_fini(&ent_hash);
	r_list_fini(&ent_list);
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
