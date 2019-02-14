  #include "ADS1115Lib.h"
#include "ModbusLib.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 1
#define EXPANSION_TYPE 1   



// This is the Modbus Slave object that handles all modbus comms of the card
ModbusRTUSlave_t modbusRTUSlave;
// This is the Modbus Address of the card. This is read from the main board
unsigned int BOARD_ADDR;

// These are the registers mapped to Modbus
//unsigned int holdingRegisters[100];
unsigned int inputRegisters[30];


Adafruit_ADS1115 ads1(0x48);
Adafruit_ADS1115 ads2(0x49);

// This is used by Modbus to shutdown the RTS upon transmission. This has to be encapsulated in the 
// Modbus Library
ISR(USART3_TX_vect)
{
  RTS_OFF;
}


void setup() {
    // Set PB7 as Output
    DDRB |= 1 << DDB7;
    DDRB |= 1 << DDB6;

    // Turn ON Both LEDS while in setup mode
    PORTB &= ~(1 << PORTB6);
    PORTB &= ~(1 << PORTB7);


    // Set port H as inputs and read Board address from the main board
    DDRH = 0x00;
    BOARD_ADDR = PINH  & 0x1F;

    // Receiver Enable Configuration as output and setup as 0
    DDRJ |= 1 << DDJ2;
    PORTJ &= ~(1 << PORTJ2); 

    // Driver Enable Configuration as output
    DDRJ |= 1 << DDJ3;
    //PORTJ |= 1 << PORTJ3;
    PORTJ &= ~(1 << PORTJ3); 



    // Enable the interrups on the Modbus port upon transmission end  
    _SFR_BYTE(UCSR3B) |= _BV(TXCIE3);        //Enable TXCIE3 interrupts

    Serial.begin(115200);
    //Serial.begin(230400);
    //pinMode(LED,OUTPUT);
    Serial.println("Starting UP!!!");
    Serial.print("My address on the bus is: ");
    Serial.println(BOARD_ADDR);



    //modbusRTUSlave.pHoldingRegisters = holdingRegisters;
    modbusRTUSlave.pHoldingRegisters = inputRegisters;
    modbusRTUSlave.pInputRegisters = inputRegisters;
    modbusRTUSlave.pInputStatus = inputRegisters;
    modbusRTUSlave.pCoils = inputRegisters;
    modbusRTUSlave.hrCount = 0;
    modbusRTUSlave.irCount = sizeof(inputRegisters) >> 1;
    modbusRTUSlave.isCount = 0;
    modbusRTUSlave.coilsCount = 0;


    // Initialize Standard Values
    inputRegisters[0] = EXPANSION_TYPE;
    inputRegisters[1] = VERSION_MAJOR;
    inputRegisters[2] = VERSION_MINOR;



    
//    createModbusRTUSlave(&modbusRTUSlave,&Serial3,1000000,SERIAL_8N1,1,0,300);
    // Dont even try to run at 230400 because the Arduino cannot generate it reliable
    createModbusRTUSlave(&modbusRTUSlave,&Serial3,115200,SERIAL_8N1,BOARD_ADDR,0,300);


   ads1.begin();
   ads1.setGain(GAIN_TWO);
   ads2.begin();
   ads2.setGain(GAIN_TWO);



}

// This mA scale function numbers are based on:
// ADC range: 0 to 2.048 V
// Max count at 2.048: 32767
float mAscale(int16_t rawVal) {
  if (rawVal<0) return 0.0;
  float scaledV = rawVal * 2.048 / 32767;
  return scaledV * 10;  
}

void encodeFloat(unsigned int result[],float floatVal) {
   unsigned int *data;
   data = (unsigned int *)&floatVal;

   // Chck correct modbus mapping and make it Byte Wise
   result[0] = data[1];
   result[1] = data[0];
   
}





// Funcation to read analog input with no delay
// This will have to be revised, as it is is not reeentrant
int readAnaIn(Adafruit_ADS1115 &ads,int channel,int16_t &result)
{
  static int state = 0;
  static unsigned long lastTime = 0;

   
  switch (state) {
    case 0:
      ads.readADC_SingleEndedStart(channel);
      lastTime = millis();
      state = 1;
      return 0;
      break;

    case 1:
      if (timeDiff(lastTime)>8) {
        result = ads.readADC_SingleEndedRead();
        state = 0;
        return 1;
      }
      return 0;
  }

  
}





#define DEBUG_STATE_IDLE 0
#define DEBUG_STATE_HELP 1
#define DEBUG_STATE_FREQ 2
#define DEBUG_STATE_LOOP 3


float analogValues[8];

