#!/bin/sh
#负责解析函数列表并对.o文件的函数做替换

a=`cat ufsdvfs.txt | awk '{print $6}'`

if [ ! -f "libufsd_mipsel.o_shipped" ]; then
	echo "libufsd_mipsel.o_shipped文件不存在，退出"
	exit 1
fi

for i in $a
do
	mipsel-openwrt-linux-objcopy libufsd_mipsel.o_shipped --redefine-sym "$i"="$i""_hide"
done

##将部分内联函数全局化
mipsel-openwrt-linux-objcopy libufsd_mipsel.o_shipped --globalize-symbol  __bread
mipsel-openwrt-linux-objcopy libufsd_mipsel.o_shipped --globalize-symbol __getblk
