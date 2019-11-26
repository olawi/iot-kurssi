#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#define DEVICE_NAME "char_dumper"

MODULE_LICENSE("GPL");

int my_open(struct inode *inode, struct file *fp);
int my_close(struct inode *inode, struct file *fp);
ssize_t my_read(struct file *fp, char __user *to, size_t count, loff_t *position);
ssize_t my_write(struct file *fp, const char __user *from, size_t count, loff_t *position);

struct file_operations fops = {
    .owner      = THIS_MODULE,
    .read       = &my_read,
    .write      = &my_write,
    .open       = &my_open,
    .release    = &my_close
};

struct my_dev {
    struct  cdev cdev;
    int     connections;
    char    *data_buffer;
    char    name[32];
};

dev_t my_dev_nro;
struct my_dev *device;
struct class *my_dev_class;

int my_init(void) {
    printk(KERN_DEBUG "Initializing char_dumper\n");

    if(alloc_chrdev_region(&my_dev_nro, 0 , 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "Cannot register chrdev region\n");
        return -1;
    }

    device = (struct my_dev *) kmalloc(sizeof(struct my_dev), GFP_KERNEL);
    if(device == NULL) {
        printk(KERN_ALERT "Can't allocate memory\n");
        goto chrdev;
    }

    my_dev_class = class_create(THIS_MODULE, DEVICE_NAME);
    sprintf(device->name, DEVICE_NAME);
    device->connections = 0;
    cdev_init(&device->cdev, &fops);
    device->cdev.owner = THIS_MODULE;

    if(cdev_add(&device->cdev, my_dev_nro, 1) < 0) {
        printk(KERN_ALERT "Cannot add cdev\n");
        goto mem;
    }

    device_create(my_dev_class, NULL, my_dev_nro, NULL, DEVICE_NAME);
    printk(KERN_DEBUG "Initialization of char_dumper successful\n");
    return 0;

    mem:
        class_destroy(my_dev_class);
        kfree(device);
    chrdev:
        unregister_chrdev_region(my_dev_nro, 1);
        return -1;

}

void my_exit(void) {
    cdev_del(&device->cdev);
    device_destroy(my_dev_class, my_dev_nro);
    kfree(device);
    unregister_chrdev_region(my_dev_nro, 1);
    printk(KERN_DEBUG "Unregistered char_dumper\n");
}

int my_open(struct inode *inode, struct file *fp) {
    struct my_dev *device;
    printk(KERN_DEBUG "Opening device char_dumper");
    device = container_of(inode->i_cdev, struct my_dev, cdev);
    fp->private_data = device;
    if(device->connections == 0) {
        printk(KERN_DEBUG "Reserving memory for buffer\n");
        device->data_buffer = (char *) kmalloc(sizeof(char), GFP_KERNEL);
    }
    device->connections++;
    return 0;
}

int my_close(struct inode *inode, struct file *fp) {
    struct my_dev *device;
    device = fp->private_data;
    device->connections--;
    printk(KERN_DEBUG "Closing char_dumper\n");
    if(device->connections == 0) {
        printk(KERN_DEBUG "Closing for the last time, freeing buffer\n");
        kfree(device->data_buffer);
    }
    return 0;
}

ssize_t my_read(struct file *fp, char __user *to, size_t count, loff_t *position) {
    struct my_dev *device;
    int n;
    printk(KERN_DEBUG "Reading for %d bytes\n", (int) count);
    device = fp->private_data;
    n = copy_to_user(to, device->data_buffer + *position, count);
    *position = *position + (loff_t) count;
    printk(KERN_DEBUG "Failed to read %d bytes\n", n);
    printk(KERN_DEBUG "Currently in position %d\n", (int) *position);
    return count - n;
}

ssize_t my_write(struct file *fp, const char __user *from, size_t count, loff_t *position) {
    return 0;
}

module_init(my_init);
module_exit(my_exit);