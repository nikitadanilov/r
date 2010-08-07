/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_HASH_H__
#define __R_HASH_H__

#include <stdint.h>
#include <stdbool.h>

struct r_id;
struct r_hash_link;

enum {
	R_ID_SIZE = 64
};

struct r_id {
	char id_name[R_ID_SIZE];
};

bool r_id_eq(const struct r_id *id0, const struct r_id *id1);

struct r_hash_link {
	struct r_id         hl_id;
	struct r_hash_link *hl_next;
};

struct r_hash_table {
	uint32_t             ht_order;
	struct r_hash_link **ht_chain;
};

void r_hash_init(struct r_hash_table *ht, uint32_t order);
void r_hash_fini(struct r_hash_table *ht);

/**
   @post ergo(result != NULL, r_id_eq(&result->hl_id, id))
 */
struct r_hash_link *r_hash_lookup(struct r_hash_table *ht, 
				  const struct r_id *id);

/**
	@pre r_hash_lookup(ht, &link->hl_id) == NULL
	@post r_hash_lookup(ht, &link->hl_id) == link
 */
void r_hash_add(struct r_hash_table *ht, struct r_hash_link *link);

/**
	@pre r_hash_lookup(ht, &link->hl_id) == link
	@post r_hash_lookup(ht, &link->hl_id) == NULL
 */
void r_hash_del(struct r_hash_table *ht, struct r_hash_link *link);

/* __R_HASH_H__ */
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
