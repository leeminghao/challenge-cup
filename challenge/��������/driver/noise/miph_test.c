#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_LED_ON    0
#define IOCTL_LED_OFF   1


int main(int argc, char **argv)
{
    int i;
    int ret;
    int fd,fd1;
    int press_cnt;
    system("mknod /dev/gpio c 234 0");
    system("mknod /dev/miph c 235 0");

    fd = open("/dev/miph", 0); 
    fd1 = open("/dev/gpio", 0); 
    if (fd < 0) {
        printf("Can't open /dev/miph\n");
        return -1;
    }

    while (1) {
		ioctl(fd1,IOCTL_LED_ON);
 		
		sleep(1);
		ioctl(fd1,IOCTL_LED_OFF);
		
        ret = read(fd, &press_cnt, sizeof(press_cnt));
        if (ret < 0) {
            printf("read err!\n");
            continue;
        } 
        if (press_cnt){
        	printf("noise.\n");
	}

        sleep(1);
    }
    
    close(fd);
    return 0;    
}

