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
    Elf_Obj *e;

    fd = open(filename, O_RDONLY);
    e = (Elf_Obj *)malloc(sizeof(Elf_Obj));
    e->fd = fd;
    fstat(fd, &sb);
    e->sb = sb;

    e->mm = mmap(NULL, sb.st_size, PROT_READ,  MAP_SHARED, fd, 0);
    e->ehdr = (Elf_Ehdr *) e->mm;
    e->ehdr_size = e->ehdr->e_ehsize;

    e->shdr = (Elf_Shdr *) (e->mm + e->ehdr->e_shoff);
    e->shdr_size = e->ehdr->e_shnum;

    e->strtab = NULL;
    e->strtab_size = 0;

    return e;
}

Elf_Shdr *
elf_strtab(Elf_Obj *e)
{
    Elf_Shdr *ptr;

    ptr = &e->shdr[e->ehdr->e_shstrndx];
    e->strtab = malloc(ptr->sh_size);
    e->strtab_size = ptr->sh_size;

    memcpy(e->strtab, (e->mm + ptr->sh_offset), ptr->sh_size);

    return ptr;
}

int
elf_destroy(Elf_Obj *e)
{
    if (e == NULL)
        return 1;

    munmap(e->mm, e->sb.st_size);
    close(e->fd);
    if (e->strtab != NULL)
        free(e->strtab);
    free(e);

    return 0;
}

int
elf_debug(Elf_Obj *e) 
{
    int i, has_debug = 0;
    char *debug_prefix = ".debug_";
    char *section_name = NULL;

    for (i=0; i < e->shdr_size; i++) {
        section_name = &e->strtab[e->shdr[i].sh_name];
        if (strnstr(section_name, debug_prefix, strlen(debug_prefix))) {
            //printf("%s\n", section_name);
            has_debug++;
        }

    }

    return has_debug;
}
