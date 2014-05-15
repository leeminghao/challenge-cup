obj-m	:=magic_dddf.o

KERNELDIR ?= /home/lmh/program/bcb/system/linux-2.6.24.4/
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
.PHYON:modules
