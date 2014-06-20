#!/usr/bin/sh

rwmode_mknod()
{
	echo " * inserting mtd_rwmode_set module"
	insmod ../flash_rwmode_set.ko
	rm -f /dev/mtd_rwmode_set
	MTD_RWMODE_SET_MAJOR=`cat /proc/devices |grep mtd_rwmode_set|awk '{print $1}'`
	mknod /dev/mtd_rwmode_set c $MTD_RWMODE_SET_MAJOR 0
}

rwmode_rm()
{
	echo " * remove mtd_rwmode_set module"
	rmmod flash_rwmode_set
	rm -f /dev/mtd_rwmode_set
}

case $1 in
"mk") rwmode_mknod ;;
"rm") rwmode_rm;;
"rd") ../flash_rwattr_set/flash_setrdonly  "/dev/mtd_rwmode_set"  "/dev/mtd0";;
"rw") ../flash_rwattr_set/flash_setrdwr "/dev/mtd_rwmode_set"  "/dev/mtd0";;
esac

