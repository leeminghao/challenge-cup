#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/delay.h>
#include<asm/irq.h>
#include<asm/arch/regs-gpio.h>
#include<asm/hardware.h>

#define DEVICE_NAME	"dddf"
#define DDDF_MAJOR	240

#define IOCTL_DD_OFF	1
#define IOCTL_DD_ON     0
#define IOCTL_DF_OFF	3
#define IOCTL_DF_ON		4

static unsigned long gpio_table [] = {
    S3C2410_GPE11,
    S3C2410_GPE13,
};

static unsigned int gpio_cfg_table [] = {
    S3C2410_GPE11_OUTP,
    S3C2410_GPE13_OUTP,
};

static int s3c24xx_dddf_open(struct inode *inode, struct file *file)
{
    int i;
    
    for (i = 0; i < 2; i++) {
        s3c2410_gpio_cfgpin(gpio_table[i], gpio_cfg_table[i]);
    }
//    s3c2410_gpio_setpin(gpio_table[0], 0);
    return 0;
}

static int s3c24xx_dddf_ioctl(
    struct inode *inode, 
    struct file *file, 
    unsigned int cmd, 
    unsigned long arg)
{   
    switch(cmd) 
    {
    case 0:
    case 1:
	 if ( arg > 2 )
	 {
	     return -EINVAL;
	 }
	 s3c2410_gpio_setpin( gpio_table[arg], cmd );
	 return 0;
    default:
        return -EINVAL;
    }
}

static struct file_operations s3c24xx_dddf_fops = {
    .owner  =   THIS_MODULE,    
    .open   =   s3c24xx_dddf_open,     
    .ioctl  =   s3c24xx_dddf_ioctl,
};

static int __init s3c24xx_dddf_init(void)
{
    int ret;
    ret = register_chrdev(DDDF_MAJOR, DEVICE_NAME, &s3c24xx_dddf_fops);
    if (ret < 0) {
      printk(DEVICE_NAME " can't register dddf major number\n");
      return ret;
    }
    
    printk(DEVICE_NAME " initialized\n");
    return 0;
}

static void __exit s3c24xx_dddf_exit(void)
{
    unregister_chrdev(DDDF_MAJOR, DEVICE_NAME);
}

module_init(s3c24xx_dddf_init);
module_exit(s3c24xx_dddf_exit);

MODULE_AUTHOR("WANGJW");          
MODULE_DESCRIPTION("UP-MAGIC DDDF Driver");
MODULE_LICENSE("GPL");  
