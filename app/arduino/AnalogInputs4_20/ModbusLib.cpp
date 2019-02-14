#include <arduino.h>

// ************************************************************************************
// ******************************* Modbus Library *************************************





#define CMD_ERROR_NO_ERROR            0
#define CMD_ERROR_TIMEOUT            -1
#define CMD_ERROR_INTERCHAR_TIMEOUT  -2
#define CMD_ERROR_CRC                -3
#define CMD_ERROR_INVALID_FUNCTION   -4
#define CMD_ERROR_INVALID_IPADDR   -5
#define CMD_ERROR_UNABLE_TO_CONNECT  -6
#define CMD_ERROR_CLOSED_CONNECTION  -7



#define CMD_ERROR_INTERNAL_ERROR_1   -100
#define CMD_ERROR_INTERNAL_ERROR_2   -101


/* Possible Modbus Exception Errors */
#define MB_EXCEPTION_ILLEGAL_FUNCTION        1
#define MB_EXCEPTION_ILLEGAL_DATA_ADDRESS    2
#define MB_EXCEPTION_ILLEGAL_DATA_VALUE      3
#define MB_EXCEPTION_SLAVE_DEVICE_FAILURE    4





/* Valores de los diversos comandos MODBUS en un mensaje            */
#define MB_FUNC_READ_COILS             1
#define MB_FUNC_READ_INPUT_STATUS        2
#define MB_FUNC_READ_HOLDING_REGISTERS     3
#define MB_FUNC_READ_INPUT_REGISTERS       4
#define MB_FUNC_WRITE_SINGLE_COIL          5
#define MB_FUNC_WRITE_SINGLE_REGISTER      6

#define MB_FUNC_WRITE_MULTIPLE_COILS      15
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS  16

#define ReadExceptionStatus     7
#define ExecuteDiagnostics      8
#define FetchCommEventCounter   11
#define FetchCommEventLog       12
#define ReportSlaveID           17
#define ReadGeneralReference    20
#define WriteGeneralReference   21
#define MaskWrite4xRegister     22
#define ReadWrite4xRegisters    23
#define ReadFIFOQueue           24







#define MB_RESULT_OK  0
#define MB_RESULT_BUSY  1
#define MB_RESULT_INVALID_STATE -1






#define MBRTUSLAVE_STATE_IDLE     0
#define MBRTUSLAVE_STATE_RX     1
#define MBRTUSLAVE_STATE_START_TX 2
#define MBRTUSLAVE_STATE_TX     3



int tx_ended = 1;
//#define RTS_ON       PORTB |= 1 << PORTB7
//#define RTS_OFF      if (tx_ended) { PORTB &= ~(1 << PORTB7); }

#define RTS_ON       PORTJ |= 1 << PORTJ3
#define RTS_OFF      if (tx_ended) { PORTJ &= ~(1 << PORTJ3); }





/* Value tables for fast CRC16 calculation */
const unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;

/* Value tables for fast CRC16 calculation */
const unsigned char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};





/*-----------------------------------------------------------*/
/* CRC16 calculation function                                */
/*-----------------------------------------------------------*/
void CRC16(unsigned char  *puchMsg, unsigned short usDataLen,unsigned char crc[2])

/*  *puchMsg        mensaje al que se le va a calcular el CRC16 */
/*  usDataLen        cantidad de bytes en el mensaje             */

{
  unsigned char uchCRCHi = 0xFF ;   /* byte alto del CRC inicializado en FF */
  unsigned char uchCRCLo = 0xFF ;   /* byte bajo del CRC inicializado en FF */
  unsigned uIndex ;         /* indice para la tabla del CRC16 */
  while (usDataLen--)      /* recorre el buffer del mensaje  */
  {
    uIndex = uchCRCHi ^ *puchMsg++ ;   /* calcula el CRC */
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
    uchCRCLo = auchCRCLo[uIndex] ;
  }
  crc[0] = uchCRCHi;
  crc[1] = uchCRCLo;
}





