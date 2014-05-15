#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>
#include <asm/io.h>

#define DEVICE_NAME     "MIPH"   
#define miph_MAJOR    235        



static DECLARE_WAIT_QUEUE_HEAD(miph_waitq);

static volatile int ev_press = 0;


static irqreturn_t miph_interrupt(int irq)
{
    ev_press = 1;                
    wake_up_interruptible(&miph_waitq);
    
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int s3c24xx_miph_open(struct inode *inode, struct file *file)
{


    int err;
    
	err = request_irq(IRQ_EINT19, miph_interrupt, IRQF_TRIGGER_RISING ,"miph", NULL);
 
    if (err) 
        printk("can't request irq!\n");
    
    return 0;
}

static int s3c24xx_miph_close(struct inode *inode, struct file *file)
{

    free_irq(IRQ_EINT19, NULL);

    return 0;
}

static int s3c24xx_miph_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
    unsigned long err;
    
    wait_event_interruptible(miph_waitq, ev_press);

    err = copy_to_user(buff, (const void *)&ev_press, sizeof(ev_press));

    ev_press = 0;

    return err ? -EFAULT : 0 ;
    
}

static struct file_operations s3c24xx_miph_fops = {
    .owner   =   THIS_MODULE,   
    .open    =   s3c24xx_miph_open,
    .release =   s3c24xx_miph_close, 
    .read    =   s3c24xx_miph_read,
};

static int __init s3c24xx_miph_init(void)
{
    int ret;

    ret = register_chrdev(miph_MAJOR, DEVICE_NAME, &s3c24xx_miph_fops);
    if (ret < 0) {
      printk(DEVICE_NAME " can't register major number\n");
      return ret;
    }
    
    printk(DEVICE_NAME " initialized\n");
    return 0;
}

static void __exit s3c24xx_miph_exit(void)
{
    unregister_chrdev(miph_MAJOR, DEVICE_NAME);
}

module_init(s3c24xx_miph_init);
module_exit(s3c24xx_miph_exit);

MODULE_AUTHOR("UP-TECH");             
MODULE_DESCRIPTION("UP-MAGIC MIPH Driver");   
MODULE_LICENSE("GPL");                              

