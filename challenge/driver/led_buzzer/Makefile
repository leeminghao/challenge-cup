obj-m	:=magic_gpio.o

KERNELDIR ?= /home/wangjw/Linux_V4.0/kernel/linux-2.6.24.4/
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
.PHYON:modules
