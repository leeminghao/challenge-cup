#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <linux/ioport.h>
#include <linux/in.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <asm/bitops.h>
#include <asm/io.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <asm/irq.h>
#include <asm/arch/hardware.h>
#include <asm/arch/regs-gpio.h>
#include <linux/device.h>

#define SHT11_MAJOR 238  
#define DEVICE_NAME "sht11"
static unsigned char mode_value;
//struct class *Sht11_class; 
//static int major=SHT11_MAJOR;

typedef union
{
   unsigned int i;
   float f;
} value;

#define TEMP 0              //ioctl(fd,0)
#define HUMI 1              //i0ctl(fd,1)

#define noACK 0
#define ACK   1
                             //addr  command  r/w
#define STATUS_REG_W 0x06    //000   0011     0           写状态寄存器
#define STATUS_REG_R 0x07    //000   0011     1           读状态寄存器
#define MEASURE_TEMP 0x03    //000   0001     1           温度测量
#define MEASURE_HUMI 0x05    //000   0010     1           湿度测量
#define RESET        0x1e    //000   1111     0           软启动

static void Sensor_DATA_IN(void) 
{ 
  int temp;
  temp= __raw_readl(S3C2410_GPECON);
  temp&=0x3fffffff;                    //设置GPE15数据输入
  __raw_writel(temp,S3C2410_GPECON);
}
    
static void Sensor_DATA_OUT(void)
{
  int temp;
  temp= __raw_readl(S3C2410_GPECON);
  temp=((temp&0x3fffffff)|1<<30);      //设置GPE15数据输出
  __raw_writel(temp,S3C2410_GPECON);
}

static void Sensor_CLK_OUT(void)
{
   int temp;
   temp= __raw_readl(S3C2410_GPECON);
   temp=((temp&0xcfffffff)|1<<28);      //设置GPE14控制输出
   __raw_writel(temp,S3C2410_GPECON);
} 

static void set_DATA_1(void)   
{
   int temp;
   temp= __raw_readl(S3C2410_GPEDAT);
  temp=((temp&0x7fff)|1<<15);           //向数据写1
  __raw_writel(temp,S3C2410_GPEDAT);
}

static void set_DATA_0(void)    
{
   int temp;
   temp= __raw_readl(S3C2410_GPEDAT);
  temp=temp&0x7fff;                      //向数据写0
  __raw_writel(temp,S3C2410_GPEDAT);
}

static void set_CLK_1(void)             
{
   int temp;
   temp= __raw_readl(S3C2410_GPEDAT);
  temp=((temp&0xbfff)|1<<14);                //‪GPEdat14位置1
  __raw_writel(temp,S3C2410_GPEDAT);
}    
static void set_CLK_0(void)
{
  int temp;
   temp= __raw_readl(S3C2410_GPEDAT);
  temp=temp&0xbfff;                         //GPEdat14位置0
  __raw_writel(temp,S3C2410_GPEDAT);
}
static char  IS_DATA_1(void)              //？**************
{
   char temp;
   unsigned short int temp1;
   temp1=__raw_readl(S3C2410_GPEDAT);
   temp=(char)(temp1>>15);
   //printk("temp=%d\n",temp);
   udelay(10);
   return temp;
}

void _nop_(void) 
{
   int i;
   for(i=0;i<2000;i++); 
}

static int sth11_open(struct inode *inode, struct file *file)
{
    Sensor_DATA_OUT();           //GPE15数据输出
    set_DATA_0();  
    Sensor_CLK_OUT();            //GPE14控制输出
    set_CLK_0();
    return 0;
}

char s_write_byte(unsigned char value)
{
    unsigned char i,error;
	for (i=0x80;i>0;i/=2) //shift bit for masking
	{ 
		if (i & value) set_DATA_1(); //masking value with i , write to SENSI-BUS
		else set_DATA_0();
		set_CLK_1(); //clk for SENSI-BUS
		_nop_();_nop_();_nop_(); //pulswith approx. 5 us
		
		set_CLK_0();
		
	}
	set_DATA_1();            //release DATA-line
	Sensor_DATA_IN();
	set_CLK_1(); 
	
	error=IS_DATA_1(); //check ack (DATA will be pulled down by SHT11)
        //printk("error_write=0x%X\n",error);
	Sensor_DATA_OUT();
	set_CLK_0();
        return error;
}


char s_read_byte(unsigned char ack)
{
unsigned char i,val=0;
	set_DATA_1(); //release DATA-line
	Sensor_DATA_IN();
	for (i=0x80;i>0;i/=2) //shift bit for masking
	{ 
		set_CLK_1(); //clk for SENSI-BUS
		if (IS_DATA_1()) val=(val | i); //read bit
		set_CLK_0();

	}
	Sensor_DATA_OUT();
	if(ack)
		set_DATA_0();
	else
		set_DATA_1();
//	DATA=!ack; //in case of "ack==1" pull down DATA-Line
	set_CLK_1(); //clk #9 for ack
	_nop_();_nop_();_nop_(); //pulswith approx. 5 us

	set_CLK_0();
	set_DATA_1(); //release DATA-line
	return val;  
}


