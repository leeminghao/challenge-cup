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

#define DEVICE_NAME     "SMOG"  
#define SMOG_MAJOR    231         

static volatile int ev_press = 0;
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

static irqreturn_t smog_interrupt(int irq)
{
    ev_press = 1;                
    wake_up_interruptible(&button_waitq);   
    
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int s3c24xx_smog_open(struct inode *inode, struct file *file)
{ 
	 
    int err;
    
    err = request_irq(IRQ_EINT4, smog_interrupt,IRQF_TRIGGER_RISING, "smog", NULL);
    if (err)
        printk("can't request irq!\n");

    return 0;
}

static int s3c24xx_smog_close(struct inode *inode, struct file *file)
{
    free_irq(IRQ_EINT4, NULL);
    return 0;
}

static int s3c24xx_smog_read(struct file *filp, char __user *buff, 
                                         size_t count, loff_t *offp)
{
    unsigned long err;
    
    wait_event_interruptible(button_waitq, ev_press);
   
    err = copy_to_user(buff, (const void *)&ev_press, sizeof(ev_press));
    ev_press = 0;
    return err ? -EFAULT : 0;

}

static struct file_operations s3c24xx_smog_fops = {
    .owner   =   THIS_MODULE,    
    .open    =   s3c24xx_smog_open,
    .release =   s3c24xx_smog_close, 
    .read    =   s3c24xx_smog_read,
};


static int __init s3c24xx_smog_init(void)
{
    int ret;

    ret = register_chrdev(SMOG_MAJOR, DEVICE_NAME, &s3c24xx_smog_fops);
    if (ret < 0) {
      printk(DEVICE_NAME " can't register major number\n");
      return ret;
    }
    
    printk(DEVICE_NAME " initialized\n");
    return 0;
}

static void __exit s3c24xx_smog_exit(void)
{
    unregister_chrdev(SMOG_MAJOR, DEVICE_NAME);
}

module_init(s3c24xx_smog_init);
module_exit(s3c24xx_smog_exit);

MODULE_AUTHOR("xw_tech@126.com");             
MODULE_DESCRIPTION("S3C2410 SMOG Driver");   
MODULE_LICENSE("GPL");                              

