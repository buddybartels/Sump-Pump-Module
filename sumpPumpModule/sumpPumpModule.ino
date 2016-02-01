#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>
#include <avr/wdt.h>
#include <Time.h>
#include <TimeAlarms.h>

byte addresses[][6] = {"1Node", "2Node"};
int msg[1];
RF24 radio(9, 10);
//const uint64_t pipe = 0xE8E8F0F0E1LL;

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
//1-->sump module; 0-->main station
bool radioNumber = 1;

// Used to control whether this node is sending or receiving
//1-->sump module; 0-->main station
bool role = 1;

/**
* Create a data structure for transmitting and receiving data
* This allows many variables to be easily sent and received in a single transmission
* See http://www.cplusplus.com/doc/tutorial/structures/
*/
/*struct dataStruct {
  unsigned long _micros;
  float value;
} myData;*/

//Float Switches
const int fs1 = A1; //Float Switch #1
const int fs2 = A2; //Float Switch #2
const int fs3 = A3; //Float Switch #3
const int fs4 = A4; //Float Switch #4

boolean fs1State = 1;
boolean fs2State = 1;
boolean fs3State = 1;
boolean fs4State = 1;

int widelay = 0;


void setup() {
  setTime(0, 0, 0, 1, 1, 2015);
  Alarm.timerRepeat(28800, softwareReboot);
  Serial.begin(9600);
  radio.begin();
  //radio.setRetries(15, 15);
  //radio.openWritingPipe(pipe);
  //radio.stopListening();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if (radioNumber) {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  } else {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  }

  //myData.value = 0;
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  radio.stopListening();
  
  fs1State = digitalRead(fs1);
  fs2State = digitalRead(fs2);
  fs3State = digitalRead(fs3);
  fs4State = digitalRead(fs4);

  //FS1 Switch
  if (fs1State == LOW) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 11;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs1State == HIGH) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 19;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  //FS2 Switch
  if (fs2State == LOW) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 22;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs2State == HIGH) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 29;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  //FS3 Switch
  if (fs3State == LOW) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 33;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs3State == HIGH) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 39;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  //FS4 Switch
  if (fs4State == LOW) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 44;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs4State == HIGH) {
    delay(widelay);
    Alarm.delay(0);
    msg[0] = 49;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  delay(1000);


}

void softwareReboot() {
  wdt_enable(WDTO_15MS);
  while (1) {
  }
}

