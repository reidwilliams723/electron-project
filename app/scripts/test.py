# This is a test file to test the Duro1 for CW
from time import sleep
import queue
from ioteqModbusRtuMaster import *
import time
import json
import threading

import serial



BACKPLANE_SERIAL_PORT = "/dev/cu.usbserial"
#BACKPLANE_SERIAL_PORT = "/dev/cu.usbmodem1411"

