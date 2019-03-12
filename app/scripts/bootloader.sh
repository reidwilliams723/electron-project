#!/bin/bash

port='/dev/arduino'
avrDirectory='/home/ioteq/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino14'

 cd $avrDirectory

 boardListOutput=$(./bin/avrdude -C ${avrDirectory}/etc/avrdude.conf -v -patmega2560 -carduino -P $port -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xD8:m -Ulfuse:w:0xFF:m)

 echo $boardListOutput

boardListOutput=$(./bin/avrdude -C ${avrDirectory}/etc/avrdude.conf -v -patmega2560 -carduino -P/dev/ttyACM0 -b19200 -Uflash:w:/home/ioteq/Arduino/hardware/arduino/avr/bootloaders/stk500v2/stk500boot_v2_mega2560.hex:i -Ulock:w:0x0F:m)

echo boardListOutput

 exit 0