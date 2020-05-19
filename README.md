# elfdbg

## About

This project _elfdbg_ is a utility program to quickly identify if an ELF binary
is built with [debug sections](https://en.wikipedia.org/wiki/Debug_symbol)

Debug sections in ELF binary usually are generated using the `-g` flag with the
compiler. The compiler adds the new `.debug_*` sections in the binary.

The program is looking for the existence of sections with name starting with
`.debug_` to determine if the binary has been built with debug information.

The [ELF][ELF] format is a well-known standard. ELF TIS reference specification
is available [here][spec] and as a FreeBSD [elf(5)][man] man page.

## Build and install

The Makefile use the standard BSDMakefile to build the program.

```
make && sudo make install
```

## Test

The tests cases are implemented using the FreeBSD test suite framework with
[kyua](https://github.com/jmmv/kyua) and [Kyuafile](./tests/Kyuafile).

```
make test
```

## History

* _2015_ I wrote this using libelf elf(3) and gelf(3) API
  [f4b470b](https://github.com/sbz/elfdbg/commit/f4b470b)
* _2020_ I rewrote this without relying on libelf API
  [96010ce](https://github.com/sbz/eldbg/commit/96010ce)

[ELF]: https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
[spec]: http://refspecs.linuxbase.org/elf/elf.pdf
[man]: https://www.freebsd.org/cgi/man.cgi?query=elf&sektion=5
