/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <string.h> /* memcmp, strlen */
#include <stdio.h>  /* printf */

#include "hash.h"
#include "misc.h"
#include "mem.h"
#include "fail.h"

bool r_id_eq(const struct r_id *id0, const struct r_id *id1)
{
	return strcmp(id0->id_name, id1->id_name) == 0;
}

/* djb2 hash. */
static uint32_t r_hash(const char *id)
{
	uint32_t h;
	int      ch;

	h = 5381;
	while ((ch = *id++))
		h = ((h << 5) + h) ^ ch;
        return h;
}

void r_hash_init(struct r_hash_table *ht, uint32_t order)
{
	R_PRE(order < 32);
	ht->ht_chain = r_alloc((1 << order) * sizeof ht->ht_chain[0]);
	ht->ht_order = order;
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
	return r_hash(id->id_name) & ((1 << ht->ht_order) - 1);
}

static struct r_hash_link *r_hash_lookup_id(struct r_hash_table *ht, 
					    struct r_id *rid)
{
	uint32_t            hash;
	struct r_hash_link *scan;

	hash = r_table_hash(ht, rid);
	for (scan = ht->ht_chain[hash]; scan != NULL; scan = scan->hl_next) {
		if (r_id_eq(rid, &scan->hl_id))
			return scan;
	}
	return NULL;
}

struct r_hash_link *r_hash_lookup(struct r_hash_table *ht, const char *id)
{
	struct r_id rid;
	R_PRE(strlen(id) < sizeof_array(rid.id_name));

	strcpy(rid.id_name, id);
	return r_hash_lookup_id(ht, &rid);
}

void r_hash_add(struct r_hash_table *ht, struct r_hash_link *link)
{
	uint32_t hash;

	R_PRE(r_hash_lookup_id(ht, &link->hl_id) == NULL);
	R_PRE(link->hl_next == NULL);

	hash = r_table_hash(ht, &link->hl_id);
	link->hl_next = ht->ht_chain[hash];
	ht->ht_chain[hash] = link;

	R_POST(r_hash_lookup_id(ht, &link->hl_id) == link);
}

void r_hash_del(struct r_hash_table *ht, struct r_hash_link *link)
{
	uint32_t            hash;
	struct r_hash_link *scan;

	R_PRE(r_hash_lookup_id(ht, &link->hl_id) == link);

	hash = r_table_hash(ht, &link->hl_id);
	for (scan = ht->ht_chain[hash]; scan != NULL; scan = scan->hl_next) {
		if (scan->hl_next == link) {
			scan->hl_next = link->hl_next;
			break;
		}
	}

	R_POST(r_hash_lookup_id(ht, &link->hl_id) == NULL);
}

void r_hash_stats(const struct r_hash_table *ht)
{
	uint32_t            i;
	uint32_t            t;
	struct r_hash_link *scan;

	for (t = i = 0; i < (1 << ht->ht_order); ++i) {
		uint32_t j;

		for (j = 0, scan = ht->ht_chain[i]; scan != NULL; 
		     scan = scan->hl_next, ++j, ++t) {
			;
		}
		printf("%4i ", j);
		if ((i % 10) == 0)
			printf("\n");
	}
	printf("\ntotal: %i", t);
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
