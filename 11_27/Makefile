MAKE = make
MODULE_DIR = ./led_module/
MODULE_NAME = led_dev
MODULE_DEV = /sys/class/ledclass/led01/led_attr

module:
	cd $(MODULE_DIR); \
	$(MAKE) -C /usr/src/linux-headers-`uname -r` M=`pwd` modules

module_install:
	sudo insmod $(MODULE_DIR)$(MODULE_NAME).ko
	sudo chmod 0777 $(MODULE_DEV)

module_remove:
	sudo rmmod $(MODULE_NAME)

clean:
	rm $(MODULE_DIR)*.o $(MODULE_DIR)*.ko $(MODULE_DIR)*.mod.c $(MODULE_DIR)Module.symvers $(MODULE_DIR)modules.order
