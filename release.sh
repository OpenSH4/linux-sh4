
#echo $0

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

script_dir=`dirname $0 2>/dev/null`
#echo $script_dir

if [ -z $script_dir ]; then
	cd .
else
	cd $script_dir
fi


echo "Building kernel..."
if [ $platform = "7111" ]; then
	echo " $platform ..."
	source make7111.sh
elif [ $platform = "7167" ]; then
	echo " $platform ..."
	source make7167.sh
else
    echo "Unsupport this platform $platform"
    exit
fi

if [ -f mImage ]; then
	echo "Rm old mImage"
	rm mImage
else
	echo "Didn't have old mImage"
fi

if [ -z $2 ]; then
    echo "Warning you didn't provide the runtime path, use default runtime path"
	if [ $platform = "7111" ]; then
		echo "use /opt/STM/runtime_usb/runtime_usb.7111"
		source mkmimage.sh "/opt/STM/runtime_usb/runtime_usb.7111"
	elif [ $platform = "7167" ]; then
		echo "use /opt/STM/runtime_usb/runtime_usb.7167"
		source mkmimage.sh "/opt/STM/runtime_usb/runtime_usb.7167"
	fi
else
	source mkmimage.sh $2
fi

./release_to_dir.sh $platform 

echo "Done."
