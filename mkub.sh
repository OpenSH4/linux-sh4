#!/bin/bash 
export PATH=/opt/STM/STLinux-2.4/devkit/sh4/bin:$PATH
if [ "$1" = "7111" ];then
echo compile st7111 kernel
cp .config_7111 .config
fi

if [ "$1" = "7167" ];then
echo compile st7167 kernel
cp .config_7167 .config
fi
echo "Do U need make menuconfig?yes::1 no::0"

if [ -z $2 ];then
   read   mkmenu
else
   mkmenu=$2
fi

if [ $mkmenu -eq 1 ];then
make ARCH=sh CROSS_COMPILE=sh4-linux- menuconfig
fi

make ARCH=sh CROSS_COMPILE=sh4-linux- 

if [ "$1" = "7111" ];then
echo bak st7111 kernel config
cp .config .config_7111
fi
if [ "$1" = "7167" ];then
echo bak st7167 kernel config
cp .config .config_7167
fi

echo generate binary file from  vmlinux ...
sh4-linux-objcopy -O binary vmlinux vmlinux.bin
echo gzip vmlinux.bin...
gzip vmlinux.bin -f
echo make image file vmlinux.ub ...
export KERNEL_A=$(cat System.map |grep "A _text"|awk '{print $1}')
export KERNEL_E=$(cat System.map |grep "T _stext"|awk '{print $1}')

mkimage -A sh -O linux -T kernel -C gzip -a $KERNEL_A -e $KERNEL_E -n "kernel2.6" -d vmlinux.bin.gz vmlinux.ub
echo copy vmlinux.ub ~/workspace/box ...
cp vmlinux.ub ~/workspace/box
