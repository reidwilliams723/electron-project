#!/bin/bash

# Configurations that are machine specific
. "$(pwd)/app/scripts/path.config"

#Variables
BOARD_DIRECTORY="${FIRMWARE_DIRECTORY}/${1}"
FQBN="arduino:avr:mega"
CORE="arduino:avr"

echo "Checking for firmware updates..."
cd $FIRMWARE_ROOT_DIRECTORY
#sleep .5; git pull -v

# Move back to scripts directory
cd $SCRIPTS_DIRECTORY

# Using sleep commands so the piped output to the UI is more legible

sleep .5; echo "Updating index"
sleep .5; arduino-cli core update-index | tee

sleep .5; echo "Retrieving board list..."
sleep .5; arduino-cli board list | tee

sleep .5; echo "Installing core ${CORE}"
sleep .5; arduino-cli core install $CORE | tee

sleep .5; echo "Compiling code..."
sleep .5; arduino-cli compile --fqbn $FQBN  $BOARD_DIRECTORY | tee

# Check exit code of the compiled command, exit script if error occured
if [[ ${PIPESTATUS[0]} != 0 ]]; then
    exit 1
fi

# Setting a timeout...upload command won't exit at times if it fails. The duration can be set in path.config
sleep .5; echo "Uploading code..."
sleep .5; $TIMEOUT_COMMAND $TIMEOUT_DURATION arduino-cli upload -p $BOARD_PORT --fqbn $FQBN $BOARD_DIRECTORY --verbose | tee

#Remove compiled files from repo
rm "${BOARD_DIRECTORY}/${1}.arduino.avr.mega.elf"
rm "${BOARD_DIRECTORY}/${1}.arduino.avr.mega.hex"


# Check exit code of the upload command, exit script if error occured
if [[ ${PIPESTATUS[2]} != 0 ]]; then
    exit 1
fi

exit 0
