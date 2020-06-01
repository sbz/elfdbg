/*-
 * Copyright (c) 2015 Sofian Brabez <sbz@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <err.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "elf.h"

extern char *__progname;

static void
usage(void)
{
	fprintf(stderr, "Usage: %s [-qv] file\n", __progname);
	exit(EX_USAGE);
}

int
main(int argc, char *argv[])
{
	int rc, has_debug;
	int ch, qflag = 0, vflag = 0;
	Elf_Obj *e = NULL;
	Elf_Shdr *shstr = NULL;

	if (argc == 1)
		usage();

	while ((ch = getopt(argc, argv, "qhv")) != -1) {
		switch (ch) {
		case 'q':
			qflag = 1;
			break;
		case 'v':
			vflag = 1;
			break;
		case 'h':
		default:
			usage();
		}
	}

	argc -= optind;
	argv += optind;

	/* load elf binary in memory */
	e = elf_init(*argv);
	if (e == NULL)
		errx(EX_DATAERR, "elf_init");

	/* load string stable */
	shstr = elf_strtab(e);

	/* search for sections name with debug prefix */
	has_debug = elf_debug(e);

	if (!qflag)
		printf("%s\n", (has_debug > 0) ? "HAS DEBUG" : "NO DEBUG");

	if (vflag)
		elf_debug_print(e);

	rc = elf_destroy(e);

	if (qflag) {
		rc = has_debug > 0;
	}

	return (rc);
}
