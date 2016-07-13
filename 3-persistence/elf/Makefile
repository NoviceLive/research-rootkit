all:
	make lssec
	make lssym
	make setsym


install:
	install lssec lssym setsym /usr/bin/


uninstall:
	rm -rf /usr/bin/{lssec,lssym,setsym}


lssec: lssec.c
	${CC} lssec.c -o lssec


lssym: lssym.c uelf.c
	${CC} lssym.c uelf.c -o lssym


setsym: setsym.c uelf.c
	${CC} setsym.c uelf.c -o setsym


clean:
	rm -f lssec lssym setsym
