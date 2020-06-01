#ifndef ELFDBG_COMPAT_H
#define ELFDBG_COMPAT_H 1

/* Linux does not define the generic typedef arch independant */
#ifdef __linux__
#ifdef __x86_64__
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Shdr Elf_Shdr;
#else
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Shdr Elf_Shdr;
#endif

/* Linux does not define strnstr() function */
char *strnstr(const char *, const char *, size_t);
#endif

#endif /* ELFDBG_COMPAT_H */
