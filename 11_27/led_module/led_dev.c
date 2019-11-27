#include <linux/init.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/workqueue.h>

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

/** Blinking workqueue */

struct workqueue_struct *my_wq;

struct blinking_work {
    struct delayed_work blink;
    int data;    
};

struct blinking_work *my_work;
int blinking = 0;
void blinking_work_function(struct work_struct *work);

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
    
    my_wq = alloc_workqueue("blinking_queue", 0, 4);

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
    long ticks = 0;
    printk(KERN_DEBUG "led_store called %s\n", buf);
    
    if(strncmp(buf, "on", 2) == 0) {
        printk(KERN_DEBUG "Turning led on!\n");
        gpio_set_value(LED_GPIO, 1);
        return count;
    }
    if(strncmp(buf, "off", 3) == 0) {
        printk(KERN_DEBUG "Turning led off!\n");
        gpio_set_value(LED_GPIO, 0);
        return count;
    }
    if(kstrtol(buf, 0, &ticks)) {
        printk(KERN_DEBUG "Invalid value\n");
        return count;
    }
    
    if(blinking) {
        blinking = 0;
    } else {
        blinking = 1;
        my_work = (struct blinking_work *) kzalloc(sizeof(struct blinking_work), GFP_KERNEL);
        if(my_work == NULL) {
            printk(KERN_ALERT "Cannot allocate memory for work\n");
            blinking = 0;
            return count;
        }
        INIT_DELAYED_WORK((struct delayed_work *) my_work, &blinking_work_function);
        my_work->data = ticks;
        queue_delayed_work(my_wq, (struct delayed_work *) my_work, ticks);
        //queue_delayed_work(my_wq, (struct delayed_work *) my_work, 100);
    }
    
    return count;
}

static ssize_t led_show(struct device *dev, struct device_attribute *attr, char *buf) {
    printk(KERN_DEBUG "led_show called\n");
    return 1;
}

void blinking_work_function(struct work_struct *work) {
    long ticks = 0;
    struct blinking_work *my_work = (struct blinking_work *) work;

    if(blinking == 0) {
        gpio_set_value((LED_GPIO), 0);
        kfree(my_work);
        return;
    }

    if(led_on == 0) {
        led_on = 1;
    } else {
        led_on = 0;
    }

    gpio_set_value(LED_GPIO, led_on);
    queue_delayed_work(my_wq, (struct delayed_work *) my_work, ticks);
    //queue_delayed_work(my_wq, (struct delayed_work *) my_work, 100);
}

MODULE_LICENSE("GPL");
module_init(init_led_module);
module_exit(remove_led_module);