void checkDebugMenu()
{
    static int state = 0;
    static int counter = 0;
    char charIn = 0;

    switch (state) {
      case DEBUG_STATE_IDLE:
          if (Serial.available() > 0) {
               charIn = Serial.read();
          }
          switch (charIn) {
              case '?':
                  state = DEBUG_STATE_HELP;
                  break;
              case 'f':
                  state = DEBUG_STATE_FREQ;
                  break;
              case 'l':
                  counter = 0;
                  state = DEBUG_STATE_LOOP;
                  break;
            }
            break;
       case DEBUG_STATE_HELP: 
            Serial.println("Test Program");
            Serial.println("\t?      Show Help");
            Serial.println("\tf      Show Analogs");
            Serial.println("\tl      Show Analogs in a Loop");
            Serial.println("");
            state = DEBUG_STATE_IDLE;
            break;

       case DEBUG_STATE_FREQ: 
            Serial.print("Analog 1:");
            Serial.println(analogValues[0]);
            Serial.print("Analog 2:");
            Serial.println(analogValues[1]);
            Serial.print("Analog 3:");
            Serial.println(analogValues[2]);
            Serial.print("Analog 4:");
            Serial.println(analogValues[3]);
            Serial.print("Analog 5:");
            Serial.println(analogValues[4]);
            Serial.print("Analog 6:");
            Serial.println(analogValues[5]);
            Serial.print("Analog 7:");
            Serial.println(analogValues[6]);
            Serial.print("Analog 8:");
            Serial.println(analogValues[7]);
            state = DEBUG_STATE_IDLE;
            break;

       
       case DEBUG_STATE_LOOP: 
            Serial.print(analogValues[0],3);
            Serial.print("\t");
            Serial.print(analogValues[1],3);
            Serial.print("\t");
            Serial.print(analogValues[2],3);
            Serial.print("\t");
            Serial.print(analogValues[3],3);
            Serial.print("\t");
            Serial.print(analogValues[4],3);
            Serial.print("\t");
            Serial.print(analogValues[5],3);
            Serial.print("\t");
            Serial.print(analogValues[6],3);
            Serial.print("\t");
            Serial.print(analogValues[7],3);
            Serial.println("");
            counter++;
            if (counter>10) {
              counter = 0;
              state = DEBUG_STATE_IDLE;
            }
            break;

     }          
    
    
       

  
}




void loop() {
   unsigned long lastTime = millis();
   int16_t adcVal;

   int currentADC = 0;
   int currentChannel = 0;
   int i;

    // Turn LEDS off when starting main loop
    PORTB |= 1 << PORTB7;
    PORTB |= 1 << PORTB6;


 
   while (1) {
      // Attend the Modbus Protocol
      executeModbusRTUSlaveLoop(&modbusRTUSlave);
      checkDebugMenu();

      // Make on Conversion, just measure one channel per loop execution
      switch (currentChannel) {
        case 0:
          if (readAnaIn(ads2,3,adcVal)) {
              //Serial.println(adcVal);
              analogValues[0] = mAscale(adcVal);
              encodeFloat(inputRegisters+10,analogValues[0]);
              currentChannel++;
          }
          break;
        case 1:
          if (readAnaIn(ads2,2,adcVal)) {
              //Serial.println(adcVal);
              analogValues[1] = mAscale(adcVal);
              encodeFloat(inputRegisters+12,analogValues[1]);
              currentChannel++;
          }
          break;
        case 2:
          if (readAnaIn(ads2,1,adcVal)) {
              //Serial.println(adcVal);
              analogValues[2] = mAscale(adcVal);
              encodeFloat(inputRegisters+14,analogValues[2]);
              currentChannel++;
          }
          break;
        case 3:
          if (readAnaIn(ads2,0,adcVal)) {
              //Serial.println(adcVal);
              analogValues[3] = mAscale(adcVal);
              encodeFloat(inputRegisters+16,analogValues[3]);
              currentChannel++;
          }
          break;
        case 4:
          if (readAnaIn(ads1,3,adcVal)) {
              //Serial.println(adcVal);
              analogValues[4] = mAscale(adcVal);
              encodeFloat(inputRegisters+18,analogValues[4]);
              currentChannel++;
          }
          break;
        case 5:
          if (readAnaIn(ads1,2,adcVal)) {
              //Serial.println(adcVal);
              analogValues[5] = mAscale(adcVal);
              encodeFloat(inputRegisters+20,analogValues[5]);
              currentChannel++;
          }
          break;
        case 6:
          if (readAnaIn(ads1,1,adcVal)) {
              //Serial.println(adcVal);
              analogValues[6] = mAscale(adcVal);
              encodeFloat(inputRegisters+22,analogValues[6]);
              currentChannel++;
          }
          break;
        case 7:
          if (readAnaIn(ads1,0,adcVal)) {
              //Serial.println(adcVal);
              analogValues[7] = mAscale(adcVal);
              encodeFloat(inputRegisters+24,analogValues[7]);
              currentChannel++;
          }
          break;
        default:
          currentChannel = 0;
      }

 
             
      // Handle LEDs
      if (timeDiff(lastTime)>50) {
        if (PORTB & (1 << PORTB6)) {
          PORTB &= ~(1 << PORTB6);
        } else {
          PORTB |= 1 << PORTB6;
        }
        lastTime = millis();
        //Serial.println("Time Elapsed!");
      }
      //executeModbusRTUSlaveLoop(&modbusRTUSlave);


   }


}


// Led handling code
/*
 *       if (timeDiff(lastTime)>500) {
        if (PORTB & (1 << PORTB7)) {
          PORTB &= ~(1 << PORTB7);
          //PORTB |= 1 << PORTB6;
        } else {
          PORTB |= 1 << PORTB7;
          //PORTB &= ~(1 << PORTB6);          
        }
        lastTime = millis();
        //Serial.println("Time Elapsed!");
      }

 */

