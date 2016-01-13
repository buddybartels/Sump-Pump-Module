#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

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
  Serial.begin(9600);
  radio.begin();
  //radio.setRetries(15, 15);
  radio.openWritingPipe(pipe);
  radio.stopListening();
}

void loop() {
  fs1State = digitalRead(fs1);
  fs2State = digitalRead(fs2);
  fs3State = digitalRead(fs3);
  fs4State = digitalRead(fs4);

  //FS1 Switch
  if (fs1State == LOW) {
    delay(widelay);
    msg[0] = 11;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs1State == HIGH) {
    delay(widelay);
    msg[0] = 19;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  //FS2 Switch
  if (fs2State == LOW) {
    delay(widelay);
    msg[0] = 22;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs2State == HIGH) {
    delay(widelay);
    msg[0] = 29;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  //FS3 Switch
  if (fs3State == LOW) {
    delay(widelay);
    msg[0] = 33;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs3State == HIGH) {
    delay(widelay);
    msg[0] = 39;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  //FS4 Switch
  if (fs4State == LOW) {
    delay(widelay);
    msg[0] = 44;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }

  if (fs4State == HIGH) {
    delay(widelay);
    msg[0] = 49;
    radio.write(msg, 1);
    Serial.println(msg[0]);
  }


}

