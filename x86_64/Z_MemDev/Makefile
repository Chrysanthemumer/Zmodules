
obj-m += Z_MemDev.o
Z_MemDev-objs := Z_MemDev_main.o Z_MemDev_fops.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	  
clean:
	make -C/lib/modules/$(shell uname -r)/build M=$(PWD) clean
