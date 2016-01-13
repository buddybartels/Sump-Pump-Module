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

//Cell Phone Numbers
String cellular1raw = "2178989176";  //We want user to input this without +1
String prefix = "+1"; //US cell prefix needed for AT text send command
String number1 = prefix + cellular1raw; //combine prefix and number
//Define cell number as char array since this is needed for AT function
//The cell string is converted to this char array in the setup
char cellular1[15];

//Float Switches
/*MOVE THESE TO THE SUMP PUMP MODULE
const int fs1 = A1; //Float Switch #1
const int fs2 = A2; //Float Switch #2
const int fs3 = A3; //Float Switch #3
const int fs4 = A4; //Float Switch #4
*/

//Speaker
const int speakerPin = A5;

//LED Indicators
const int led_fs1 = A0;
const int led_fs2 = A1;
const int led_fs3 = A2;
const int led_fs4 = A3;
//const int led_signal=A5;

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
  //pinMode(fs1, INPUT);
  pinMode(led_fs1, OUTPUT);
  digitalWrite(led_fs1, LOW);

  //pinMode(fs2, INPUT);
  pinMode(led_fs2, OUTPUT);
  digitalWrite(led_fs2, LOW);

  //pinMode(fs3, INPUT);
  pinMode(led_fs3, OUTPUT);
  digitalWrite(led_fs3, LOW);

  //pinMode(fs4, INPUT);
  pinMode(led_fs4, OUTPUT);
  digitalWrite(led_fs4, LOW);

  //pinMode(led_signal, OUTPUT);
  //digitalWrite(led_signal, LOW);

  //Setup 2.4GHz Transmitter
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  delay(1000);
  SIM900.begin(9600);
  lcd.begin(16, 2); //set up the LCD's numbers of columns and rows
  lcd.print("Initializing...");
  //Serial.println("Initializing....");
  lcd.setCursor(0, 1);
  lcd.print("Sump Guard v0.2");
  delay(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initialization");
  lcd.setCursor(0, 1);
  lcd.print("Complete");
  //Serial.println("System Initialized");

  /*
  fs1State = digitalRead(fs1);
  fs2State = digitalRead(fs2);
  fs3State = digitalRead(fs3);
  fs4State = digitalRead(fs4);
  */

  /*Serial.print("FS1 State: ");
  Serial.print(fs1State);
  Serial.print("  FS2 State: ");
  Serial.print(fs2State);
  Serial.print("  FS3 State: ");
  Serial.print(fs3State);
  Serial.print("  FS4 State: ");
  Serial.println(fs4State);
  Serial.println("--------------------------------------------------------------------");*/

  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Checking Signal");

  //convert the cell strings to char which function needs
  number1.toCharArray(cellular1, 15);
  //Serial.println(cellular1); //for debugging

  getSignalQuality();
  //digitalWrite(led_signal, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("Signal Acquired");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sending SMS Text");
  lcd.setCursor(0, 1);
  lcd.print(number1);
  //sendInitializationSMS();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Message Sent!");
  lcd.setCursor(0, 1);
  lcd.print("Setup Complete");


  //Serial.println("Setup Completed; SMS Initiation Text Sent");

}

void loop() {

  if (radio.available()) {
    radio.read(msg, 1);
    //Serial.println(msg[0]);

    //check for FS1
    if (msg[0] == 111) {
      delay(10);
      fs1State = 1;
    }

    //check for FS2
    if (msg[0] == 222) {
      delay(10);
      fs2State = 1;
    }

    //check for FS3
    if (msg[0] == 333) {
      delay(10);
      fs3State = 1;
    }

    //check for FS4
    if (msg[0] == 444) {
      delay(10);
      fs4State = 1;
    }

  }

  else {
    delay(10);
    fs1State = 0;
    fs2State = 0;
    fs3State = 0;
    fs4State = 0;
  }

  if (fs1State != last_fs1State) {
    delay(1000);
    if (fs1State != last_fs1State) {
      if (fs1State == HIGH) {
        digitalWrite(led_fs1, HIGH);
        /*Serial.println("**** FLOAT SWITCH 1 TRIGGERED ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Normal Activity");
        lcd.setCursor(0, 1);
        lcd.print("in Progress...");
      }
      if (fs1State == LOW) {
        digitalWrite(led_fs1, LOW);
        /*Serial.println("**** FLOAT SWITCH 1 ALL CLEAR ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("* ALL SENSORS *");
        lcd.setCursor(0, 1);
        lcd.print("***  CLEAR  ***");
      }
    }
  }

  if (fs2State != last_fs2State) {
    delay(1000);
    if (fs2State != last_fs2State) {
      if (fs2State == HIGH) {
        digitalWrite(led_fs2, HIGH);
        /*Serial.println("**** FLOAT SWITCH 2 TRIGGERED ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WARNING...");
        lcd.setCursor(0, 1);
        lcd.print("SENSOR LEVEL 2");
        tone(speakerPin, 1000);
        //sendFS2SMS();
      }
      if (fs2State == LOW) {
        digitalWrite(led_fs2, LOW);
        /*Serial.println("**** FLOAT SWITCH 2 ALL CLEAR ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SENSOR LEVEL 2");
        lcd.setCursor(0, 1);
        lcd.print("ALL CLEAR");
        noTone(speakerPin);
        //sendFS2SMS_clear();
      }
    }
  }

  if (fs3State != last_fs3State) {
    delay(1000);
    if (fs3State != last_fs3State) {
      if (fs3State == HIGH) {
        digitalWrite(led_fs3, HIGH);
        /*Serial.println("**** FLOAT SWITCH 3 TRIGGERED ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WARNING...");
        lcd.setCursor(0, 1);
        lcd.print("SENSOR LEVEL 3");
        tone(speakerPin, 1000);
        //sendFS3SMS();
      }
      if (fs3State == LOW) {
        digitalWrite(led_fs3, LOW);
        /*Serial.println("**** FLOAT SWITCH 3 ALL CLEAR ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SENSOR LEVEL 3");
        lcd.setCursor(0, 1);
        lcd.print("ALL CLEAR");
        //noTone(speakerPin);
        //sendFS3SMS_clear();
      }
    }
  }

  if (fs4State != last_fs4State) {
    delay(1000);
    if (fs4State != last_fs4State) {
      if (fs4State == HIGH) {
        digitalWrite(led_fs4, HIGH);
        /*Serial.println("**** FLOAT SWITCH 4 TRIGGERED ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("!!!WARNING!!!");
        lcd.setCursor(0, 1);
        lcd.print("SENSOR LEVEL 4");
        tone(speakerPin, 1000);
        //sendFS4SMS();
      }
      if (fs4State == LOW) {
        digitalWrite(led_fs4, LOW);
        /*Serial.println("**** FLOAT SWITCH 4 ALL CLEAR ****");
        Serial.println("--------------------------------------------------------------------");*/
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("SENSOR LEVEL 4");
        lcd.setCursor(0, 1);
        lcd.print("ALL CLEAR");
        //noTone(speakerPin);
        //sendFS4SMS_clear();
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
    SIM900.println("AT+CSQ");  //get the signal Quality
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
      Serial.print("The Signal Quality is poor!");
    }
    if (SIGQ < 10) {
      //Serial.print("The Signal Quality is poor!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NO CELL SIGNAL!");
    }
  }
}
