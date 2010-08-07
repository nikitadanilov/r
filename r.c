/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <stdlib.h> /* NULL */

#include "hash.h"
#include "fail.h"
#include "misc.h"
#include "mem.h"
#include "r.h"

static struct r_hash_table ent_hash;

struct r_ent *r_ent_find(const struct r_id *id)
{
	struct r_ent       *ent;
	struct r_hash_link *link;

	link = r_hash_lookup(&ent_hash, id);
	if (link == NULL) {
		ent = r_alloc(sizeof *ent);
		link = &ent->e_linkage;
		link->hl_id = *id;
		r_hash_add(&ent_hash, link);
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
		ent->e_ops->eo_free(ent);
		r_hash_del(&ent_hash, &ent->e_linkage);
		r_free(ent);
	}
}

void r_init(void)
{
	r_hash_init(&ent_hash, 14);
}

void r_fini(void)
{
	r_hash_fini(&ent_hash);
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
