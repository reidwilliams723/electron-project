import serial
import math
import struct


def printPacket(packet):
   for b in packet:
      print(format(b, '02x'), end="", flush=True)
      print(" ", end="", flush=True)
   print("")


def parseDataAs(buffer,dataFormat):
   dataFormat = dataFormat.upper()
   data = ()
   if dataFormat == "INT":
      #print(">%dh" % int(len(buffer)/2))
      data =  struct.unpack( ">%dh" % int(len(buffer)/2), buffer) 
      #for i in range( int( len(buffer) / 2) ):
      #   data.append( struct.unpack( ">h", buffer[i*2:i*2+2])[0] )
   if dataFormat == "UINT":
      data =  struct.unpack( ">%dH" % int(len(buffer)/2), buffer) 

   if dataFormat == "REAL":
      data =  struct.unpack( ">%df" % int(len(buffer)/4), buffer) 

   return data


class modbusRTUMaster:

   crc_table = [0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0x0E40,0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,0x5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040]

   ERROR_NO_ERROR = 0
   ERROR_CRC = -1
   ERROR_TIMEOUT = -11

   def init(self,serialDevice,baudrate):
      self.serial = serial.Serial(serialDevice, baudrate,8,serial.PARITY_NONE,stopbits=1,timeout=.1)
      #ser.inter_byte_timeout
      #self.serial.open()
      self.pollList = []
      self.pollData = {}

   def calcCRC16Byte(self,data):

      crc_hi=0xFF
      crc_lo=0xFF

      for w in data:
         index=crc_lo ^ w
         crc_val=self.crc_table[index]
         crc_temp=int(crc_val/256)
         crc_val_low= int( crc_val-(crc_temp*256) )
         crc_lo=crc_val_low ^ crc_hi
         crc_hi=crc_temp

      crc=crc_hi*256 +crc_lo
      return crc


   def calcCRC16(self,data):

      crc_hi=0xFF
      crc_lo=0xFF

      for w in data:
         index=crc_lo ^ ord(w)
         crc_val=crc_table[index]
         crc_temp=crc_val/256
         crc_val_low=crc_val-(crc_temp*256)
         crc_lo=crc_val_low ^ crc_hi
         crc_hi=crc_temp

      crc=crc_hi*256 +crc_lo
      return crc


   def addPollCmd(self,deviceId,cmd,startRegister,count,dataType,dataNames):
      cmdData = { 
         "deviceId": deviceId,
         "cmd": cmd,
         "startRegister": startRegister,
         "count": count,
         "dataType": dataType,
         "dataNames": dataNames
      }
      self.pollList.append(cmdData)


   def readRegisters(self,function,deviceId,register,count,dataType):
      # Build Modbus Packet
      mbPacket = bytearray()
      mbPacket.append(deviceId)
      mbPacket.append(function & 0xFF)      # User defined cmd
      mbPacket.append((register & 0xFF00)>>8)
      mbPacket.append(register & 0xFF)
      mbPacket.append((count & 0xFF00)>>8)
      mbPacket.append(count & 0xFF)

      # Calculate and Add CRC at the end of packet
      crc = self.calcCRC16Byte(mbPacket)
      mbPacket.append(crc & 0xFF)
      mbPacket.append((crc & 0xFF00)>>8)

      #printPacket(mbPacket)

      self.serial.flushInput()
      # Send modbus request
      self.serial.write(mbPacket)
      remainingExpectedLen = 0
      # Wait only for header, this will allow calculating full packet length
      rxDataHeader = self.serial.read(3)
      if len(rxDataHeader) == 3:
         #print("Header: ",rxDataHeader)
         # Determine if regular or exception response
         if rxDataHeader[1] & 0x80 > 0 :
            print("Exception Response received")
            remainingExpectedLen = 2 # Only CRC is missing 
            rxCRC = self.serial.read(remainingExpectedLen)
            if len(rxCRC) != 2:
               return { "status": self.ERROR_TIMEOUT }
            rxCalcCrc = self.calcCRC16Byte(rxDataHeader)
            #print(format(rxCalcCrc,"04X"))
            rxCrc = (rxCRC[-1] << 8) | rxCRC[-2]
            if rxCalcCrc == rxCalcCrc:
               # Return modbus status
               return { "status": rxDataHeader[2] }
            else:
               return { "status": self.ERROR_CRC }

         else:
            remainingExpectedLen = rxDataHeader[2] + 2 # Expected is 2 bytes CRC plus data size

         rxDataAndCRC = self.serial.read(remainingExpectedLen)
         #print("Rest: ",rxDataAndCRC)
         if len(rxDataAndCRC)!=remainingExpectedLen:
            return { "status": self.ERROR_TIMEOUT }

         fullRxPacket = rxDataHeader + rxDataAndCRC
         packetNoCRC = fullRxPacket[:-2]
         rxCalcCrc = self.calcCRC16Byte(packetNoCRC)
         rxCrc = (fullRxPacket[-1] << 8) | fullRxPacket[-2]
         # Check that CRC matches
         if rxCalcCrc == rxCrc:
            # If CRC Matches
            return { "status": self.ERROR_NO_ERROR, "data": parseDataAs(rxDataAndCRC[:-2],dataType) }
         else:
            #If CRC Doesn't match
            return { "status": self.ERROR_CRC }
      else:
         # if different length received just return as timeout
         return { "status": self.ERROR_TIMEOUT }



   def readHoldingRegisters(self,deviceId,register,count,dataType):
      return self.readRegisters(0x03,deviceId,register,count,dataType) # 0x03 Read Holding Registers

   def readInputRegisters(self,deviceId,register,count,dataType):
      return self.readRegisters(0x04,deviceId,register,count,dataType) # 0x03 Read Holding Registers

   def writeHoldingRegister(self,deviceId,register,value):
      # Build Modbus Packet
      mbPacket = bytearray()
      mbPacket.append(deviceId)
      mbPacket.append(0x06)      # cmd Write Holding Registers
      mbPacket.append((register & 0xFF00)>>8)
      mbPacket.append(register & 0xFF)
      mbPacket.append((value & 0xFF00)>>8)
      mbPacket.append(value & 0xFF)

      # Calculate and Add CRC at the end of packet
      crc = self.calcCRC16Byte(mbPacket)
      mbPacket.append(crc & 0xFF)
      mbPacket.append((crc & 0xFF00)>>8)

      # Send modbus request
      self.serial.write(mbPacket)
      remainingExpectedLen = 0
      # Wait only for header, this will allow calculating full packet length
      rxDataHeader = self.serial.read(3)
      if len(rxDataHeader) == 3:
         # Determine if regular or exception response
         if rxDataHeader[1] & 0x80 > 0 :
            print("Exception Response received")
            remainingExpectedLen = 2 # Only CRC is missing 
            rxCRC = self.serial.read(remainingExpectedLen)
            if len(rxCRC) != 2:
               return { "status": self.ERROR_TIMEOUT }
            rxCalcCrc = self.calcCRC16Byte(rxDataHeader)
            #print(format(rxCalcCrc,"04X"))
            rxCrc = (rxCRC[-1] << 8) | rxCRC[-2]
            if rxCalcCrc == rxCrc:
               # Return modbus status
               return { "status": rxDataHeader[2] }
            else:
               return { "status": self.ERROR_CRC }

         else:
            remainingExpectedLen = 5 # Expected length for command 16 is 5

         rxDataAndCRC = self.serial.read(remainingExpectedLen)
         fullRxPacket = rxDataHeader + rxDataAndCRC
         packetNoCRC = fullRxPacket[:-2]
         rxCalcCrc = self.calcCRC16Byte(packetNoCRC)
         rxCrc = (fullRxPacket[-1] << 8) | fullRxPacket[-2]
         # Check that CRC matches
         if rxCalcCrc == rxCrc:
            # If CRC Matches
            return { "status": self.ERROR_NO_ERROR  }
         else:
            #If CRC Doesn't match
            return { "status": self.ERROR_CRC }
      else:
         # if different length received just return as timeout
         return { "status": self.ERROR_TIMEOUT }


   def writeMultipleHoldingRegisters(self,deviceId,register,values):
      regCount = len(values)
      # Build Modbus Packet
      mbPacket = bytearray()
      mbPacket.append(deviceId)
      mbPacket.append(0x10)      # cmd 16 Write Multiple Holding Registers
      mbPacket.append((register & 0xFF00)>>8)
      mbPacket.append(register & 0xFF)

      mbPacket.append((regCount & 0xFF00)>>8)
      mbPacket.append(regCount & 0xFF)

      mbPacket.append(regCount*2) # Byte Count

      # Now put the data
      for value in values:
         #print("Adding dat in MBPacket:",value)
         mbPacket.append((int(value) & 0xFF00)>>8)
         mbPacket.append(int(value) & 0xFF)


      # Calculate and Add CRC at the end of packet
      crc = self.calcCRC16Byte(mbPacket)
      mbPacket.append(crc & 0xFF)
      mbPacket.append((crc & 0xFF00)>>8)

      # Send modbus request
      self.serial.write(mbPacket)
      remainingExpectedLen = 0
      # Wait only for header, this will allow calculating full packet length
      rxDataHeader = self.serial.read(3)
      if len(rxDataHeader) == 3:
         # Determine if regular or exception response
         if rxDataHeader[1] & 0x80 > 0 :
            print("Exception Response received")
            remainingExpectedLen = 2 # Only CRC is missing 
            rxCRC = self.serial.read(remainingExpectedLen)
            if len(rxCRC) != 2:
               return { "status": self.ERROR_TIMEOUT }
            rxCalcCrc = self.calcCRC16Byte(rxDataHeader)
            #print(format(rxCalcCrc,"04X"))
            rxCrc = (rxCRC[-1] << 8) | rxCRC[-2]
            if rxCalcCrc == rxCrc:
               # Return modbus status
               return { "status": rxDataHeader[2] }
            else:
               return { "status": self.ERROR_CRC }

         else:
            remainingExpectedLen = 5 # Expected length for command 16 is 5

         rxDataAndCRC = self.serial.read(remainingExpectedLen)
         fullRxPacket = rxDataHeader + rxDataAndCRC
         packetNoCRC = fullRxPacket[:-2]
         rxCalcCrc = self.calcCRC16Byte(packetNoCRC)
         rxCrc = (fullRxPacket[-1] << 8) | fullRxPacket[-2]
         # Check that CRC matches
         if rxCalcCrc == rxCrc:
            # If CRC Matches
            return { "status": self.ERROR_NO_ERROR  }
         else:
            #If CRC Doesn't match
            return { "status": self.ERROR_CRC }
      else:
         # if different length received just return as timeout
         return { "status": self.ERROR_TIMEOUT }

   def runPoll(self):
      for cmd in self.pollList:
         print(cmd)
         if cmd['cmd'] == 3:
            returnedData = self.readHoldingRegisters(cmd['deviceId'],cmd['startRegister'],cmd['count'],cmd['dataType'])
            if returnedData['status'] == 0:
               for idx, val in enumerate(returnedData['data']):
                  if idx<len(cmd['dataNames']):
                     self.pollData[cmd['dataNames'][idx]] = val
                     print(cmd['dataNames'][idx] + " = ")
                     print(val)




