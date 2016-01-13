//Required Libraries
#include <SerialGSM.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//2.4GHz Transmitter Settings (nRF24L01 +2.4GHz)
int msg[1];
RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int widelay = 10;

//Cell Phone Numbers
String cellular1raw = "2178989176";  //We want user to input this without +1
String prefix = "+1"; //US cell prefix needed for AT text send command
String number1 = prefix + cellular1raw; //combine prefix and number
//Define cell number as char array since this is needed for AT function
//The cell string is converted to this char array in the setup
char cellular1[15];

//Speaker
const int speakerPin = A5;

//LED Indicators
const int led_fs1 = A0;
const int led_fs2 = A1;
const int led_fs3 = A2;
const int led_fs4 = A3;

//Variables
boolean fs1State = 0;
boolean last_fs1State = 0;
boolean fs2State = 0;
boolean last_fs2State = 0;
boolean fs3State = 0;
boolean last_fs3State = 0;
boolean fs4State = 0;
boolean last_fs4State = 0;

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//Setup GSM Modules pins and variables
SerialGSM cell(8, A4);
SoftwareSerial SIM900(8, A4);
unsigned char SigQ[50];
unsigned char SigQ1[5] = {'a', 'a', 'a', 'a', 'a'};
int SIGQ = 0;
int signalPercent = 0;

void setup() {
  pinMode(led_fs1, OUTPUT);
  digitalWrite(led_fs1, LOW);

  pinMode(led_fs2, OUTPUT);
  digitalWrite(led_fs2, LOW);

  pinMode(led_fs3, OUTPUT);
  digitalWrite(led_fs3, LOW);

  pinMode(led_fs4, OUTPUT);
  digitalWrite(led_fs4, LOW);

  //Setup 2.4GHz Transmitter
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
  delay(1000);
  SIM900.begin(9600);
  lcd.begin(16, 2); //set up the LCD's numbers of columns and rows
  lcd.print(F("Initializing..."));
  //Serial.println("Initializing....");
  lcd.setCursor(0, 1);
  lcd.print(F("Sump Guard v0.3"));
  delay(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Initialization"));
  lcd.setCursor(0, 1);
  lcd.print(F("Complete"));

  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Checking Signal"));

  //convert the cell strings to char which function needs
  number1.toCharArray(cellular1, 15);
  //Serial.println(cellular1); //for debugging

  getSignalQuality();
  lcd.setCursor(0, 1);
  lcd.print(F("Signal Acquired"));
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Sending SMS Text"));
  lcd.setCursor(0, 1);
  lcd.print(number1);

  //------------TURN ON TO ACTIVATE GSM-----------------
  //sendInitializationSMS();
  //------------TURN ON TO ACTIVATE GSM-----------------

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Message Sent!"));
  lcd.setCursor(0, 1);
  lcd.print(F("Setup Complete"));

}

void loop() {

  if (radio.available()) {
    radio.read(msg, 1);
    Serial.println(msg[0]);

    //check for FS1
    if (msg[0] == 11) {
      delay(widelay);
      fs1State = 1;
    }

    if (msg[0] == 19) {
      delay(widelay);
      fs1State = 0;
    }

    //check for FS2
    if (msg[0] == 22) {
      delay(widelay);
      fs2State = 1;
    }

    if (msg[0] == 29) {
      delay(widelay);
      fs2State = 0;
    }

    //check for FS3
    if (msg[0] == 33) {
      delay(widelay);
      fs3State = 1;
    }

    if (msg[0] == 39) {
      delay(widelay);
      fs3State = 0;
    }

    //check for FS4
    if (msg[0] == 44) {
      delay(widelay);
      fs4State = 1;
    }

    if (msg[0] == 49) {
      delay(widelay);
      fs4State = 0;
    }

  }

  else {
    delay(widelay);
    //Serial.println(F("No 2.4Ghz Signal"));
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("No Sump Pump"));
    lcd.setCursor(0, 1);
    lcd.print(F("Activity"));
  }

  if (fs1State != last_fs1State) {
    delay(1000);
    if (fs1State != last_fs1State) {
      if (fs1State == HIGH) {
        digitalWrite(led_fs1, HIGH);
        //Serial.println("FLOAT SWITCH 1 TRIGGERED");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("Normal Activity"));
        lcd.setCursor(0, 1);
        lcd.print(F("in Progress..."));
        noTone(speakerPin);
      }
      if (fs1State == LOW) {
        digitalWrite(led_fs1, LOW);
        //Serial.println("FLOAT SWITCH 1 ALL CLEAR");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("* ALL SENSORS *"));
        lcd.setCursor(0, 1);
        lcd.print(F("***  CLEAR  ***"));
        noTone(speakerPin);
      }
    }
  }

  if (fs2State != last_fs2State) {
    delay(1000);
    if (fs2State != last_fs2State) {
      if (fs2State == HIGH) {
        digitalWrite(led_fs2, HIGH);
        //Serial.println("FLOAT SWITCH 2 TRIGGERED");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("WARNING..."));
        lcd.setCursor(0, 1);
        lcd.print(F("SENSOR LEVEL 2"));
        tone(speakerPin, 1000);
        //------------TURN ON TO ACTIVATE GSM-----------------
        //sendFS2SMS();
        //------------TURN ON TO ACTIVATE GSM-----------------
      }
      if (fs2State == LOW) {
        digitalWrite(led_fs2, LOW);
        //Serial.println("FLOAT SWITCH 2 ALL CLEAR");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("SENSOR LEVEL 2"));
        lcd.setCursor(0, 1);
        lcd.print(F("ALL CLEAR"));
        noTone(speakerPin);
        //------------TURN ON TO ACTIVATE GSM-----------------
        //sendFS2SMS_clear();
        //------------TURN ON TO ACTIVATE GSM-----------------
      }
    }
  }

  if (fs3State != last_fs3State) {
    delay(1000);
    if (fs3State != last_fs3State) {
      if (fs3State == HIGH) {
        digitalWrite(led_fs3, HIGH);
        //Serial.println("FLOAT SWITCH 3 TRIGGERED");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("WARNING..."));
        lcd.setCursor(0, 1);
        lcd.print(F("SENSOR LEVEL 3"));
        tone(speakerPin, 1000);
        //------------TURN ON TO ACTIVATE GSM-----------------
        //sendFS3SMS();
        //------------TURN ON TO ACTIVATE GSM-----------------
      }
      if (fs3State == LOW) {
        digitalWrite(led_fs3, LOW);
        //Serial.println("FLOAT SWITCH 3 ALL CLEAR");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("SENSOR LEVEL 3"));
        lcd.setCursor(0, 1);
        lcd.print(F("ALL CLEAR"));
        //noTone(speakerPin);
        //------------TURN ON TO ACTIVATE GSM-----------------
        //sendFS3SMS_clear();
        //------------TURN ON TO ACTIVATE GSM-----------------
      }
    }
  }

  if (fs4State != last_fs4State) {
    delay(1000);
    if (fs4State != last_fs4State) {
      if (fs4State == HIGH) {
        digitalWrite(led_fs4, HIGH);
        //Serial.println("FLOAT SWITCH 4 TRIGGERED");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("!!!WARNING!!!"));
        lcd.setCursor(0, 1);
        lcd.print(F("SENSOR LEVEL 4"));
        tone(speakerPin, 1000);
        //------------TURN ON TO ACTIVATE GSM-----------------
        //sendFS4SMS();
        //------------TURN ON TO ACTIVATE GSM-----------------
      }
      if (fs4State == LOW) {
        digitalWrite(led_fs4, LOW);
        //Serial.println("FLOAT SWITCH 4 ALL CLEAR");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(F("SENSOR LEVEL 4"));
        lcd.setCursor(0, 1);
        lcd.print(F("ALL CLEAR"));
        //------------TURN ON TO ACTIVATE GSM-----------------
        //sendFS4SMS_clear();
        //------------TURN ON TO ACTIVATE GSM-----------------
      }
    }
  }


  last_fs1State = fs1State;
  last_fs2State = fs2State;
  last_fs3State = fs3State;
  last_fs4State = fs4State;

}


