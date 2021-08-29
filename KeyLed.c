/* MIPSfpga: Example userspace application for custom_axi_gpio */
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include<time.h>
#include<string.h>
#define SYSFS_GPIO_EXPORT "/sys/class/gpio/export"
#define SYSFS_GPIO_RST_DIR_VAL "out"
#define SYSFS_GPIO_RST_VAL_H "1"
#define SYSFS_GPIO_RST_VAL_L "0"


int fd;
int i;
char PIN_VAL[16][5]={"505","504","503","502","501","500","499","498","497","496","495","494","493","492","491","490"}; //端口号

//设置方向
char RST_DIR[16][100]={"/sys/class/gpio/gpio505/direction","/sys/class/gpio/gpio504/direction","/sys/class/gpio/gpio503/direction","/sys/class/gpio/gpio502/direction","/sys/class/gpio/gpio501/direction","/sys/class/gpio/gpio500/direction","/sys/class/gpio/gpio499/direction","/sys/class/gpio/gpio498/direction","/sys/class/gpio/gpio497/direction","/sys/class/gpio/gpio496/direction","/sys/class/gpio/gpio495/direction","/sys/class/gpio/gpio494/direction","/sys/class/gpio/gpio493/direction","/sys/class/gpio/gpio492/direction","/sys/class/gpio/gpio491/direction","/sys/class/gpio/gpio490/direction"};

//设置输出
char RST_VAL[16][100]={"/sys/class/gpio/gpio505/value","/sys/class/gpio/gpio504/value","/sys/class/gpio/gpio503/value","/sys/class/gpio/gpio502/value","/sys/class/gpio/gpio501/value","/sys/class/gpio/gpio500/value","/sys/class/gpio/gpio499/value","/sys/class/gpio/gpio498/value","/sys/class/gpio/gpio497/value","/sys/class/gpio/gpio496/value","/sys/class/gpio/gpio495/value","/sys/class/gpio/gpio494/value","/sys/class/gpio/gpio493/value","/sys/class/gpio/gpio492/value","/sys/class/gpio/gpio491/value","/sys/class/gpio/gpio490/value"};
 
//开关值   
int num[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//亮灯函数
void led_open(int index){
   
    for(i=0;i<16;i++){
        if(num[i]==1){
    		fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
    		if (fd ==-1) {
        		printf("could not open ERROR\n");
        		exit(EXIT_FAILURE);
    		}
    		write(fd,PIN_VAL[(i+index)%16],sizeof(PIN_VAL[(i+index)%16])); //"echo PIN_VAL > export"写入编号
    		close(fd);
    
    		fd = open(RST_DIR[(i+index)%16], O_WRONLY);
		    if (fd==-1) {
		        printf("could not open ERROR1\n");
		        exit(EXIT_FAILURE);
		    }
		    write(fd,SYSFS_GPIO_RST_DIR_VAL,sizeof(SYSFS_GPIO_RST_DIR_VAL)); //"echo out > RST_DIR"定义方向
		    close(fd);

		    fd = open(RST_VAL[(i+index)%16], O_RDWR);
		    if (fd ==-1) {
		        printf("could not open ERROR2\n");
		        exit(EXIT_FAILURE);
		    }
        	write(fd,"1",sizeof(SYSFS_GPIO_RST_VAL_H));                 //"echo 1 > RST_VAL"设置高电平
    
    		close(fd);
    	}
   }

}


//灭灯函数
void led_close(){  
   	int i;
   	for(i=0;i<16;i++){
	    fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	    if (fd ==-1) {
	        printf("could not open ERROR\n");
	        exit(EXIT_FAILURE);
    	}

	    write(fd,PIN_VAL[i],sizeof(PIN_VAL[i]));  //"echo PIN_VAL > export"写入编号
	    close(fd);
    
    	
	    if (fd==-1) {
	        printf("could not open ERROR1\n");
	        exit(EXIT_FAILURE);
	    }
	    write(fd,SYSFS_GPIO_RST_DIR_VAL,sizeof(SYSFS_GPIO_RST_DIR_VAL));  //"echo out > RST_DIR"定义方向
	    close(fd);

	    fd = open(RST_VAL[i], O_RDWR);
	    if (fd ==-1) {
	        printf("could not open ERROR2\n");
	        exit(EXIT_FAILURE);
	    }
        write(fd,"0",sizeof(SYSFS_GPIO_RST_VAL_L));   //"echo 0 > RST_VAL"设置低电平
   
    
    	close(fd);

  	}
}

//开关控制 
void functionKEY()
{
	int switches_fd;
	int switches;
	time_t start;
	start = time(NULL);
	
	while (difftime(time(NULL), start) < 20) 
	{
		//获取开关值
		volatile unsigned int * gpio;
		switches_fd = open("/dev/uio0", O_RDWR);
		if (switches_fd < 1) 
		{
			printf("could not open /dev/uio0\n");
			exit(EXIT_FAILURE);
		}

		
		gpio = (unsigned int *)mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, switches_fd, 0);
		gpio[1] = 0xFFFF; // write 0x00FF to memory at location 0x10C0_0004  
		switches = gpio[0];
		printf("%d\n", switches);

		//开关值赋给灯
		for (i = 15; i >= 0; i--) 
		{
			num[i] = switches % 2;
			printf("%d ", num[i]);
			switches = switches / 2;
		}
		printf("\n");
		index = 0;
		
		//显示亮灯规则
		led_open(0);  
		led_close();
	}	
}

//走马灯 
void functionLED()
{
	int index=0;
	for (i = 0; i < 16; i++)
	{
		num[i] = 0;
	}
	//设置初始亮灯	
	num[2] = 1;
    while(1)
	{
       led_close();
       index++;
       led_open(index);
       sleep(1);
    }
}

int main(void)
{	
	char str[10];
	gets(str);

	if(strcmp(str,"KEY") == 0)
	{
		functionKEY();
	}
	else if(strcmp(str,"LED") == 0)
	{
		functionLED();
	}
	
    return 0;
}