typedef struct modbusRTUSlave {
  // Change this to arduino serial port class
   HardwareSerial *serialPort;
  int   state;

  unsigned long lastStateTime; // Time that the last state machine event happened
  unsigned char buffer[280]; // Buffer used for Transmission and Reception
  unsigned int  rxIndex;

  unsigned long lastResponseTime; // Time that the last complete transmission happened

  long long requests;
  long long responsesOK;
  long long responsesERR;

  unsigned int  txSize;
  unsigned int  txIndex; // Number of bytes sent

  unsigned char lastCmd;
  unsigned short lastCmdBitCount;

  int cmdLastError;  // Last error for a modbus command in this port

  long interCharTimeout;
  long responseDelay;

  long charTime;
  long modbus35Gap;

  int slaveID;


   short int *pHoldingRegisters;
   int hrCount;

   short int *pInputRegisters;
   int irCount;

   unsigned short *pInputStatus;
   int isCount;

   unsigned short *pCoils;
   int coilsCount;


} ModbusRTUSlave_t;



int executeModbusRTUSlaveLoop(ModbusRTUSlave_t *mbSlave);
void respondModbusRTURequest(ModbusRTUSlave_t *mbSlave);
int generateModbusRTUResponse(ModbusRTUSlave_t *mbSlave, unsigned char functionCode, unsigned short startAddr, unsigned short regCount, unsigned char *data);
int createModbusRTUSlave(
    ModbusRTUSlave_t *mbSlave,
    HardwareSerial *pSerialDevice,
    long  baudrate,
    long serialConfig,
    int   slaveID,
    long  responseDelay,
    long  interCharTimeout);




// ************************************************************************************
// ***************************** General Functions ************************************



unsigned long timeDiff(unsigned long prevTime) {
   unsigned long currentTime = millis();
   if (currentTime >= prevTime) {
      return currentTime - prevTime;
   } else {
      return ((unsigned long)0xFFFFFFFF - prevTime) + currentTime;
   }
}


// ***************************** General Functions ************************************
// ************************************************************************************






int createModbusRTUSlave(
    ModbusRTUSlave_t *mbSlave,
    HardwareSerial *pSerialDevice,
    long  baudrate,
    long serialConfig,
    int   slaveID,
    long  responseDelay,
    long  interCharTimeout)

{


  // Save the configured timeouts
  //mbServerConfig->connectionTimeout = connTimeout;
  mbSlave->requests = 0;
  mbSlave->responsesOK = 0;
  mbSlave->responsesERR = 0;

  mbSlave->serialPort = pSerialDevice;

  pSerialDevice->begin(baudrate,serialConfig);


  // Save the configured timeouts
  mbSlave->interCharTimeout = interCharTimeout;
  mbSlave->responseDelay = responseDelay;

  // calculate the character time
  // For now hardcode databits and parity
  //long bitsTime = databits + stopbits +1; // +1 is the start bit
  //if ((*parity!='N') && (*parity!='n')) bitsTime++;
  long bitsTime = 10;
  

  // Multiplied by 1,000 because we want the time in milliSeconds
  mbSlave->charTime = bitsTime * 1000 / baudrate;
  mbSlave->modbus35Gap  = bitsTime * 3500 / baudrate ;

  // Just a test
  //mbSlave->modbus35Gap  =   10* mbSlave->modbus35Gap;

  mbSlave->slaveID  = slaveID;



  mbSlave->state = MBRTUSLAVE_STATE_IDLE;
  mbSlave->lastStateTime = millis();

  return 0;
}






