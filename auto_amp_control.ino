#include <Arduino.h>
#include <EEPROM.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
#include <Dictionary.h>
IRsend irsend;

#define DECODE_DENON        // Includes Sharp
#define DECODE_JVC
#define DECODE_KASEIKYO
#define DECODE_PANASONIC    // the same as DECODE_KASEIKYO
#define DECODE_LG
#define DECODE_NEC          // Includes Apple and Onkyo
#define DECODE_SAMSUNG
#define DECODE_SONY
#define DECODE_RC5
#define DECODE_RC6

#define DECODE_BOSEWAVE
#define DECODE_LEGO_PF
#define DECODE_MAGIQUEST
#define DECODE_WHYNTER


const bool pwr_recover = false;// set true if amp remembers last state after power loss
const int treshold = 23;
const int time = 5;
const int btn = 4;
const int repeats = 4;
const int intputL = A1;


//int PanasonicAddress = 0x1CA;
//int PanasonicPower = 0x3D;

bool state = 0;
long unsigned int oldmilis;


// [1,2,3=address][4,5,6=command][7=protocol]

void saveCodes(int address, int command, int protocol) {

  //   for (int ii = 0 ; ii < EEPROM.length()/2 ; ii++) {
  //   EEPROM.write(0, 0);
  // }

  // store address
  EEPROM.put(1, address);

  // store command
  EEPROM.put(4, command);

  EEPROM.put(7, protocol);
}



void irRecord() {
  bool commandReceived = false;

  while (!commandReceived) {
    if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown protocol. \n Exiting."));
      } else {
        // Valid command received
        Serial.println(IrReceiver.decodedIRData.address);
        Serial.println(IrReceiver.decodedIRData.command);
        Serial.println(IrReceiver.decodedIRData.protocol);
        saveCodes(IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command, IrReceiver.decodedIRData.protocol);
        Serial.println("OK");

        commandReceived = true; // Set the flag to exit the loop
      }
    }
  }
}



void send(){


  int sendAddress = EEPROM.get(1, sendAddress);
  int sendCommand = EEPROM.get(4, sendCommand);
  int sendProtocol = EEPROM.get(7, sendProtocol);

  if (getProtocolString(sendProtocol) == "Panasonic"){
    IrSender.sendPanasonic(sendAddress, sendCommand, repeats);

  } else if (getProtocolString(sendProtocol) == "LG"){
    IrSender.sendLG(sendAddress, sendCommand, repeats);

  } else if (getProtocolString(sendProtocol) == "NEC"){
    IrSender.sendNEC(sendAddress, sendCommand, repeats);

  } else if (getProtocolString(sendProtocol) == "Samsung"){
    IrSender.sendSamsung(sendAddress, sendCommand, repeats);

  } else if (getProtocolString(sendProtocol) == "Sony"){
    IrSender.sendSony(sendAddress, sendCommand, repeats);
    
  } else if (getProtocolString(sendProtocol) == "RC5"){
    IrSender.sendRC5(sendAddress, sendCommand, repeats);

  } else if (getProtocolString(sendProtocol) == "RC6"){
    IrSender.sendRC6(sendAddress, sendCommand, repeats);

  } else{
    Serial.print("protocool not suported yet");
  }


  Serial.print("Address:");
  Serial.println(sendAddress);
  Serial.print("Command:");
  Serial.println(sendCommand);
  Serial.print("protocol:");
  Serial.println(getProtocolString(sendProtocol));
}




void turnOn(){
  if(state == 0){
    send();
    state = 1;
    if(pwr_recover){
      EEPROM.put(0, state);
    }
    Serial.println("Sending on");
  }
  else{
    
  }
}

void turnOff(){
  if(state == 1){
    send();
    state = 0;
    if(pwr_recover){
      EEPROM.put(0, state);
    }
    Serial.println("more than 5 seconds have pased\nSending off");
  }
  else{

  }
}





void setup() {
  pinMode(btn, INPUT_PULLUP);
  IrSender.begin();
  Serial.begin(115200);
  pinMode(intputL, INPUT);

  //enter recorting mode if button is pressed
  if(digitalRead(btn) == LOW){
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    Serial.println("entering recording mode");
    irRecord();
  }

  //if amp remembers last state get is from eprom
  if(pwr_recover){
    state = EEPROM.put(0, state);
  }
}

void loop() {

  //while the input is higher than treshold refresh the timer 
  if(analogRead(intputL) >= treshold){
    oldmilis = millis();
    Serial.print("Curent value:");
    Serial.println(analogRead(intputL));
    delay(300);
    turnOn();
  }


  //checking if more than time has pased since last timer refresh
  if (time*1000 + oldmilis <= millis()){
    turnOff();
  }


}


//old save rutine just in case
// void saveCodes(int address, int command, const String& protocol) {

//   //   for (int ii = 0 ; ii < EEPROM.length()/2 ; ii++) {
//   //   EEPROM.write(0, 0);
//   // }


//   // store address
//   EEPROM.write(1, address >> 8);
//   EEPROM.write(2, address & 0xFF);

//   // store command
//   EEPROM.write(3, command >> 8);
//   EEPROM.write(4, command & 0xFF);

//   byte protocolLength = protocol.length();
//   EEPROM.put(2 * sizeof(int), protocolLength);

//   for (int i = 0; i < protocolLength; i++) {
//     EEPROM.put(2 * sizeof(int) + 1 + i, protocol[i]);
//   }
// }

