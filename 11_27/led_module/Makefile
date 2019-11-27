MODULE_NAME = led_dev
MODULE_DEV = /sys/class/ledclass/led01/led_attr
KERNEL_HEADER_PATH = /usr/src/linux-headers-$(shell uname -r)

obj-m += $(MODULE_NAME).o

all: $(MODULE_NAME).ko

$(MODULE_NAME).ko: $(MODULE_NAME).c
	make -C $(KERNEL_HEADER_PATH) M=$(PWD) modules

clean: 
	make -C $(KERNEL_HEADER_PATH) M=$(PWD) clean

install-module: $(MODULE_DEV)

$(MODULE_DEV): $(MODULE_NAME).ko
	sudo insmod $(CWD)$(MODULE_NAME).ko; \
	sudo chmod 0777 $(MODULE_DEV)

remove-module:
	test ! -f $(MODULE_DEV) || sudo rmmod $(MODULE_NAME)
