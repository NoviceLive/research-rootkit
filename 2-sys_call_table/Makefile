obj-m := sys_call_table.o


default:
	${MAKE} modules \
		--directory "/lib/modules/$(shell uname --release)/build" \
		M="$(shell pwd)"
