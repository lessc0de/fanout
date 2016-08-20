ifneq ($(KERNELRELEASE),)
	obj-m += fanout.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
 
all: fanout mkfanout

fanout:
	make -C $(KERNELDIR) M=$(PWD) modules
 
mkfanout: mkfanout.c
	gcc -O2 -o $@ $<
	chmod 750 $@
	sudo setcap CAP_MKNOD=+ep $@

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
	rm -f mkfanout
 
install:
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char
	cp fanout.ko /lib/modules/$(shell uname -r)/kernel/drivers/char
	depmod -a
endif
