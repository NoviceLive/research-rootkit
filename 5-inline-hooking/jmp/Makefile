TARGET = jmp
obj-m := ${TARGET}ko.o
${TARGET}ko-objs := ${TARGET}.o zeroevil/zeroevil.o


ccflags-y = "-O0"


default:
	${MAKE} modules \
		--directory "/lib/modules/$(shell uname --release)/build" \
		M="$(shell pwd)"


clean:
	${MAKE} clean \
		--directory "/lib/modules/$(shell uname --release)/build" \
		M="$(shell pwd)"