int generateModbusRTUResponse(ModbusRTUSlave_t *mbSlave, unsigned char functionCode, unsigned short startAddr, unsigned short regCount, unsigned char *data)
{
  int i;
  int swapCode;
  long mbBasedOffset;      // Modbus Based Address Offset
  long bufferBasedOffset;  // buffer Based request offset
  unsigned char tempByte;
  int responseLength; // Modbus Response Data Length
      int wordIndex = 0;
      int byteIndex = 0;

  //printf("In generateModbusResponse, functionCode: %d, startAddr: %d, regCount: %d\n\n",functionCode,startAddr,regCount);


   switch (functionCode)
   {
   case MB_FUNC_READ_COILS:
   case MB_FUNC_READ_INPUT_STATUS:
   /*
      // Determine the Modbus Addr based offset for this request
      mbBasedOffset = startAddr - modbusMapping[i].mbAddrStart;
      bufferBasedOffset = modbusMapping[i].bufferOffset + mbBasedOffset;
      //bitarray_copy(modbusMapping[i].buffer, bufferBasedOffset, regCount, data, 0);
      //bitarray_copy_word((unsigned short *)modbusMapping[i].buffer, bufferBasedOffset, regCount, (unsigned short *)data, 0);
      bitarray_copy_toMB(modbusMapping[i].buffer, data, bufferBasedOffset, regCount);
      responseLength = (regCount/8) + ((regCount%8>0)?1:0);
      return responseLength;
      break;*/

   case MB_FUNC_READ_HOLDING_REGISTERS:
      if (regCount>127) return 0;
      if ((startAddr + regCount)>mbSlave->hrCount) return 0;

      wordIndex = 0;
      byteIndex = 0;
      for (wordIndex=0;wordIndex<regCount;wordIndex++) {
         data[byteIndex] = (mbSlave->pHoldingRegisters[wordIndex+startAddr] & 0xFF00) >> 8;
         data[byteIndex+1] = (mbSlave->pHoldingRegisters[wordIndex+startAddr] & 0xFF);
         byteIndex += 2;         
      }

      return regCount*2;
   case MB_FUNC_READ_INPUT_REGISTERS:
      if (regCount>127) return 0;
      if ((startAddr + regCount)>mbSlave->irCount) return 0;

      wordIndex = 0;
      byteIndex = 0;
      for (wordIndex=0;wordIndex<regCount;wordIndex++) {
         data[byteIndex] = (mbSlave->pInputRegisters[wordIndex+startAddr] & 0xFF00) >> 8;
         data[byteIndex+1] = (mbSlave->pInputRegisters[wordIndex+startAddr] & 0xFF);
         byteIndex += 2;         
      }

      return regCount*2;


   case MB_FUNC_WRITE_SINGLE_REGISTER:
      if (startAddr >= mbSlave->hrCount) return 0;
      mbSlave->pHoldingRegisters[startAddr] =  data[1] | data[0]<<8;
      return 2;


   case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
      if ((startAddr+regCount) > mbSlave->hrCount) return 0;
      byteIndex=0;
      for (wordIndex=0;wordIndex<regCount;wordIndex++) {
         mbSlave->pHoldingRegisters[startAddr+wordIndex] =  data[byteIndex+1] | data[byteIndex]<<8;
         byteIndex+=2;
      }
      return regCount*2;
   }



/*
   case MB_FUNC_WRITE_SINGLE_COIL:
      // Determine the Modbus Addr based offset for this request
      mbBasedOffset = startAddr - modbusMapping[i].mbAddrStart;
      bufferBasedOffset = modbusMapping[i].bufferOffset + mbBasedOffset;
      if ((data[1] == 0x00) && (data[0] == 0xFF)) {
         tempByte = 0xFF;
      } else {
         tempByte = 0x00;
      }
      bitarray_copy_fromMB(&tempByte,modbusMapping[i].buffer,bufferBasedOffset,1);
      //bitarray_copy_word(&tempByte,0,1,modbusMapping[i].buffer, bufferBasedOffset);
      return 2; // 2 bytes of data copies the same value as the input

   case MB_FUNC_WRITE_SINGLE_REGISTER:
      // This should later become an option of each mapped set of registers
      swapCode = SWAP_CODE_NONE;
      //swapCode = SWAP_CODE_BYTES_BIT;
      mbBasedOffset = startAddr - modbusMapping[i].mbAddrStart;
      bufferBasedOffset = modbusMapping[i].bufferOffset + mbBasedOffset*2; // This offset is in Bytes so it must be multiplied by two
      swapMemcpy(swapCode, &modbusMapping[i].buffer[bufferBasedOffset],data, 2);
      return 2;

   case MB_FUNC_WRITE_MULTIPLE_COILS:
      // Determine the Modbus Addr based offset for this request
      mbBasedOffset = startAddr - modbusMapping[i].mbAddrStart;
      bufferBasedOffset = modbusMapping[i].bufferOffset + mbBasedOffset;
      //printf("bitarray_copy_word(data,0,%d,modbusMapping[i].buffer, %d);\n",regCount,bufferBasedOffset);
      //bitarray_copy_word(data,0,regCount,modbusMapping[i].buffer, bufferBasedOffset);
      bitarray_copy_fromMB(data,modbusMapping[i].buffer,bufferBasedOffset,regCount);
      responseLength = (regCount/8) + ((regCount%8>0)?1:0);
      return responseLength;

   case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
      //swapCode = SWAP_CODE_BYTES_BIT;
      // This should later become an option of each mapped set of registers
      swapCode = SWAP_CODE_NONE;
      mbBasedOffset = startAddr - modbusMapping[i].mbAddrStart;
      bufferBasedOffset = modbusMapping[i].bufferOffset + mbBasedOffset*2; // This offset is in Bytes so it must be multiplied by two
      swapMemcpy(swapCode, &modbusMapping[i].buffer[bufferBasedOffset],data, regCount*2);
      return regCount*2;
   }
*/


  return 0;
}




