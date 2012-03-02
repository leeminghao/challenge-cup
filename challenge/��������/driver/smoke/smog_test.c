#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IOCTL_LED_ON    0
#define IOCTL_LED_OFF   1
#define IOCTL_BUZZER_OFF  3
#define IOCTL_BUZZER_ON   4

int main(int argc, char **argv)
{
    int i;
    int ret;
    int fd,fd1;
    int smog_cnt;

	system("mknod /dev/gpio c 234 0");
	system("mknod /dev/smog c 231 0");
    fd = open("/dev/smog", 0); 
    fd1 = open("/dev/gpio", 0);  
    if (fd < 0) {
        printf("Can't open /dev/smog\n");
        return -1;
    }

    while (1) {
 		ioctl(fd1,IOCTL_LED_ON);
 		ioctl(fd1,IOCTL_BUZZER_ON);
		sleep(1);
		ioctl(fd1,IOCTL_LED_OFF);
		ioctl(fd1,IOCTL_BUZZER_OFF);
		
        ret = read(fd,&smog_cnt, sizeof(smog_cnt));
        if (ret < 0) {
            printf("read err!\n");
            continue;
        } 
        if (smog_cnt){
            printf("fire!\n");
		}
        sleep(1);
    }
    close(fd);
    return 0;
}

