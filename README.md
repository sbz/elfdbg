# elfdbg

[![Build Status](https://api.cirrus-ci.com/github/sbz/elfdbg.svg)](https://cirrus-ci.com/github/sbz/elfdbg)

## About

This project _elfdbg_ is a utility program to quickly identify if an ELF binary
is built with [debug sections](https://en.wikipedia.org/wiki/Debug_symbol)

Debug sections in ELF binary usually are generated using the `-g` flag with the
compiler. The compiler adds the new `.debug_*` sections in the binary.

The program is looking for the existence of sections with name starting with
`.debug_` to determine if the binary has been built with debug information.

The [ELF][ELF] format is a well-known standard. ELF TIS reference specification
is available [here][spec] and as a FreeBSD [elf(5)][man] man page.

## Usage

This is intented to be used in shell script in order to avoid using file(1) and
parsing the output like below:

```shell
`file /path/to/binary | awk '/debug_info/{print $2}'` != 'ELF'
```

To determine if binary has been built with debug, use the following:

```shell
if elfdbg -q /path/to/binary; then
        echo "No symbols"
    else
        echo "Binary with debug symbols"
fi
```

On FreeBSD, you can easily identify the binary packages with missing debug
using similar one-liner:

```shell
pkg info -a -l|grep local/bin/|while read a; do echo $a: `elfdbg $a`; done
```

## Build and install

The Makefile use the standard BSD Makefile to build the program.

```
make && sudo make install
```

If your on Linux, use `make -f GNUMakefile` to specify the GNU Makefile.

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
  [1960d10](https://github.com/sbz/elfdbg/commit/1960d10)

[ELF]: https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
[spec]: http://refspecs.linuxbase.org/elf/elf.pdf
[man]: https://www.freebsd.org/cgi/man.cgi?query=elf&sektion=5
