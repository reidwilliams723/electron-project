echo SCANNING FOR TSYSTEM BOARD
variable=`nmap -sP 192.168.0.* | awk '/Nmap scan report for/{printf $5;}/MAC Address:/{print " => "$4 " " $5;}' | grep "Technologic Systems" | awk '{print $1}'`
echo Found $variable
echo BOARD FOUND! BEGINING FLASH
ssh -o StrictHostKeyChecking=no root@${variable} './flash_emmc.sh solaris_emmc.tgz'
echo FLASH COMPLETED!

