#ifndef ELFDBG_ELF_H
#define ELFDBG_ELF_H 1

#include <elf.h>
#include <sys/stat.h>

#include "compat.h"

typedef struct {
	int fd; /* file descriptor */
	struct stat sb; /* file stat */
	char *mm; /* memory mapping */
	char *strtab; /* string table */
	size_t strtab_size; /* string table size */
	Elf_Ehdr *ehdr; /* header pointer */
	size_t ehdr_size; /* header size */
	Elf_Shdr *shdr; /* section header pointer */
	size_t shdr_size; /* section header size */
} Elf_Obj;

Elf_Obj *elf_init(const char *);
int elf_destroy(Elf_Obj *);
Elf_Shdr *elf_strtab(Elf_Obj *);
char *elf_str_get(Elf_Obj *, size_t);
unsigned int elf_debug_count(Elf_Obj *);
void elf_debug_print(Elf_Obj *);

#endif /* ELFDBG_ELF_H */
