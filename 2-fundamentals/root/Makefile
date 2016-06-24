TARGET = root
obj-m := ${TARGET}ko.o
${TARGET}ko-objs := ${TARGET}.o lib/lib.o


EXTRA_CFLAGS = "-I${PWD}/lib"


default:
	${MAKE} modules \
		--directory "/lib/modules/$(shell uname --release)/build" \
		M="$(shell pwd)"


clean:
	${MAKE} clean \
		--directory "/lib/modules/$(shell uname --release)/build" \
		M="$(shell pwd)"
