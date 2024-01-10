#include <Arduino.h>
#include <EEPROM.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
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


const int treshold = 23;
const int time = 5;
const int btn = 4;

const int repeats = 4;
//int PanasonicAddress = 0x1CA;
//int PanasonicPower = 0x3D;
int state = 0;
long unsigned int oldmilis;



void saveCodes(int address, int command, const String& protocol) {

    for (int ii = 0 ; ii < EEPROM.length()/2 ; ii++) {
    EEPROM.write(0, 0);
  }

  EEPROM.put(0, address);
  EEPROM.put(sizeof(int), command);

  byte protocolLength = protocol.length();
  EEPROM.put(2 * sizeof(int), protocolLength);
  
  for (int i = 0; i < protocolLength; i++) {
    EEPROM.put(2 * sizeof(int) + 1 + i, protocol[i]);
  }
}


int getAddress() {
  int adr;
  EEPROM.get(0, adr);
  return adr;
}

int getCommand() {
  int cmd;
  EEPROM.get(sizeof(int), cmd);
  return cmd;
}

String getProtocol() {
  byte protocolLength;
  EEPROM.get(2 * sizeof(int), protocolLength);
  
  char protocolChars[protocolLength + 1];
  for (int i = 0; i < protocolLength; i++) {
    EEPROM.get(2 * sizeof(int) + 1 + i, protocolChars[i]);
  }
  protocolChars[protocolLength] = '\0';
  
  return String(protocolChars);
}


void irRecord() {
  bool commandReceived = false;

  while (!commandReceived) {
    if (IrReceiver.decode()) {
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        Serial.println(F("Received noise or an unknown protocol. \n Exiting."));
      } else {
        // Valid command received
        saveCodes(IrReceiver.decodedIRData.address, IrReceiver.decodedIRData.command, getProtocolString(IrReceiver.decodedIRData.protocol));
        Serial.println("OK");

        commandReceived = true; // Set the flag to exit the loop
      }
    }
  }
}



void send(){



    if (getProtocol() == "Panasonic"){
      IrSender.sendPanasonic(getAddress(), getCommand(), repeats);

    } else if (getProtocol() == "LG"){
      IrSender.sendLG(getAddress(), getCommand(), repeats);

    } else if (getProtocol() == "NEC"){
      IrSender.sendNEC(getAddress(), getCommand(), repeats);

    } else if (getProtocol() == "Samsung"){
      IrSender.sendSamsung(getAddress(), getCommand(), repeats);

    } else if (getProtocol() == "Sony"){
      IrSender.sendSony(getAddress(), getCommand(), repeats);
      
    } else if (getProtocol() == "RC5"){
      IrSender.sendRC5(getAddress(), getCommand(), repeats);

    } else if (getProtocol() == "RC6"){
      IrSender.sendRC6(getAddress(), getCommand(), repeats);

    } else{
      Serial.print("protocool not suported yet");
    }


  Serial.println(getAddress());
  Serial.println(getCommand());
  Serial.println(getProtocol());
}




void turnOn(){
  if(state == 0){
    send();
    state = 1;
    Serial.println("Sending on");
  }
  else{
    
  }
}

void turnOff(){
  if(state == 1){
    send();
    state = 0;
    Serial.println("more than 5 seconds have pased");
    Serial.println("Sending off");
  }
  else{

  }
}





void setup() {
  pinMode(btn, INPUT_PULLUP);
  IrSender.begin();
  Serial.begin(115200);
  pinMode(A6, INPUT);


  if(digitalRead(btn) == LOW){
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    Serial.println("entering recording mode");
    irRecord();
  }
}

void loop() {

  if(analogRead(A6) >= treshold){
    oldmilis = millis();
    Serial.print("Curent value:");
    Serial.println(analogRead(A6));
    delay(300);
    turnOn();
  }


  //checking if more than time has pased
  if (time*1000 + oldmilis <= millis()){
    turnOff();
  }




}



// v1.0


/*

#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
IRsend irsend;

const int treshold = 23;
const int time = 5;
const int PanasonicAddress = 0x1CA;
const int PanasonicPower = 0x3D;

const int reprats = 4;



int state = 0;
long unsigned int oldmilis;

void turnOn(){
  if(state == 0){
    IrSender.sendPanasonic(PanasonicAddress, PanasonicPower, reprats);
    state = 1;
    Serial.println("Sending on");
  }
  else{
    
  }
}

void turnOff(){
  if(state == 1){
    IrSender.sendPanasonic(PanasonicAddress, PanasonicPower, reprats);
    state = 0;
    Serial.println("more than 5 seconds have pased");
    Serial.println("Sending off");
  }
  else{

  }
}


void setup() {
  IrSender.begin();
  Serial.begin(115200);
  pinMode(A6, INPUT);
}

void loop() {

  if(analogRead(A6) >= treshold){
    oldmilis = millis();
    Serial.print("Curent value:");
    Serial.println(analogRead(A6));
    delay(300);
    turnOn();
  }


//checking if more than time has pased
  if (time*1000 + oldmilis <= millis()){
    turnOff();
  }


}

*/