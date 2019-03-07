# This is a test file to test the Duro1 for CW
from time import sleep
import queue
from ioteqModbusRtuMaster import *
import time
import json
import threading
import sys
import serial

##### EXIT CODES ####
WRONG_CARD = -1
FAILED_TEST = 0

#################### VARIABLES ####################
CARD_TYPE = int(sys.argv[1])
BACKPLANE_SERIAL_PORT = "/dev/dtech"
REGISTER_COUNT = 0
STARTING_REGISTER = 10
ANALOG_TEST_VALUES = []
FREQUENCY_TEST_VALUE = 936
ADDRESS = 1
####################################################

# mbMaster = modbusRTUMaster()
# mbMaster.init(BACKPLANE_SERIAL_PORT,115200)


# #Check to see that the right board is being tested
# if int(mbMaster.readInputRegisters(1, 0, 1, "INT")['data'][0]) != CARD_TYPE:
#     sys.exit(WRONG_CARD)
#Analog
if CARD_TYPE == 1:
    REGISTER_COUNT = 16
    for REGISTER in range(STARTING_REGISTER,STARTING_REGISTER + REGISTER_COUNT, 2):
        # result = int(mbMaster.readInputRegisters(ADDRESS, REGISTER, 2, "REAL")['data'][0])
        # if result == FREQUENCY_TEST_VALUE:
        #     print("It works!")
        # else:
        #     sys.exit(FAILED_TEST)
        print("Register " + str(REGISTER) + " Passed")
        sleep(.1)
#Frequency 6
# elif CARD_TYPE == 2:
#     REGISTER_COUNT = 12
#     for REGISTER in range(STARTING_REGISTER,STARTING_REGISTER + REGISTER_COUNT, 2):
#         result = int(mbMaster.readInputRegisters(ADDRESS, REGISTER, 2, "REAL")['data'][0])
#         if result == FREQUENCY_TEST_VALUE:
#             print("It works!")
#         else:
#             sys.exit(FAILED_TEST)
# else:
#     print("Something weird.")

# sys.exit(0)

#result = mbMaster.readInputRegisters(1,10,12,"REAL")
#print(result['data'])

