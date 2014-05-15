#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/ioctl.h>

#define TEMP 0
#define HUMI 1
#define IOCTL_LED_ON    0
#define IOCTL_LED_OFF   1
#define IOCTL_BUZZER_OFF  3
#define IOCTL_BUZZER_ON   4

void calc_sht11(float *p_humidity,float *p_temprature)
{
   const float C1=-0.40;//针对于12为测量精度
   const float C2=0.0405;
   const float C3=-0.0000028;
   const float T1=0.01;//相对湿度的温度补偿
   const float T2=0.00008;

   float rh=*p_humidity;
   float t=*p_temprature;
   float rh_lin;
   float rh_true;
   float t_C;

   t_C=t*0.01-40;//温度值（14为测量数据精度时）
   rh_lin=C3*rh*rh+C2*rh+C1;//临时湿度值
   rh_true=(t_C-25)*(T1+T2*rh)+rh_lin;//修正后的湿度值
   if(rh_true>100)rh_true=100;
   if(rh_true<0.1)rh_true=0.1;

   *p_temprature=t_C;
   *p_humidity=rh_true;
}

float calc_dewpoint(float h,float t)//空气的露点值
{
   float k,dew_point;
   k=(log10(h)-2)/0.4343+(17.62*t)/(243.12+t);
   dew_point=243.12*k/(17.62-k);
   return dew_point;
}

void delay(int time)
{
   int i;
   for(i=0;i<time*1000;)
     i++;
}

int main(void)
{
    int fd,fd1,ret,i;
    	unsigned int value_t=0;
        unsigned int value_h=0;
        float fvalue_t,fvalue_h;
        float dew_point;
    system("mknod /dev/sht11 c 238 0");
    system("mknod /dev/gpio c 234 0");
	fd = open("/dev/sht11",0);

	if(fd<0)
	{
		printf("open /dev/sht11 error!\n");
		return -1;
	}
	fd1 = open("/dev/gpio", 0);
	if (fd1 < 0) {
        printf("Can't open /dev/gpio\n");
        return -1;
    }
 	
	for(;;)
	{
                fvalue_t=0.0,fvalue_h=0.0;value_t=0;value_h=0;
		ioctl(fd,0);
		ret=read(fd,&value_t,sizeof(value_t));
                if(ret<0)
		{
			printf("read err!\n");
			continue;
		}
		
                value_t=value_t&0x3fff;//温度：14位测量数据

                fvalue_t=(float)value_t;

                ioctl(fd,1);
		ret=read(fd,&value_h,sizeof(value_h));

		if(ret<0)
		{
			printf("read err!\n");
			continue;
		}
                value_h=value_h&0xfff;//湿度：12位测量数据
                fvalue_h=(float)value_h;
                calc_sht11(&fvalue_h,&fvalue_t);//将输出转换为物理量
                dew_point=calc_dewpoint(fvalue_h,fvalue_t);//空气的露点值
                printf("temp:%fc humi:%f dew point:%fc\n",fvalue_t,fvalue_h,dew_point);
                if(fvalue_t>25)
                {

    					ioctl(fd1,IOCTL_BUZZER_ON);
    					ioctl(fd1,IOCTL_LED_ON);
    					sleep(1);
						                    
						ioctl(fd1,IOCTL_LED_OFF);
						ioctl(fd1,IOCTL_BUZZER_OFF);
					
				}
                else
                {
                	ioctl(fd1,IOCTL_BUZZER_OFF);
                	ioctl(fd1,IOCTL_LED_OFF);
                }
                sleep(1);
               
}
}
