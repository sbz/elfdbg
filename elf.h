#include <elf.h>
#include <sys/stat.h>

typedef struct Elf_Obj {
    int         fd; /* file descriptor */
    struct stat sb; /* file stat */
    char        *mm; /* memory mapping */
    char        *strtab; /* string table */
    size_t      strtab_size; /* string table size */
    Elf_Ehdr    *ehdr; /* header pointer */
    size_t      ehdr_size; /* header size */
    Elf_Shdr    *shdr; /* section header pointer */
    size_t      shdr_size; /* section header size */
} Elf_Obj;

Elf_Obj *elf_init(const char *);
int elf_destroy(Elf_Obj *);
Elf_Shdr *elf_strtab(Elf_Obj *);
char *elf_str_get(Elf_Obj *, int);
int elf_debug(Elf_Obj *);
void elf_debug_print(Elf_Obj *);
