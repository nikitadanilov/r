/* -*- C -*- */
/* This file is part of R. See the end of the file for the prominent
   copyright and licensing information. */

#include <fts.h>
#include <stdio.h>  /* sprintf */
#include <errno.h>  /* errno */
#include <stddef.h> /* NULL */
#include <sys/stat.h>   /* struct stat */

#include "fail.h"
#include "misc.h" /* r_name_make */
#include "epsilon.h"

int file_rel_build(char *path, struct r_eps_rel *rel)
{
	FTS          *iter;
	FTSENT       *el;
	FTSENT       *pel;
	int           result;
	char         *argv[] = { [0] = path, [1] = NULL };
	struct r_id   id;
	struct r_id   pid;
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
		if (result != 0)
			break;
		sprintf(id.id_name, "%06lu", 
			(long unsigned)el->fts_statp->st_ino);
		ent = r_ent_find(&id, r_name_make("%s", id.id_name));
		R_ASSERT(ent != NULL);
		r_ent_add(ent, r);
		if (el->fts_level > 0) {
			struct r_duo *duo;

			pel = el->fts_parent;
			sprintf(pid.id_name, "%06lu", 
				(long unsigned)pel->fts_statp->st_ino);
			pent = r_ent_find(&pid, 
					  r_name_make("%s", pid.id_name));
			R_ASSERT(pent != NULL);
			r_ent_add(pent, r);
			duo = r_eps_add(rel, ent, pent);
			r_name_set(&duo->d_ent, 
				   r_name_make("%s", el->fts_name));
		}
	}
	fts_close(iter);
	result = result ? : -errno;
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
