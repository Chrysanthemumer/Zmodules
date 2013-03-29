#include <fcntl.h>
#include <linux/cdrom.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
int main (int argc, char* argv[])
{
printf("%s\n",argv[1]);
int fd = open(argv[1], O_RDWR|O_NONBLOCK);
printf("open result [%d]\n", fd);
if(fd<0)perror("ERROR open:");
int id = ioctl (fd, CDROMEJECT, 0);
printf("ioctl result [%d]\n", id);
if(id<0)perror("ERROR ioctl:");
//strerror( errno );
close (fd);
return 0;
}

