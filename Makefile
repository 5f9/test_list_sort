TARGET_MODULE := test_list_sort
obj-m := $(TARGET_MODULE).o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: main
	$(MAKE) -C $(KDIR) M=$(PWD) modules

main: main.c list_sort.c
	$(CC) -O0 -g -o main main.c list_sort.c

load:
	sudo insmod $(TARGET_MODULE).ko

unload:
	sudo rmmod $(TARGET_MODULE).ko

clean:
	-rm -rf *.o *.ko .*.cmd $(TARGET_MODULE).mod* modules.order Module.symvers main
 
