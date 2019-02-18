obj-m:= hashtable.o

all:
	make ARCH=arm CROSS_COMPILE=~/buildroot/output/host/bin/arm-linux- -C ~/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220 M=$(shell pwd) modules

clean: 
	make ~/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220 M=$(shell pwd) clean

	

