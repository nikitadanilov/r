/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_R_H__
#define __R_R_H__

#include <stdbool.h>
#include <stdint.h>

#include "hash.h"

struct r_ent;
struct r_rel;
struct r_ptr;

struct r_ent_ops;
struct r_rel_ops;
struct r_ptr_ops;

typedef uint64_t r_refcnt_t;

/** R-entity.

    An entity that can be in a relation.

    @see r_rel
 */
struct r_ent {
	r_refcnt_t              e_ref;
	struct r_hash_link      e_linkage;
	const char             *e_name;
	const struct r_ent_ops *e_ops;
};

/** A relation between entities.

    @see r_ent
 */
struct r_rel {
	/* A relation is itself an entity. */
	struct r_ent            r_ent;
	const struct r_rel_ops *r_ops;
};

/** A relation-entity pointer.

    Pointer describes a position of entity within a relation. Given a pointer,
    the entity pointed to and entities related to the pointed entity (by the
    relation) can be efficiently located.

    Pointer is a relation-specific data structure.
 */
struct r_ptr {
	/** A pointer is an entity. */
	struct r_ent            p_self;
	struct r_rel           *p_rel;
	struct r_ent           *p_ent;
	const struct r_ptr_ops *p_ops;
};

struct r_ent_ops {
	/** A list of pointers to this entity from all relations this entity is
	    in. */
	unsigned (*eo_ptrs)(struct r_ent *ent, unsigned start, unsigned nr,
			    struct r_ptr **out);
	void     (*eo_free)(struct r_ent *ent);
};

struct r_rel_ops {
	/** True iff entities pointed to by p0 and p1 are related by rel. 

	    @pre p0->p_rel == rel && p1->p_rel == rel
	 */
	bool (*ro_ptr_are_in)(struct r_rel *rel, 
			      struct r_ptr *p0, struct r_ptr *p1);
};

struct r_ptr_ops {
	unsigned (*po_U)(struct r_ptr *ptr, unsigned start, unsigned nr,
			 struct r_ptr **out);
};

struct r_ent *r_ent_find(const struct r_id *id);
void r_ent_get(struct r_ent *ent);
void r_ent_put(struct r_ent *ent);

void r_init(void);
void r_fini(void);

/* __R_R_H__ */
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
