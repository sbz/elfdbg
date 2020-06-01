#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "elf.h"

Elf_Obj *
elf_init(const char *filename)
{
	int fd;
	struct stat sb;
	Elf_Obj *e = NULL;

	if ((fd = open(filename, O_RDONLY)) == -1)
		errx(1, "open");

	e = (Elf_Obj *)malloc(sizeof(Elf_Obj));
	if (e == NULL)
		errx(1, "malloc");

	e->fd = fd;

	if (fstat(fd, &sb) == -1)
		errx(1, "fstat");

	e->sb = sb;

	e->mm = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (e->mm == MAP_FAILED)
		errx(1, "mmap");

	e->ehdr = (Elf_Ehdr *)e->mm;
	e->ehdr_size = e->ehdr->e_ehsize;

	if (e->sb.st_size < sizeof(Elf_Ehdr))
		errx(1, "not a ELF object");

	if (memcmp(e->ehdr->e_ident, ELFMAG, sizeof(ELFMAG) - 1) != 0)
		errx(1, "not a ELF object");

	e->shdr = (Elf_Shdr *)(e->mm + e->ehdr->e_shoff);
	e->shdr_size = e->ehdr->e_shnum;

	e->strtab = NULL;
	e->strtab_size = 0;

	return (e);
}

Elf_Shdr *
elf_strtab(Elf_Obj *e)
{
	Elf_Shdr *ptr;

	ptr = &e->shdr[e->ehdr->e_shstrndx];
	e->strtab = malloc(ptr->sh_size);
	if (e->strtab == NULL)
		errx(1, "malloc %s:%d", __func__, __LINE__);

	e->strtab_size = ptr->sh_size;

	memcpy(e->strtab, (e->mm + ptr->sh_offset), ptr->sh_size);

	return (ptr);
}

int
elf_destroy(Elf_Obj *e)
{
	if (e == NULL)
		return (1);

	if (e->mm != NULL)
		if (munmap(e->mm, e->sb.st_size) == -1)
			return (1);

	close(e->fd);

	if (e->strtab != NULL)
		free(e->strtab);

	free(e);

	return (0);
}

char *
elf_str_get(Elf_Obj *e, int index)
{
	if (e == NULL || index > e->strtab_size)
		return (NULL);

	return (&e->strtab[e->shdr[index].sh_name]);
}

int
elf_debug_count(Elf_Obj *e)
{
	int i, has_debug = 0;
	char *debug_prefix = ".debug_";
	char *section_name = NULL;

	for (i = 0; i < e->shdr_size; i++) {
		section_name = elf_str_get(e, i);
		if (strnstr(section_name, debug_prefix, strlen(debug_prefix))) {
			has_debug++;
		}
	}

	return (has_debug);
}

void
elf_debug_print(Elf_Obj *e)
{
	int i;
	char *debug_prefix = ".debug_";
	char *section_name = NULL;

	printf("%d ELF debug sections:\n", elf_debug_count(e));

	for (i = 0; i < e->shdr_size; i++) {
		if (e->shdr[i].sh_type != SHT_PROGBITS)
			continue;

		section_name = elf_str_get(e, i);
		if (strnstr(section_name, debug_prefix, strlen(debug_prefix))) {
			printf("%s\n", section_name);
		}
	}
}
