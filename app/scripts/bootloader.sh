port='/dev/arduino'
avrDirectory='/Applications/Arduino.app/Contents/Java/hardware/tools/avr'

cd $avrDirectory

boardListOutput=$(./bin/avrdude -C ${avrDirectory}/etc/avrdude.conf -v -patmega2560 -carduino -P $port -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xD8:m -Ulfuse:w:0xFF:m)

echo $boardListOutput
exit 0