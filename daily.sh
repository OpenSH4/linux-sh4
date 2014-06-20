
echo $0

script_dir=`dirname $0 2>/dev/null`
echo $script_dir

if [ -z $script_dir ]; then
	cd .
else
	cd $script_dir
fi

if [ -z $1 ]; then
    echo "Warning you didn't choose a platform, use default 7111"
    platform="7111"
else
    platform="$1"
fi

if [ $platform = "7111" ]; then
	echo " $platform ..."
	./release7111.sh
elif [ $platform = "7167" ]; then
	echo " $platform ..."
	./release7167.sh
else
    echo "Unsupport this platform $platform"
    exit
fi
