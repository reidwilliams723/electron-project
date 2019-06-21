#!/bin/bash

. "$(pwd)/app/scripts/path.config"

cd $AVR_DIRECTORY

echo ./bin/avrdude -C ${AVR_DIRECTORY}/etc/avrdude.conf  -patmega2560 -carduino -P $ARDUINO_PORT -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xD8:m -Ulfuse:w:0xFF:m | tee
./bin/avrdude -C ${AVR_DIRECTORY}/etc/avrdude.conf  -patmega2560 -carduino -P $ARDUINO_PORT -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xD8:m -Ulfuse:w:0xFF:m | tee

if [ ${PIPESTATUS[0]} -ne "0" ]; then
    exit 1
fi

echo ./bin/avrdude -C ${AVR_DIRECTORY}/etc/avrdude.conf  -patmega2560 -carduino -P $ARDUINO_PORT -b19200 -Uflash:w:$BOOTLOADER_FILE:i -Ulock:w:0x0F:m | tee
./bin/avrdude -C ${AVR_DIRECTORY}/etc/avrdude.conf  -patmega2560 -carduino -P $ARDUINO_PORT -b19200 -Uflash:w:$BOOTLOADER_FILE:i -Ulock:w:0x0F:m | tee

if [[ ${PIPESTATUS[0]} -ne "0" ]]; then
    echo ${PIPESTATUS[0]}
    exit 1
fi

exit 0