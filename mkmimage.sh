
if [ -z $1 ]; then
    echo "Warning you didn't provide the runtime path"
	return
fi

if [ "$1" = "none" ];then
    echo "Warning you didn't need the runtime path"
    return
fi

runtime_path=$1

sudo cp System.map vmlinux.bin.gz $runtime_path/kernel

ori_pwd=`pwd`

cd $runtime_path/script

sudo ./make_image.sh

cd $ori_pwd

cp $runtime_path/release/image/mImage ./
