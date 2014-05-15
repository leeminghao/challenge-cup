#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

#define DEVICE_NAME     "gpio" 
#define GPIO_MAJOR       234     

#define IOCTL_LED_OFF     1
#define IOCTL_LED_ON      0
#define IOCTL_BUZZER_OFF  3
#define IOCTL_BUZZER_ON   4

static unsigned long gpio_table [] = {
    S3C2410_GPH0,
    S3C2410_GPH1,
};

static unsigned int gpio_cfg_table [] = {
    S3C2410_GPH0_OUTP,
    S3C2410_GPH1_OUTP,
};

static int s3c24xx_gpio_open(struct inode *inode, struct file *file)
{
    int i;
    
    for (i = 0; i < 2; i++) {
        s3c2410_gpio_cfgpin(gpio_table[i], gpio_cfg_table[i]);
    }
    s3c2410_gpio_setpin(gpio_table[0], 0);
    return 0;
}

static int s3c24xx_gpio_ioctl(
    struct inode *inode, 
    struct file *file, 
    unsigned int cmd, 
    unsigned long arg)
{   
    if(arg>3){
	return -EINVAL;
    }
    switch(cmd) {
    case IOCTL_LED_ON:
        s3c2410_gpio_setpin(gpio_table[1], 1);
        return 0;

    case IOCTL_LED_OFF:
        s3c2410_gpio_setpin(gpio_table[1], 0);
        return 0;

    case IOCTL_BUZZER_ON:
        s3c2410_gpio_setpin(gpio_table[0], 1);
        return 0;

    case IOCTL_BUZZER_OFF:
        s3c2410_gpio_setpin(gpio_table[0], 0);
        return 0;
    default:
        return -EINVAL;
    }
}

static struct file_operations s3c24xx_gpio_fops = {
    .owner  =   THIS_MODULE,    
    .open   =   s3c24xx_gpio_open,     
    .ioctl  =   s3c24xx_gpio_ioctl,
};

static int __init s3c24xx_gpio_init(void)
{
    int ret;
    ret = register_chrdev(GPIO_MAJOR, DEVICE_NAME, &s3c24xx_gpio_fops);
    if (ret < 0) {
      printk(DEVICE_NAME " can't register gpio major number\n");
      return ret;
    }
    
    printk(DEVICE_NAME " initialized\n");
    return 0;
}

static void __exit s3c24xx_gpio_exit(void)
{
    unregister_chrdev(GPIO_MAJOR, DEVICE_NAME);
}

module_init(s3c24xx_gpio_init);
module_exit(s3c24xx_gpio_exit);

MODULE_AUTHOR("UP-TECH");          
MODULE_DESCRIPTION("UP-MAGIC GPIO Driver");
MODULE_LICENSE("GPL");                          
