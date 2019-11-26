#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("olawi");

int hello_init(void) {
    printk(KERN_DEBUG "Ehllo from olawi\n");
    return 0;
}

void hello_exit(void) {
    printk(KERN_DEBUG "byebye from olawi\n");
}        

module_init(hello_init);
module_exit(hello_exit);

