.include <bsd.own.mk>

PREFIX?=	/usr/local
BINDIR=	${PREFIX}/bin

CFLAGS=-O2 -pipe -std=gnu99 -Wall

PROG=	elfdbg
MAN=	elfdbg.1
SRCS=	elfdbg.c elf.c compat.c

test:
	@if ! type kyua >/dev/null 2>&1; then \
		${ECHO} "kyua not found. Install with pkg install kyua"; exit 1; \
	fi; \

	@(cd tests && kyua test && kyua report)

.include <bsd.prog.mk>