void respondModbusRTURequest(ModbusRTUSlave_t *mbSlave)
{
  //unsigned int transactionID = mbServer->buffer[1] | (mbServer->buffer[0] << 8);
  //unsigned int protocolID = mbServer->buffer[3] | (mbServer->buffer[2] << 8);
  //unsigned int dataLength = mbServer->buffer[5] | (mbServer->buffer[4] << 8);
  //unsigned int unitID = mbServer->buffer[6];
  unsigned int functionCode = mbSlave->buffer[1];

  unsigned int startAddr = 0;
  unsigned int regCount = 0;

  unsigned char crcResult[2];
  //unsigned int value = 0;
  //unsigned char *dataBuffer;

  int copiedCount;

  mbSlave->requests++;


  switch (functionCode)
  {
  case MB_FUNC_READ_COILS:
  case MB_FUNC_READ_INPUT_STATUS:
  case MB_FUNC_READ_HOLDING_REGISTERS:
  case MB_FUNC_READ_INPUT_REGISTERS:
    startAddr = mbSlave->buffer[3] | (mbSlave->buffer[2] << 8);
    regCount = mbSlave->buffer[5] | (mbSlave->buffer[4] << 8);
    //Serial.println("Received MB Function: Start: Count: \n");
    //Serial.print(functionCode);
    //Serial.print(" ");
    //Serial.print(startAddr);
    //Serial.print(" ");
    //Serial.println(regCount);
    copiedCount = generateModbusRTUResponse(mbSlave,functionCode,startAddr,regCount,&mbSlave->buffer[3]);
    if (copiedCount<=0) { // Invalid address requested
      mbSlave->buffer[1] |= 0x80; // Signal exception
      mbSlave->buffer[2] = MB_EXCEPTION_ILLEGAL_DATA_ADDRESS;

      CRC16(mbSlave->buffer,3,crcResult);
      mbSlave->buffer[3] = crcResult[0];
      mbSlave->buffer[4] = crcResult[2];

      mbSlave->txSize = 5;
      mbSlave->responsesERR++;
      break;
    }
    mbSlave->buffer[2] = copiedCount;

    CRC16(mbSlave->buffer,copiedCount + 3,crcResult);
    mbSlave->buffer[3+copiedCount] = crcResult[0];
    mbSlave->buffer[4+copiedCount] = crcResult[1];

    mbSlave->txSize = 5 + copiedCount;
    mbSlave->responsesOK++;

    break;

  case MB_FUNC_WRITE_SINGLE_COIL:
  case MB_FUNC_WRITE_SINGLE_REGISTER:
    startAddr = mbSlave->buffer[3] | (mbSlave->buffer[2] << 8);
    //Serial.println("Received Write Single Register");
    //Serial.println(functionCode);
    //Serial.println(startAddr);
    
    copiedCount = generateModbusRTUResponse(mbSlave,functionCode,startAddr,1,&mbSlave->buffer[4]);
    if (copiedCount<=0) { // Invalid address requested
      mbSlave->buffer[1] |= 0x80; // Signal exception
      mbSlave->buffer[2] = MB_EXCEPTION_ILLEGAL_DATA_ADDRESS;

      CRC16(mbSlave->buffer,3,crcResult);
      mbSlave->buffer[3] = crcResult[0];
      mbSlave->buffer[4] = crcResult[1];

      mbSlave->txSize = 5;
      mbSlave->responsesERR++;
      break;
    }

    //CRC16(mbSlave->buffer,copiedCount + 3,crcResult);
    //mbSlave->buffer[3+copiedCount] = crcResult[0];
    //mbSlave->buffer[4+copiedCount] = crcResult[2];

    mbSlave->txSize = 8;
    mbSlave->responsesOK++;

    break;


  case MB_FUNC_WRITE_MULTIPLE_COILS:
  case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
    startAddr = mbSlave->buffer[3] | (mbSlave->buffer[2] << 8);
    regCount = mbSlave->buffer[5] | (mbSlave->buffer[4] << 8);
    //printf("Received MB Function: %d Start: %d Count: %d\n",functionCode,startAddr,regCount);
    //Serial.println(" Received MB_FUNC_WRITE_MULTIPLE_REGISTERS");

    copiedCount = generateModbusRTUResponse(mbSlave,functionCode,startAddr,regCount,&mbSlave->buffer[7]);
    //Serial.println("X");
    if (copiedCount<=0) { // Invalid address requested
      //Serial.println("Invalid address requested");
      mbSlave->buffer[1] |= 0x80; // Signal exception
      mbSlave->buffer[2] = MB_EXCEPTION_ILLEGAL_DATA_ADDRESS;

      CRC16(mbSlave->buffer,3,crcResult);
      mbSlave->buffer[3] = crcResult[0];
      mbSlave->buffer[4] = crcResult[1];

      mbSlave->txSize = 5;
      mbSlave->responsesERR++;
      break;
    }

    CRC16(mbSlave->buffer,6,crcResult);
    mbSlave->buffer[6] = crcResult[0];
    mbSlave->buffer[7] = crcResult[1];

    mbSlave->txSize = 8;
    mbSlave->responsesOK++;
    break;

  default:
    // Invalid Function
    mbSlave->buffer[1] |= 0x80; // Signal exception
    mbSlave->buffer[2] = MB_EXCEPTION_ILLEGAL_FUNCTION;

    CRC16(mbSlave->buffer,3,crcResult);
    mbSlave->buffer[3] = crcResult[0];
    mbSlave->buffer[4] = crcResult[1];

    mbSlave->txSize = 5;
    mbSlave->responsesERR++;
    break;
  }


}

