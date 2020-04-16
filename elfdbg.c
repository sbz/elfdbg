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
#include <fcntl.h>
#include <gelf.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

extern char *__progname;

static int
elf_debug_sections(Elf *e)
{
	Elf_Scn *scn = NULL;
	GElf_Shdr shdr;
	size_t n, shstrndx, sz;
	char *name;
	int has_debug = 0;

	if (elf_getshdrstrndx(e, &shstrndx) != 0)
		errx(EX_SOFTWARE, "elf_getshdrstrndx() failed : %s . ",
		    elf_errmsg(-1));

	while ((scn = elf_nextscn(e, scn)) != NULL) {
		gelf_getshdr(scn, &shdr);

		name = elf_strptr(e, shstrndx, shdr.sh_name);
		if (!strstr(name, "debug_"))
			continue;

		has_debug++;
	}

	return (has_debug > 0);
}

static void
usage(void) {
	fprintf(stderr, "Usage: %s file\n", __progname);
	exit(EX_USAGE);
}

int
main(int argc, char *argv[])
{
	const char *filename;
	int fd, rc;
	Elf *e;
	int has_debug;

	if (argc == 1)
		usage();

	filename = argv[1];

	if (elf_version(EV_CURRENT) == EV_NONE)
		errx(EX_SOFTWARE, "ELF library initialization failed : %s ",
		    elf_errmsg(-1));

	if ((fd = open(filename, O_RDONLY, 0)) < 0)
		err(EX_NOINPUT, "open %s failed ", filename);
	if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
		errx(EX_SOFTWARE, "elf_begin() failed : %s", elf_errmsg(-1));
	if (elf_kind(e) != ELF_K_ELF)
		errx(EX_DATAERR, "%s is not an ELF object", filename);

	has_debug = elf_debug_sections(e);
	printf(has_debug ? "HAS DEBUG\n" : "NO DEBUG\n");

	rc = close(fd);
	rc = elf_end(e);

	return (rc);
}
