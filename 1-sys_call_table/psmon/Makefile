TARGET = psmon
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
		&& whoami; uname -a &&  sudo dmesg -c && sudo rmmod ${TARGET}ko && sudo dmesg -c
