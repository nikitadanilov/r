/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <string.h> /* memcmp */

#include "hash.h"
#include "misc.h"
#include "mem.h"
#include "fail.h"

bool r_id_eq(const struct r_id *id0, const struct r_id *id1)
{
	return memcmp(id0->id_name, id1->id_name, sizeof id1->id_name) == 0;
}

static uint32_t r_hash(const struct r_id *id)
{
	uint32_t h;
	int      i;

	for (h = 0, i = 0; i < sizeof_array(id->id_name); ++i) {
		h += id->id_name[i] * 11;
		h ^= h << 1;
	} 
	return h;
}

void r_hash_init(struct r_hash_table *ht, uint32_t order)
{
	R_PRE(order < 32);
	ht->ht_chain = r_alloc(1 << order);
}

void r_hash_fini(struct r_hash_table *ht)
{
	uint32_t i;

	if (ht->ht_chain != NULL) {
		for (i = 0; i < (1 << ht->ht_order); ++i)
			R_ASSERT(ht->ht_chain[i] == NULL);
		r_free(ht->ht_chain);
		ht->ht_chain = NULL;
	}
}

static uint32_t r_table_hash(const struct r_hash_table *ht, 
			     const struct r_id *id)
{
	return r_hash(id) & ((1 << ht->ht_order) - 1);
}

struct r_hash_link *r_hash_lookup(struct r_hash_table *ht, 
				  const struct r_id *id)
{
	uint32_t            hash;
	struct r_hash_link *scan;

	hash = r_table_hash(ht, id);
	for (scan = ht->ht_chain[hash]; scan != NULL; scan = scan->hl_next) {
		if (r_id_eq(id, &scan->hl_id))
			return scan;
	}
	return NULL;
}

void r_hash_add(struct r_hash_table *ht, struct r_hash_link *link)
{
	uint32_t hash;

	R_PRE(r_hash_lookup(ht, &link->hl_id) == NULL);

	hash = r_table_hash(ht, &link->hl_id);
	link->hl_next = ht->ht_chain[hash];
	ht->ht_chain[hash] = link;

	R_POST(r_hash_lookup(ht, &link->hl_id) == link);
}

void r_hash_del(struct r_hash_table *ht, struct r_hash_link *link)
{
	uint32_t            hash;
	struct r_hash_link *scan;

	R_PRE(r_hash_lookup(ht, &link->hl_id) == link);

	hash = r_table_hash(ht, &link->hl_id);
	for (scan = ht->ht_chain[hash]; scan != NULL; scan = scan->hl_next) {
		if (scan->hl_next == link) {
			scan->hl_next = link->hl_next;
			break;
		}
	}

	R_POST(r_hash_lookup(ht, &link->hl_id) == NULL);
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
