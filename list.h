/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_LIST_H__
#define __R_LIST_H__

#include <stdbool.h>
#include <stdint.h>

struct r_list;
struct r_link;

struct r_list {
	struct r_link *ls_head;
	struct r_link *ls_tail;
};

struct r_link {
	struct r_link *ln_next;
	struct r_link *ln_prev;
};

void r_list_init(struct r_list *list);
void r_list_fini(struct r_list *list);

void r_link_init(struct r_link *link);
void r_link_fini(struct r_link *link);

void r_list_add(struct r_list *list, struct r_link *link);
void r_list_add_tail(struct r_list *list, struct r_link *link);
void r_list_move(struct r_list *list, struct r_link *link);
void r_list_move_tail(struct r_list *list, struct r_link *link);
void r_link_del(struct r_link *link);

bool r_list_contains(const struct r_list *list, const struct r_link *link);
uint32_t r_list_length(const struct r_list *list);

struct r_link *r_list_at(const struct r_list *list, uint32_t nr);

bool r_list_is_empty(const struct r_list *list);
bool r_link_is_linked(const struct r_link *link);

bool r_list_invariant(const struct r_list *list);
bool r_link_invariant(const struct r_link *link);

void r_list_iterate(struct r_list *list, bool (*f)(struct r_link *link));
void r_list_iterate_const(const struct r_list *list, 
			  bool (*f)(const struct r_link *link));

#define R_HEAD(list, ptr, field)			\
container_of((list)->ls_head, typeof(*(ptr)), field)

#define R_NEXT(ptr, field)					\
container_of((ptr)->field.ln_next, typeof(*(ptr)), field)

#define r_list_for(list, scan, field)					\
for (scan = R_HEAD(list, scan, field);					\
     &scan->field != (void *)(list); scan = R_NEXT(scan, field))

#define r_list_for_mod(list, scan, next, field)				\
for (scan = R_HEAD(list, scan, field), next = R_NEXT(scan, field);	\
     &scan->field != (void *)(list); scan = next, next = R_NEXT(next, field))

/* __R_LIST_H__ */
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
