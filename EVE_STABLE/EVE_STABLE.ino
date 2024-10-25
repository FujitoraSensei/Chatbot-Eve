#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <LiquidCrystal.h>

// The remaining pins are gnd and power ones only so you can check the pinout of the lcd and do the wiring 
const byte rs = 12;
const byte en = 7;
const byte d4 = 8;
const byte d5 = 9;
const byte d6 = 10;
const byte d7 = 13;
int RandomInt;

String readSerial;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

byte neutral_part[8] = {

  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111

};

byte blink_part[8] = {

  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111

};

byte Glee_left[8] = {

  0b00000,
  0b00000,
  0b00111,
  0b01111,
  0b11111,
  0b11111,
  0b11100,
  0b11000
};

byte Glee_right[8] = {

  0b00000,
  0b00000,
  0b11100,
  0b11110,
  0b11111,
  0b11111,
  0b00111,
  0b00001

};

byte Sad_left[8] = {

  0b00000,
  0b00000,
  0b10000,
  0b11000,
  0b11111,
  0b11111,
  0b01111,
  0b00111
};

byte Sad_right[8] = {

  0b00000,
  0b00000,
  0b00001,
  0b00011,
  0b11111,
  0b11111,
  0b11110,
  0b11100
};

byte Squint_left[8] = {

  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01111,
  0b11111,
  0b00000

};

byte Squint_right[8] = {

  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11110,
  0b11111,
  0b00000

};


void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16, 2);       

//create the characters
  lcd.createChar(0, neutral_part);
  //neutral is set as 0
  lcd.createChar(1, blink_part);
  //blink as 1
  lcd.createChar(2, Glee_left);
  //glee left as 2
  lcd.createChar(3, Glee_right);
  //glee right as 3
  lcd.createChar(4, Sad_left);
  //sad left as 4
  lcd.createChar(5, Sad_right);
  //sad right as 5
  lcd.createChar(6, Squint_left);
  //squint left as 6
  lcd.createChar(7, Squint_right);
  //squint right as 7

  Serial.println(F("Voice Controlled mp3 player Demo."));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms

  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.play(1);
  delay(500);
  Glee();

}

void loop() {
  if (Serial.available()) {
    readSerial = Serial.readStringUntil('#');
    Serial.println(readSerial);

    if ((readSerial.indexOf("hello") > -1) || (readSerial.indexOf("hey") > -1)) {
    Glee();
    RandomInt = random(1, 7); 
    myDFPlayer.play(RandomInt);
    Serial.println(RandomInt);
    }
// below code means you can trigger the audiofile using the two given keywords
   else if ((readSerial.indexOf("audiobook") > -1) || (readSerial.indexOf("goals") > -1)) {
    Neutral();
    myDFPlayer.play(53);
    Serial.println(53);
    } 

    // below code means that you can only trigger the audiofile when this two keyword matches
    else if ((readSerial.indexOf("feeling") > -1) && (readSerial.indexOf("lonely") > -1)) {
    Sad();
    myDFPlayer.play(89);
    Serial.println(89);
    }
// below code can be used when you have more than one response and then eve can give random response whenever the keyword matches
    else if (readSerial.indexOf("story") > -1) {
    Glee();
    RandomInt = random(36, 37);
    myDFPlayer.play(RandomInt);
    Serial.println(RandomInt);
    }

// below code is when you have only keyword and only one audiofile
else if (readSerial.indexOf("Siri") > -1) {
    Squint();
    myDFPlayer.play(34);
    Serial.println(34);
    }

    // you copy the above give else if codes to add more and more keywords 
    // Thank You :)






  }

}

void blink() {

  lcd.clear();  //clear the lcd screen
  delay(40);
  for (int i; i <= 15; i++) {
    lcd.setCursor(i, 0);
    lcd.write(byte(1));  //print the blink character unless the it covers the whole screen
  }
  delay(400);  //wait for 400 micro seconds so that the blink becomes prominent to the human eye
}

void Neutral() {

  blink();
  lcd.clear();  //clear the previous lcd values
  delay(200);
  lcd.setCursor(6, 0);
  lcd.write(byte(0));
  lcd.setCursor(6, 1);
  lcd.write(byte(0));
  lcd.setCursor(7, 0);
  lcd.write(byte(0));
  lcd.setCursor(7, 1);
  lcd.write(byte(0));

  lcd.setCursor(9, 0);
  lcd.write(byte(0));
  lcd.setCursor(9, 1);
  lcd.write(byte(0));
  lcd.setCursor(10, 0);
  lcd.write(byte(0));
  lcd.setCursor(10, 1);
  lcd.write(byte(0));
}

void Glee() {

  blink();      //blink before the robot is in the glee mood
  lcd.clear();  //clear the lcd screen
  delay(200);
  lcd.setCursor(6, 0);
  lcd.write(byte(2));
  lcd.setCursor(7, 0);
  lcd.write(byte(3));

  lcd.setCursor(9, 0);
  lcd.write(byte(2));
  lcd.setCursor(10, 0);
  lcd.write(byte(3));
}


void Sad() {

  blink();      //blink before the robot sets in the mood
  lcd.clear();  //clear the screen
  delay(200);
  lcd.setCursor(6, 0);
  lcd.write(byte(4));
  lcd.setCursor(7, 0);
  lcd.write(byte(5));

  lcd.setCursor(9, 0);
  lcd.write(byte(4));
  lcd.setCursor(10, 0);
  lcd.write(byte(5));
}

void Squint() {

  blink();      //blink before the robot sets in the mood
  lcd.clear();  //clear the screen for the current display
  delay(200);
  lcd.setCursor(6, 0);
  lcd.write(byte(6));
  lcd.setCursor(7, 0);
  lcd.write(byte(7));

  lcd.setCursor(9, 0);
  lcd.write(byte(6));
  lcd.setCursor(10, 0);
  lcd.write(byte(7));
}
//BYE BYE 

//Mihran Raza BARKATI
