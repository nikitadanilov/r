/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include "fail.h"
#include "misc.h"
#include "list.h"

void r_list_init(struct r_list *list)
{
	list->ls_head = list->ls_tail = (void *)list;
	R_POST(r_list_is_empty(list));
}

void r_list_fini(struct r_list *list)
{
	R_PRE(r_list_is_empty(list));
}

void r_link_init(struct r_link *link)
{
	link->ln_next = link->ln_prev = link;
	R_POST(!r_link_is_linked(link));
}

void r_link_fini(struct r_link *link)
{
	R_PRE(!r_link_is_linked(link));
}

static void r_list_add_between(struct r_link *link,
			       struct r_link *prev, struct r_link *next)
{
	R_PRE(prev->ln_next == next);
	R_PRE(next->ln_prev == prev);
	R_PRE(!r_link_is_linked(link));

	next->ln_prev = link;
	link->ln_next = next;
	link->ln_prev = prev;
	prev->ln_next = link;
}
			       
void r_list_add(struct r_list *list, struct r_link *link)
{
	R_PRE(!r_list_contains(list, link));
	r_list_add_between(link, (void *)list, list->ls_head);
	R_POST(r_list_contains(list, link));
}

void r_list_add_tail(struct r_list *list, struct r_link *link)
{
	R_PRE(!r_list_contains(list, link));
	r_list_add_between(link, list->ls_tail, (void *)list);
	R_POST(r_list_contains(list, link));
}

void r_list_move(struct r_list *list, struct r_link *link)
{
	R_PRE(r_list_contains(list, link));
	r_link_del(link);
	r_list_add(list, link);
	R_POST(r_list_contains(list, link));
}

void r_list_move_tail(struct r_list *list, struct r_link *link)
{
	R_PRE(r_list_contains(list, link));
	r_link_del(link);
	r_list_add_tail(list, link);
	R_POST(r_list_contains(list, link));
}

void r_link_del(struct r_link *link)
{
	R_INVARIANT(r_link_invariant(link));
	link->ln_next->ln_prev = link->ln_prev;
	link->ln_prev->ln_next = link->ln_next;
	r_link_init(link);
	R_INVARIANT(r_link_invariant(link));
}

bool r_list_contains(const struct r_list *list, const struct r_link *link)
{
	bool gotit;

	gotit = false;
	r_list_iterate_const(list, LAMBDA(bool, (const struct r_link *scan) {
				return gotit = (link == scan);
			} ));
	return gotit;
}

uint32_t r_list_length(const struct r_list *list)
{
	uint32_t length;

	length = 0;
	r_list_iterate_const(list, LAMBDA(bool, (const struct r_link *scan) {
				length++;
				return false;
			} ));
	return length;
}

bool r_list_is_empty(const struct r_list *list)
{
	R_INVARIANT(r_list_invariant(list));
	return list->ls_head == (void *)list;
}

bool r_link_is_linked(const struct r_link *link)
{
	return link->ln_next != link && link->ln_next != NULL;
}

bool r_link_invariant(const struct r_link *link)
{
	struct r_link *scan;

	if ((link->ln_next == link) != (link->ln_prev == link))
		return false;

	for (scan = link->ln_next; scan != link; scan = scan->ln_next) {
		if (scan->ln_next->ln_prev != scan ||
		    scan->ln_prev->ln_next != scan)
			return false;
	}
	return true;
}

bool r_list_invariant(const struct r_list *list)
{
	return r_link_invariant((void *)list);
}

void r_list_iterate(struct r_list *list, bool (*f)(struct r_link *link))
{
	struct r_link *scan;
	struct r_link *next;

	R_INVARIANT(r_list_invariant(list));
	for (scan = list->ls_head; scan != (void *)list; scan = next) {
		next = scan->ln_next;
		if (f(scan))
			break;
	}
	R_INVARIANT(r_list_invariant(list));
}

void r_list_iterate_const(const struct r_list *list, 
			  bool (*f)(const struct r_link *link))
{
	struct r_link *scan;

	R_INVARIANT(r_list_invariant(list));
	for (scan = list->ls_head; scan != (void *)list; scan = scan->ln_next) {
		if (f(scan))
			break;
	}
	R_INVARIANT(r_list_invariant(list));
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
