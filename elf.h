#include <elf.h>
#include <sys/stat.h>

typedef struct Elf_Obj {
    int         fd;
    struct stat sb;
    char        *mm;
    char        *strtab;
    size_t      strtab_size;
    Elf_Ehdr    *ehdr;
    size_t      ehdr_size;
    Elf_Shdr    *shdr;
    size_t      shdr_size;
} Elf_Obj;

Elf_Obj *elf_init(const char *filename);
int elf_destroy(Elf_Obj *);
Elf_Shdr *elf_strtab(Elf_Obj *);
int elf_debug(Elf_Obj *);