void s_transstart(void)
{
   set_DATA_1(); set_CLK_0(); //Initial state
	_nop_();
	set_CLK_1();
	_nop_();
	set_DATA_0();
	_nop_();
	set_CLK_0();
	_nop_();_nop_();_nop_();
	set_CLK_1();
	_nop_();
	set_DATA_1();
	_nop_();
	set_CLK_0();
}

void s_connectionreset(void)
{
   unsigned char i;
	set_DATA_1(); set_CLK_0(); //Initial state
	for(i=0;i<9;i++) //9 SCK cycles
	{ 
		set_CLK_1();
		_nop_();
		set_CLK_0();
		_nop_();
	}
	s_transstart(); //transmission start   
}

char s_measure(unsigned char *p_value,unsigned char *p_checksum,unsigned char mode)
{
   unsigned error=0;
	unsigned int i;
	s_transstart(); //transmission start
	switch(mode){ //send command to sensor
		case TEMP : error+=s_write_byte(MEASURE_TEMP); break;
		case HUMI : error+=s_write_byte(MEASURE_HUMI); break;
		default : break;
	}
//printk("error_measure=0x%X\n",error);
	Sensor_DATA_IN();

	for (i=0;i<65535;i++) if(!IS_DATA_1()) break; //wait until sensor has finished the measurement
	for (i=0;i<65535;i++) if(!IS_DATA_1()) break;
	if(IS_DATA_1()) error+=1; // or timeout (~2 sec.) is reached
	Sensor_DATA_OUT();//»ָ´
	*(p_value+1) =s_read_byte(ACK); //read the first byte (MSB)
 // printk("error_measure1=0x%X\n",error);
	*(p_value)=s_read_byte(ACK); //read the second byte (LSB)

	*p_checksum =s_read_byte(noACK); //read checksum

      // ( *(p_value+1))&=0x0f;
	return error;
}

int sth11_ioctl(struct inode *inode,struct file * file,unsigned int cmd,unsigned long arg)
{
    
    switch(cmd) {
    case TEMP:
        mode_value = TEMP;
      //  printk("1 cmd=%d\n",cmd);
       // printk("2 mode_value=%d\n",mode_value);
        break;
    case HUMI:
	mode_value = HUMI;
//printk("cmd=%d\n",cmd);printk("mode_value=%d\n",mode_value);
break;
    default:break;
    }
        return 0;
}
static int  sth11_read(struct file * file, char __user * userbuf, size_t count, loff_t * off)
{
    unsigned int temp_value=0;
    unsigned char error,checksum=0;
    error=0;
    //printk("3 mode_value=%d\n",mode_value);
   
    error+=s_measure((unsigned char*)&temp_value,&checksum,mode_value);
    udelay(10);
   // printk("valuek=%d\n",temp_value);
   // printk("error_read=0x%X\n",error);
    copy_to_user(userbuf,&temp_value,sizeof(temp_value));
//set_DATA_1();
//set_CLK_1();
    return error;
}

static struct file_operations s3c24xx_sth11_fops = {
    .owner  =   THIS_MODULE,    
    .open   =   sth11_open,     
    .ioctl  =   sth11_ioctl,
    .read   =   sth11_read,
};

int __init init_sht11(void)
{
	int ret_val=0;
	ret_val=register_chrdev(SHT11_MAJOR,DEVICE_NAME,&s3c24xx_sth11_fops);
	
	if (ret_val < 0) 
	{
		printk("can't get major %d\n", SHT11_MAJOR);
		return ret_val;
    	}
    printk("register_chrdev: %d\n", ret_val);
    
 /*   Sht11_class=class_create(THIS_MODULE,"Sht11_class");   /*新添加
    if(IS_ERR(Sht11_class))
    {
		printk("err:failed in creating class.\n");
                return -1;
    }
        
    class_device_create(Sht11_class,NULL,MKDEV(SHT11_MAJOR,0),NULL,"Sht11%d",0);
    //class_device_create(keys_class,NULL,MKDEV(KEYS_MAJOR,0),NULL,"KEYS%d",0);*/
	return 0;
}
static void __exit exit_sht11(void)
{
	unregister_chrdev(SHT11_MAJOR, DEVICE_NAME);
}


module_init(init_sht11);
module_exit(exit_sht11);

MODULE_AUTHOR("UP-TECH");          
MODULE_DESCRIPTION("UP-MAGIC STH11 Driver");
MODULE_LICENSE("GPL");

