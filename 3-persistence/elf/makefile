all:
	make lssec
	make lssym
	make setsym


install:
	install lssec lssym setsym /usr/bin/


uninstall:
	rm -rf /usr/bin/{lssec,lssym,setsym}


lssec: lssec.c
	${CC} -std=c99 lssec.c -o lssec


lssym: lssym.c uelf.c
	${CC} -std=c99 lssym.c uelf.c -o lssym


setsym: setsym.c uelf.c
	${CC} -std=c99 setsym.c uelf.c -o setsym


clean:
	rm -f lssec lssym setsym
