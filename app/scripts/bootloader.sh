#!/bin/bash

. "$(pwd)/app/scripts/path.config"

cd $AVR_DIRECTORY

./bin/avrdude -C ${AVR_DIRECTORY}/etc/avrdude.conf -v -patmega2560 -carduino -P $ARDUINO_PORT -b19200 -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xD8:m -Ulfuse:w:0xFF:m | tee

if [[ ${PIPESTATUS[0]} != 0 ]]; then
    exit 1
fi

./bin/avrdude -C ${AVR_DIRECTORY}/etc/avrdude.conf -v -patmega2560 -carduino -P $ARDUINO_PORT -b19200 -Uflash:w:${AVR_DIRECTORY}/bootloaders/stk500v2/stk500boot_v2_mega2560.hex:i -Ulock:w:0x0F:m | tee

if [[ ${PIPESTATUS[2]} != 0 ]]; then
    exit 1
fi

exit 0