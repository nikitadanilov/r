/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_MISC_H__
#define __R_MISC_H__

#define ergo(a, b) (!(a) || (b))
#define sizeof_array(a) (sizeof(a)/sizeof((a)[0]))
#define container_of(ptr, type, field) \
        ((type *)((char *)(ptr) - (char *)(&((type *)0)->field)))
#define outer(ptr, type, field) \
	((ptr) != 0 ? container_of(ptr, type, field) : (type *)0)

#define LAMBDA(t, ...) ({ t lambda __VA_ARGS__ ; &lambda; })

char *r_name_make(const char *fmt, ...)
	__attribute__((format(printf, 1, 2)));

/* __R_MISC_H__ */
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
