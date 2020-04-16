.include <bsd.own.mk>

PREFIX?=	/usr/local
BINDIR=	${PREFIX}/bin

PROG=	elfdbg
MAN=	elfdbg.1
SRCS=	elfdbg.c

LDADD=	-lelf

.include <bsd.prog.mk>
