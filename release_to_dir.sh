
if [ -z $1 ]; then
    echo "Warning you didn't choose a platform, use default 7111"
	platform="7111"
else
    platform="$1"
fi

if [ $platform = "7111" ]; then
	echo "Choose platform $platform"
elif [ $platform = "7167" ]; then
	echo "Choose platform $platform"
else
	echo "Unsupport this platform $platform"
	exit
fi

git status
describe=`git describe --dirty --always --tags`

real_dir=output/kernel_`date +%y.%m.%d-%H.%M`_$describe

release_dir=release
release_modules_dir=$release_dir/modules

if [ -L $release_dir ]; then
    rm $release_dir
fi

if [ ! -d output ]; then
    mkdir output
fi

if [ -d $real_dir ]; then
    rm $real_dir -rf
fi

if [ -d $release_dir ]; then
	rm $release_dir -rf
fi

mkdir $real_dir
ln -s $real_dir $release_dir

echo "Release kernel..."
cp vmlinux $release_dir
cp vmlinux.bin.gz $release_dir
cp vmlinux.ub $release_dir
cp System.map $release_dir

cp mImage $release_dir

mkdir $release_modules_dir
echo "Release modules..."

if [ $platform = "7111" ]; then
	cp drivers/fulan/Flash_SetMode/flash_rwmode_set.ko $release_modules_dir
fi

if [ $platform = "7167" ]; then
    cp drivers/fulan/i2c_spi/i2s.ko $release_modules_dir
fi

cp drivers/fulan/vfd/vfd.ko $release_modules_dir
cp drivers/input/misc/uinput.ko $release_modules_dir
cp drivers/scsi/scsi_wait_scan.ko $release_modules_dir
cp drivers/usb/serial/option.ko $release_modules_dir
cp drivers/usb/serial/option.ko $release_modules_dir
cp drivers/usb/serial/pl2303.ko $release_modules_dir
cp drivers/usb/serial/sierra.ko $release_modules_dir
cp drivers/usb/serial/usb_wwan.ko $release_modules_dir


echo "Strip modules..."
sh4-linux-strip --strip-unneeded $release_modules_dir/*.ko

describe=`git describe --dirty --always --tags`
touch $release_dir/$describe

git log --graph --decorate --oneline > $release_dir/history.txt
