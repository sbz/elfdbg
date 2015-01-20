DESTDIR=/usr/local/
BINDIR=bin

PROG=	elfdbg
SRCS=	elfdbg.c

CFLAGS+=-lelf

.include <bsd.prog.mk>
