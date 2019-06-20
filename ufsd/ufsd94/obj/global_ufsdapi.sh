#!/bin/sh

func=`mipsel-openwrt-linux-objdump -t libufsd_mipsel.o_shipped | grep ufsdapi | awk {'print $7'}`

for i in $func; do
	 mipsel-openwrt-linux-objcopy libufsd_mipsel.o_shipped --globalize-symbol $i
done
	
