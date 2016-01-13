#include  <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

//Float Switches
const int fs1 = A1; //Float Switch #1
const int fs2 = A2; //Float Switch #2
const int fs3 = A3; //Float Switch #3
const int fs4 = A4; //Float Switch #4

boolean fs1State = 1;
boolean last_fs1State = 1;
boolean fs2State = 1;
boolean last_fs2State = 1;
boolean fs3State = 1;
boolean last_fs3State = 1;
boolean fs4State = 1;
boolean last_fs4State = 1;

void setup(){
  Serial.begin(9600);
  radio.begin();
  radio.setRetries(15,15);
  radio.openWritingPipe(pipe);
  radio.stopListening();
  }

void loop(){
  fs1State = digitalRead(fs1);
  fs2State = digitalRead(fs2);
  fs3State = digitalRead(fs3);
  fs4State = digitalRead(fs4);
  
  //FS1 Switch
 if (fs1State != last_fs1State){
  delay(1000);
  if (fs1State != last_fs1State){
    if(fs1State == LOW){
      msg[0] = 111;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
    if(fs1State == HIGH){
      msg[0] = 110;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
  }
 }

  //FS2 Switch
 if (fs2State != last_fs2State){
  delay(1000);
  if (fs2State != last_fs2State){
    if(fs2State == LOW){
      msg[0] = 222;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
    if(fs2State == HIGH){
      msg[0] = 220;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
  }
 }

  //FS3 Switch
 if (fs3State != last_fs3State){
  delay(1000);
  if (fs3State != last_fs3State){
    if(fs3State == LOW){
      msg[0] = 333;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
    if(fs3State == HIGH){
      msg[0] = 330;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
  }
 }

  //FS4 Switch
 if (fs4State != last_fs4State){
  delay(1000);
  if (fs4State != last_fs4State){
    if(fs4State == LOW){
      msg[0] = 444;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
    if(fs4State == HIGH){
      msg[0] = 440;
      radio.write(msg, sizeof(msg));
      //Serial.println(msg[0]);
    }
  }
 }

 last_fs1State = fs1State;
 last_fs2State = fs2State;
 last_fs3State = fs3State;
 last_fs4State = fs4State;

}

