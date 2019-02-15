#Variables
boardDirectory="newFirmware/${1}"
port="/dev/cu.usbmodem142240"
firmwareRepo="/Users/reidwilliams/Repositories/ioteqKernel/${boardDirectory}"
fqbn='arduino:avr:mega'
core='arduino:avr'
projectDirectory="/Users/reidwilliams/Repositories/electron-project/electron-project/app/scripts"

cd $firmwareRepo
date && git pull -v

cd $projectDirectory

echo Updating index
./arduino-cli core update-index

echo Retrieving board list
boardListOutput=$(./arduino-cli board list)

if [[ $boardListOutput != *"Arduino/Genuino Mega or Mega 2560"* ]]; then
    echo Installing core $core
    ./arduino-cli core install $core

    echo Retrieving board list again
    boardListOutput=$(./arduino-cli board list)

    if [[ $boardListOutput != *"Arduino/Genuino Mega or Mega 2560"* ]]; then
        echo Upload Failed
        exit 1
    fi
fi

echo Compiling code
compileOutput=$(./arduino-cli compile --fqbn $fqbn  $firmwareRepo)
echo compileOutput
if [[ $compileOutput == *"Compilation Failed"* ]]; then
    echo Upload Failed
    exit 1
fi

echo Uploading code
uploadOutput=$(./arduino-cli upload -p $port --fqbn $fqbn $firmwareRepo --verbose)
echo $uploadOutput
if [[ $uploadOutput == *"Please compile first"* ]]; then
    echo Upload Failed
    exit 1
fi

rm "${firmwareRepo}/${1}.arduino.avr.mega.elf"
rm "${firmwareRepo}/${1}.arduino.avr.mega.hex"

echo Upload Success