void sendInitializationSMS() {
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();
  cell.Rcpt(cellular1);
  cell.Message("Sump Pump Monitoring System: Initialization After Complete Shutdown");
  cell.SendSMS();
}

void sendFS2SMS() {
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();
  cell.Rcpt(cellular1);
  cell.Message("BE ON WATCH: SUMP PUMP LEVEL 2 REACHED");
  cell.SendSMS();
}

void sendFS2SMS_clear() {
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();
  cell.Rcpt(cellular1);
  cell.Message("SENSOR LEVEL 2 - ALL CLEAR");
  cell.SendSMS();
}

void sendFS3SMS() {
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();
  cell.Rcpt(cellular1);
  cell.Message("WARNING!: SUMP PUMP LEVEL 3 REACHED");
  cell.SendSMS();
}

void sendFS3SMS_clear() {
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();
  cell.Rcpt(cellular1);
  cell.Message("SENSOR LEVEL 3 - ALL CLEAR");
  cell.SendSMS();
}

void sendFS4SMS() {
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();
  cell.Rcpt(cellular1);
  cell.Message("WARNING!: SUMP PUMP LEVEL 4 REACHED. ACT IMMEDIATELY!");
  cell.SendSMS();
}

void sendFS4SMS_clear() {
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  cell.FwdSMS2Serial();
  cell.Rcpt(cellular1);
  cell.Message("SENSOR LEVEL 4 - ALL CLEAR");
  cell.SendSMS();
}

void getSignalQuality() {
  for (int x = 0; x < 20; x++) {
    SIM900.println(F("AT+CSQ"));  //get the signal Quality
    delay(100);
    int k = 0;
    while (SIM900.available() != 0) {
      SigQ[k] = SIM900.read();
      Serial.write(SigQ[k]);
      k += 1;
    }
    delay(800);
    for (int i = 0; i < 26; i++) {
      if (SigQ[i] == 58) {
        int j = 0;
        int k = 0;
        i++; i++;
        while ((SigQ[i + j]) != 44) {
          if ((SigQ[i + j] >= '0' && SigQ[i + j] <= '9')) {
            SigQ1[j] = SigQ[i + j];
          }
          j++;  //j=1
        }
        SIGQ = SigQ1[0] - '0';
        if (j == 2) {
          SIGQ = SIGQ * 10 + SigQ1[1] - '0';
        }
        //Serial.println("");
        //Serial.print("SIGQ:");
        //Serial.println(SIGQ);
        signalPercent = SIGQ / .31;
        //Serial.print("Signal: ");
        //Serial.print(signalPercent);
        //Serial.println("%");
      }
    }
    if (SIGQ >= 10) {
      break;
    }
    if (x == 20) {
      //Serial.print("The Signal Quality is poor!");
    }
    if (SIGQ < 10) {
      //Serial.print("The Signal Quality is poor!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("NO CELL SIGNAL!"));
    }
  }
}
