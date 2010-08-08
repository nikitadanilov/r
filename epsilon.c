/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include "epsilon.h"
#include "fail.h"
#include "misc.h"
#include "mem.h"

struct eptr {
	struct r_ptr  sp_ptr;
	/** list of elements X, such that (X, this-element) is in this
	    relation. */
	struct r_list sp_left;
	uint32_t      sp_left_nr;
	/** list of elements X, such that (this-element, X) is in this
	    relation. */
	struct r_list sp_right;
	uint32_t      sp_right_nr;
};

struct eduo {
	struct r_duo   b_duo;
	struct r_link  b_left_linkage;
	struct r_link  b_right_linkage;
};

static const struct r_rel_ops eps_rel_ops;
static const struct r_ptr_ops eps_ptr_ops;

static bool eps_left_related(const struct eptr *ptr, const struct r_ent *ent);
static bool eps_right_related(const struct eptr *ptr, const struct r_ent *ent);

static struct eptr *get_eptr(const struct r_ptr *ptr)
{
	return outer(ptr, struct eptr, sp_ptr);
}

static struct eptr *get_repr(const struct r_ent *ent, const struct r_rel *rel)
{
	return get_eptr(r_ptr_find(ent, rel));
}

static bool r_eps_invariant(const struct r_eps_rel *er, 
			    const struct r_ent *ent)
{
	struct eptr  *ep;
	struct eduo  *link;

	ep = get_repr(ent, &er->er_rel);
	if (ep == NULL)
		return false;

	if (ep->sp_left_nr != r_list_length(&ep->sp_left) ||
	    ep->sp_right_nr != r_list_length(&ep->sp_right))
		return false;

	r_list_for(&ep->sp_right, link, b_right_linkage) {
		if (!eps_left_related(get_eptr(link->b_duo.d_right), ent))
			return false;
	}
	r_list_for(&ep->sp_left, link, b_left_linkage) {
		if (!eps_right_related(get_eptr(link->b_duo.d_left), ent))
			return false;
	}
	return true;
}

void r_eps_rel_init(struct r_eps_rel *er)
{
	r_rel_init(&er->er_rel);
	er->er_rel.r_ops = &eps_rel_ops;
}

void r_eps_rel_fini(struct r_eps_rel *er)
{
	r_rel_fini(&er->er_rel);
}

void r_eps_add(struct r_eps_rel *er, struct r_ent *a, struct r_ent *b)
{
	struct eptr  *A;
	struct eptr  *B;
	struct eduo  *link;
	struct r_rel *r = &er->er_rel;

	A = get_repr(a, r);
	B = get_repr(b, r);

	R_PRE(A != NULL);
	R_PRE(B != NULL);
	R_PRE(!r_ents_are_in(&er->er_rel, a, b));

	R_INVARIANT(r_eps_invariant(er, a));
	R_INVARIANT(r_eps_invariant(er, b));

	link = r_alloc(sizeof *link);
	r_duo_init(&link->b_duo);
	link->b_duo.d_left  = &A->sp_ptr;
	link->b_duo.d_right = &B->sp_ptr;
	r_list_add(&A->sp_right, &link->b_right_linkage);
	r_list_add(&B->sp_left, &link->b_left_linkage);
	A->sp_right_nr++;
	B->sp_left_nr++;

	R_INVARIANT(r_eps_invariant(er, a));
	R_INVARIANT(r_eps_invariant(er, b));

	R_POST(r_ents_are_in(&er->er_rel, a, b));
}

static int eps_ent_add(struct r_rel *rel, struct r_ent *ent, 
		       struct r_ptr **out)
{
	struct eptr *ep;

	ep = r_alloc(sizeof *ep);
	r_ptr_init(&ep->sp_ptr);
	r_list_init(&ep->sp_left);
	r_list_init(&ep->sp_right);
	ep->sp_ptr.p_ops = &eps_ptr_ops;
	*out = &ep->sp_ptr;
	return 0;
}

static bool eps_right_related(const struct eptr *ptr, const struct r_ent *ent)
{
	struct eduo  *link;
	struct r_ptr *left;
	struct r_ptr *right;

	r_list_for(&ptr->sp_right, link, b_right_linkage) {
		left  = link->b_duo.d_left;
		right = link->b_duo.d_right;
		R_ASSERT(left == &ptr->sp_ptr);
		R_ASSERT(right->p_rel == ptr->sp_ptr.p_rel);
		if (right->p_ent == ent)
			return true;
	}
	return false;
}

static bool eps_left_related(const struct eptr *ptr, const struct r_ent *ent)
{
	struct eduo  *link;
	struct r_ptr *left;
	struct r_ptr *right;

	r_list_for(&ptr->sp_left, link, b_left_linkage) {
		right = link->b_duo.d_right;
		left  = link->b_duo.d_left;
		R_ASSERT(right == &ptr->sp_ptr);
		R_ASSERT(left->p_rel == ptr->sp_ptr.p_rel);
		if (left->p_ent == ent)
			return true;
	}
	return false;
}

static bool eps_ptr_are_in(struct r_rel *rel,
			   struct r_ptr *p0, struct r_ptr *p1)
{
	struct eptr      *e0;
	struct eptr      *e1;
	bool              related;
	struct r_eps_rel *er;

	R_PRE(p0->p_rel == rel);
	R_PRE(p1->p_rel == rel);

	e0 = get_eptr(p0);
	e1 = get_eptr(p1);

	er = outer(rel, struct r_eps_rel, er_rel);
	R_INVARIANT(r_eps_invariant(er, p0->p_ent));
	R_INVARIANT(r_eps_invariant(er, p1->p_ent));

	if (e0->sp_right_nr < e1->sp_left_nr)
		related = eps_right_related(e0, p1->p_ent);
	else
		related = eps_left_related(e1, p0->p_ent);

	R_INVARIANT(r_eps_invariant(er, p0->p_ent));
	R_INVARIANT(r_eps_invariant(er, p1->p_ent));

	return related;
}

static void eps_ptr_free(struct r_ptr *ptr)
{
}

struct r_duo *eps_ptr_right(struct r_ptr *ptr, uint32_t nr)
{
	struct eduo *link;

	link = outer(r_list_at(&get_eptr(ptr)->sp_right, nr), 
		     struct eduo, b_right_linkage);
	return link != NULL ? &link->b_duo : NULL;
}

struct r_duo *eps_ptr_left(struct r_ptr *ptr, uint32_t nr)
{
	struct eduo *link;

	link = outer(r_list_at(&get_eptr(ptr)->sp_left, nr), 
		     struct eduo, b_left_linkage);
	return link != NULL ? &link->b_duo : NULL;
}

static const struct r_rel_ops eps_rel_ops = {
	.ro_ent_add = eps_ent_add,
	.ro_ptr_are_in = eps_ptr_are_in
};

static const struct r_ptr_ops eps_ptr_ops = {
	.po_free  = eps_ptr_free,
	.po_right = eps_ptr_right,
	.po_left  = eps_ptr_left
};

/* 
 *  Local variables:
 *  c-indentation-style: "K&R"
 *  c-basic-offset: 8
 *  tab-width: 8
 *  fill-column: 80
 *  scroll-step: 1
 *  End:
 */