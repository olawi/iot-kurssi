#include <linux/init.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/kernel.h>
#include <linux/gpio.h>

#define DEVICE_NAME "led01"
#define CLASS_NAME  "ledclass"
#define LED_GPIO    4

struct led_dev {
        struct cdev cdev;
};

int led_on = 0;

static ssize_t led_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
static ssize_t led_show(struct device *dev, struct device_attribute *attr, char *buf);

struct file_operations fops = {
        .owner = THIS_MODULE
};

dev_t led_dev_number;
struct device *led_device;
struct class *led_class;
struct led_dev *my_dev;

DEVICE_ATTR(led_attr, S_IRUSR | S_IWUSR, led_show, led_store);

int init_led_module(void) {
    printk(KERN_DEBUG "Init led module\n");
    my_dev = (struct led_dev *) kmalloc(sizeof(struct led_dev), GFP_KERNEL);
    if(my_dev == NULL) {
        printk(KERN_DEBUG "Memory allocation failed\n");
        return -ENOMEM;
    }
    if(alloc_chrdev_region(&led_dev_number, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_DEBUG "Can't reserve magic number\n");
        goto mem;
    }
    led_class = class_create(THIS_MODULE, CLASS_NAME);
    led_device = device_create(led_class, NULL, led_dev_number, NULL, DEVICE_NAME);

    // HOX: dev_attr_led_attr created by macro DEVICE_ATTR
    if(device_create_file(led_device, &dev_attr_led_attr) < 0) {
        printk(KERN_DEBUG "Can't create device attribute\n");
        goto attr;
    }

    cdev_init(&my_dev->cdev, &fops);
    if(cdev_add(&my_dev->cdev, led_dev_number, 1) < 0) {
        printk(KERN_DEBUG "Can't add led dev\n");
        goto attr;
    }

    if(!gpio_is_valid(LED_GPIO)) {
        printk(KERN_ALERT "GPIO %d is not valid\n", LED_GPIO);
        cdev_del(&my_dev->cdev);
        goto attr;
    }
    gpio_request(LED_GPIO, DEVICE_NAME);
    gpio_direction_output(LED_GPIO, led_on);

    printk(KERN_DEBUG "Success installing led_dev\n");
    return 0;

    attr:
        device_destroy(led_class, led_dev_number);
        class_destroy(led_class);
        unregister_chrdev_region(led_dev_number, 1);

    mem:
        kfree(my_dev);
        return -ENODEV;
}

void remove_led_module(void) {
    printk(KERN_DEBUG "Removing led module\n");
    cdev_del(&my_dev->cdev);
    device_destroy(led_class, led_dev_number);
    class_destroy(led_class);
    unregister_chrdev_region(led_dev_number, 1);
    kfree(my_dev);
}

static ssize_t led_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count) {
    printk(KERN_DEBUG "led_store called\n");

    if(led_on == 0) {
        led_on = 1;
        printk(KERN_DEBUG "Turning led on!\n");
    } else {
        led_on = 1;
        printk(KERN_DEBUG "Turning led off!\n");
    }
    gpio_set_value(LED_GPIO, led_on);
    return count;
}

static ssize_t led_show(struct device *dev, struct device_attribute *attr, char *buf) {
    printk(KERN_DEBUG "led_show called\n");
    return 1;
}

MODULE_LICENSE("GPL");
module_init(init_led_module);
module_exit(remove_led_module);