/*
void hexview(unsigned char *buffer,int count){
  int i =0;
  for (i=0;i<count;i++) {
    Serial.print(buffer[i],HEX);
    Serial.print(" ");
  }
  Serial.println("");
}
*/


int executeModbusRTUSlaveLoop(ModbusRTUSlave_t *mbSlave)
{
  int charsRead;
   int writeAvailable;
   int bytesToSend;
  // Process all existing sockets
  switch (mbSlave->state) {

  case MBRTUSLAVE_STATE_IDLE:
    //printf("MBRTUSLAVE: IN IDLE STATE\n");
    mbSlave->rxIndex = 0;
    mbSlave->state = MBRTUSLAVE_STATE_RX;
      mbSlave->lastStateTime = millis();
    return mbSlave->state;
    break;


  case MBRTUSLAVE_STATE_RX: // Wait for a request

      mbSlave->serialPort->setTimeout(0);
    charsRead = mbSlave->serialPort->readBytes((char *)&mbSlave->buffer[mbSlave->rxIndex],(int)(sizeof(mbSlave->buffer) - mbSlave->rxIndex));

  

    //charsRead = readData(&mbPort->serialPort,&mbPort->buffer[mbPort->rxIndex],sizeof(mbPort->buffer) - mbPort->rxIndex);
    // If no chars read check the timeout
    if (charsRead<=0) {
      // The timeout is different if characters have ever been received or not
      if (mbSlave->rxIndex>0) {
        if ( timeDiff(mbSlave->lastStateTime) > mbSlave->interCharTimeout ) {
/*          Serial.println("interCharTimeoutError!!!!!!!!!\n");
          Serial.print("Timeout: ");
          Serial.println(mbSlave->interCharTimeout); 
          Serial.print("rxIdex: ");
          Serial.println(mbSlave->rxIndex); 
          Serial.print(mbSlave->buffer[0],HEX);
          Serial.print(mbSlave->buffer[1],HEX);
          Serial.print(mbSlave->buffer[2],HEX);
          Serial.print(mbSlave->buffer[3],HEX);
          Serial.print(mbSlave->buffer[4],HEX);
          Serial.println("");*/
          // If timeout, the error is registered
          // The state changes to idle and the last event time is registered
          mbSlave->cmdLastError = CMD_ERROR_INTERCHAR_TIMEOUT;
          mbSlave->state = MBRTUSLAVE_STATE_IDLE;
          mbSlave->lastStateTime = millis();
          return mbSlave->state;
        }
      }
    } else {
      //Serial.println("Received Data");
      // Update time when characters are received
      mbSlave->lastStateTime = millis();
      //Serial.print("Received bytes");
      //Serial.println(charsRead);
      mbSlave->rxIndex += charsRead;
      //printf("MBRTUSLAVE_STATE_RX Chars Received rxIndex=%d\n",mbPort->rxIndex);
      if (mbSlave->rxIndex > 5) {
        int expectedByteCount = 4;
        // Expected length varies by command
        switch (mbSlave->buffer[1]) {
        case MB_FUNC_READ_COILS:
        case MB_FUNC_READ_INPUT_STATUS:
        case MB_FUNC_READ_INPUT_REGISTERS:
        case MB_FUNC_READ_HOLDING_REGISTERS:
        case MB_FUNC_WRITE_SINGLE_COIL:
        case MB_FUNC_WRITE_SINGLE_REGISTER:
          expectedByteCount = 8;
          break;
        case MB_FUNC_WRITE_MULTIPLE_COILS:
        case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
          //Serial.println(" HERE 123");
          if (mbSlave->rxIndex > 5)
          expectedByteCount = mbSlave->buffer[6] + 9;
          break;
        }
        //Serial.print("Expected Byte Count:");
        //Serial.println(expectedByteCount);
        if (mbSlave->rxIndex >= expectedByteCount) {
          // If we got to this point, it means we have a full modbus packet in the buffer. We should calculate the CRC now
          unsigned char crcResult[2];
          CRC16(mbSlave->buffer,mbSlave->rxIndex-2,crcResult);

          if ((crcResult[0]==mbSlave->buffer[mbSlave->rxIndex-2]) &&
            (crcResult[1]==mbSlave->buffer[mbSlave->rxIndex-1])) {
            //Serial.println("MBRTUSLAVE: CRC OK!!! Responding!\n");

            // Now check the Slave ID. If is not for us we do not respond
            if (mbSlave->buffer[0] == mbSlave->slaveID) {
              //Serial.println("My Modbus Address!!!");
              respondModbusRTURequest(mbSlave);
//              mbSlave->state = MBRTUSLAVE_STATE_START_TX;
              mbSlave->state = MBRTUSLAVE_STATE_TX;
              RTS_ON;
              tx_ended = 0;
              mbSlave->txIndex = 0;
            } else {
              //Serial.println("Not My Modbus Address");
              mbSlave->state = MBRTUSLAVE_STATE_IDLE;
              mbSlave->lastStateTime = millis();
            }
            return mbSlave->state;
          } else {
            mbSlave->cmdLastError = CMD_ERROR_CRC;
          }

          //Serial.println("MBRTUSLAVE CRC Error!!!");

          //printf("MBRTUSLAVE CRC Error!!! Received: %02X %02X Expected: %02X %02X\n",mbPort->buffer[mbPort->rxIndex-2],mbPort->buffer[mbPort->rxIndex-1],crcResult[0],crcResult[1]);
          //printf("Extra Data: rxIndex: %d \n",mbPort->rxIndex);
          //hexview(mbSlave->buffer, mbSlave->rxIndex);


          mbSlave->state = MBRTUSLAVE_STATE_IDLE;
          mbSlave->lastStateTime = millis();
          return mbSlave->state;
        }
      }
    }
    return mbSlave->state;
    break;


/*  case MBRTUSLAVE_STATE_START_TX:
      mbSlave->state = MBRTUSLAVE_STATE_TX;
      RTS_ON;
      break;
*/
  case MBRTUSLAVE_STATE_TX:
    //charsRead = readData(&mbPort->serialPort,mbPort->dataAnalyzerTempBuffer,sizeof(mbPort->dataAnalyzerTempBuffer));
    //if (charsRead>0) getMonotonicTime(&mbPort->lastStateTime);
    //Serial.println("IN TX STATE");
    if (timeDiff(mbSlave->lastStateTime) > mbSlave->responseDelay) {
      //Serial.println("PreTransmision expired!\n");
      // Calculate the timeout plus the current message send time. This way we do not
      // have to wait until the message is sent to start counting the timeout
      //mbPort->timeoutPlusSendTime = mbPort->timeout + mbPort->txSize*mbPort->charTime;
      //mbPort->rxIndex = 0;
         if (mbSlave->txIndex < mbSlave->txSize) {
            //Serial.println("Buffer Not Empty");
            writeAvailable = mbSlave->serialPort->availableForWrite();
            bytesToSend = (mbSlave->txSize - mbSlave->txIndex) >= writeAvailable ? writeAvailable : (mbSlave->txSize - mbSlave->txIndex); 
            writeAvailable = mbSlave->serialPort->write(&mbSlave->buffer[mbSlave->txIndex],bytesToSend);
            mbSlave->txIndex += writeAvailable;

            if (mbSlave->txIndex >= mbSlave->txSize) tx_ended = 1;

         } else {
            //Serial.println("Buffer Empty");
            mbSlave->lastStateTime = millis();
            mbSlave->state = MBRTUSLAVE_STATE_IDLE;
            //RTS_OFF;
         }
    }
    return mbSlave->state;
    break;

  default:  // We should never get here, but just in case close connection and reinitialize
    //Serial.println("Invalid State!");
    mbSlave->lastStateTime = millis();
    mbSlave->state = MBRTUSLAVE_STATE_IDLE;

  }


  return 0;
}



// ******************************* Modbus Library *************************************
// ************************************************************************************


