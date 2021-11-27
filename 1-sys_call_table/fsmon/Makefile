TARGET = fsmon
obj-m := ${TARGET}ko.o
${TARGET}ko-objs := ${TARGET}.o zeroevil/zeroevil.o





default:
	${MAKE} modules \
		--directory "/lib/modules/$(shell uname --release)/build" \
		M="$(shell pwd)"


clean:
	${MAKE} clean \
		--directory "/lib/modules/$(shell uname --release)/build" \
		M="$(shell pwd)"


test: default
	sudo dmesg -c && sudo insmod ${TARGET}ko.ko && sudo dmesg -c \
		&& touch 12345xxx; cat 12345xxx; rm 12345xxx &&  sudo dmesg -c && sudo rmmod ${TARGET}ko && sudo dmesg -c
