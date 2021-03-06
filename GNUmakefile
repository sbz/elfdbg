CC?=	cc
CFLAGS?=	-O2 -g -pipe -D_DEFAULT_SOURCE -Wincompatible-pointer-types
SRCS=	elf.c elfdbg.c compat.c
OBJS=	${SRCS:.c=.o}
PREFIX?=	/usr/local
MANPREFIX?=	/usr/share/man

all: elfdbg

elfdbg: ${OBJS}
	${CC} ${OBJS} ${LDFLAGS} -o $@

.c.o:
	${CC} ${CFLAGS} -c $<

install: elfdbg install-man
	install -m 755 elfdbg ${DESTDIR}${PREFIX}/bin/

install-man:
	gzip elfdbg.1 || /usr/bin/true
	install -m 644 elfdbg.1.gz ${MANPREFIX}/man1/

install-strip: elfdbg
	install -s -m 755 elfdbg ${DESTDIR}${PREFIX}/bin/

clean:
	rm -rf ${OBJS} elfdbg

.PHONY: elfdbg
