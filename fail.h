/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#ifndef __R_FAIL_H__
#define __R_FAIL_H__

void r_panic(const char *fmt, 
	     const char *file, int lineno, const char *func, ...) 
	__attribute__((noreturn, format(printf, 1, 5)));

#define R_PANIC(str) r_panic(str, __FILE__, __LINE__, __func__)
#define R_FAIL(cond, str) ((cond) ? ((void)0) : R_PANIC(str))
#define R_ASSERT(cond) R_FAIL(cond, "Assertion " #cond " failed")
#define R_PRE(cond) R_FAIL(cond, "Pre-condition " #cond " failed")
#define R_POST(cond) R_FAIL(cond, "Post-condition " #cond " failed")
#define R_INVARIANT(cond) R_FAIL(cond, "Invariant " #cond " violated")

/* __R_FAIL_H__ */
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
