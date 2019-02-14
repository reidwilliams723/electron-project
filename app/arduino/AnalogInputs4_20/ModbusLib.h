
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



/* Modbus Command Types       */
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



extern int tx_ended;
#define RTS_ON       PORTJ |= 1 << PORTJ3
#define RTS_OFF      if (tx_ended) { PORTJ &= ~(1 << PORTJ3); }


void CRC16(unsigned char  *puchMsg, unsigned short usDataLen,unsigned char crc[2]);


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



unsigned long timeDiff(unsigned long prevTime);

// ***************************** General Functions ************************************
// ************************************************************************************







// ******************************* Modbus Library *************************************
// ************************************************************************************


