/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <fts.h>
#include <stdio.h>  /* sprintf, fprintf */
#include <errno.h>  /* errno */
#include <string.h> /* strerror */
#include <stddef.h> /* NULL */
#include <sys/stat.h>   /* struct stat */

#include "fail.h"
#include "misc.h" /* r_name_make */
#include "epsilon.h"

static char *ftsent_name(const FTSENT *el)
{
	return r_name_make("%06lx:%06lu",
			   (long unsigned)el->fts_statp->st_dev,
			   (long unsigned)el->fts_statp->st_ino);

}

int file_rel_build(char *path, struct r_eps_rel *rel)
{
	FTS          *iter;
	FTSENT       *el;
	int           result;
	char         *argv[] = { [0] = path, [1] = NULL };
	char         *id;
	char         *pid;
	struct r_ent *ent;
	struct r_ent *pent;
	struct r_rel *r;	

	iter = fts_open(argv, FTS_LOGICAL, NULL);
	R_ASSERT(iter != NULL);

	r = &rel->er_rel;
	result = 0;
	while ((el = fts_read(iter)) != NULL) {
		switch (el->fts_info) {
		case FTS_DOT:
		case FTS_DP:
			continue;
		case FTS_ERR:
		case FTS_NS:
		case FTS_DNR:
			result = -el->fts_errno;
			break;
		case FTS_NSOK:
			result = -EIO;
			break;
		case FTS_DC:
			result = -ELOOP;
			break;
		}
		if (result != 0) {
			if (result == -ELOOP) {
				result = fts_set(iter, el, FTS_SKIP);
				R_ASSERT(result == 0);
			} else
				fprintf(stderr, "%s: \"%s\"\n", 
					strerror(-result), el->fts_path);
			continue;
		}
		id = ftsent_name(el);
		ent = r_ent_find(id, id);
		R_ASSERT(ent != NULL);
		r_ent_add(ent, r);
		if (el->fts_level > 0) {
			struct r_duo *duo;

			pid = ftsent_name(el->fts_parent);
			pent = r_ent_find(pid, pid);
			R_ASSERT(pent != NULL);
			r_ent_add(pent, r);
			duo = r_eps_add(rel, ent, pent);
			r_name_set(&duo->d_ent, 
				   r_name_make("%s", el->fts_name));
		}
	}
	fts_close(iter);
	result = result ? : -errno;
	fprintf(stderr, "%s: %i\n", __func__, result);
	return result;
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
