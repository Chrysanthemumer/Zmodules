Zmodules
========

#####Kernel Modules from basic to advanced
------------------------------------------

 1. hello_world runnable cross compiled for target to work.(link runtime dynamic lib to system root)
 2. hello_world .ko cross compiled against kernel source modules.symvers for target to work. (make ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi-)
 3. locate existing app and driver and make modifications
 4. hello_gpio app against existing driver
 5. hello_gpio ko against existing hardware 
 6. AXI bus 3 mode simple adder realization in VHDL
 7. Driver for above 3 adders.
