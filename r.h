/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_R_H__
#define __R_R_H__

struct r_ent;
struct r_rel;
struct r_ent_ops;

enum {
	R_ID_SIZE = 64
};

struct r_id {
	char id_name[R_ID_SIZE];
};

/** R-entity.

    An entity that can be in a relation.

    @see r_rel
 */
struct r_ent {
	struct r_id             ent_id;
	const struct r_ent_ops *ent_ops;
};

/** Relation between entities.

    @see r_ent
 */
struct r_rel {
	/* A relation is itself an entity */
	struct r_ent            rel_ent;
	const struct r_rel_ops *rel_ops;
};

struct r_ent_ops {
};

struct r_rel_ops {
};

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
