#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=184,238
AudioAnalyzePeak         peak1;          //xy=366,177
AudioOutputI2S           i2s2;           //xy=638,237
AudioConnection          patchCord1(adc1, peak1);
AudioConnection          patchCord2(adc1, 0, i2s2, 0);
AudioConnection          patchCord3(adc1, 0, i2s2, 1);
// GUItool: end automatically generated code


const int redPin = 4;
const int yelPin = 3;
const int grePin = 2;
const int buttonPin = 6;

AudioControlSGTL5000 audioShield;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(yelPin, OUTPUT);
  pinMode(grePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  

  // digitalWrite(redPin, HIGH);   // set the LED on
  // digitalWrite(yelPin, HIGH);   // set the LED on
  // digitalWrite(grePin, HIGH);   // set the LED on


  while (!Serial) ;
  delay(3000);
  AudioMemory(2);
  audioShield.enable();
  audioShield.volume(0.8);
  
}

int newval;
elapsedMillis fps;

void loop() {
  // put your main code here, to run repeatedly:
  if (fps > 54) {
    if (peak1.available()) {
      fps = 0;
      int monoPeak = peak1.read() * 30.0;
      // Serial.print(monoPeak);
      // Serial.println();
      if (monoPeak > 1) {
        digitalWrite(grePin, HIGH);
        if (monoPeak > 7) {
          digitalWrite(yelPin, HIGH);
          if (monoPeak > 15) {
            digitalWrite(redPin, HIGH); 
          }
          else {
            digitalWrite(redPin, LOW); 
          }
        }
        else {
          digitalWrite(redPin, LOW); 
          digitalWrite(yelPin, LOW);
        }
      }
      else {
        digitalWrite(redPin, LOW); 
        digitalWrite(yelPin, LOW);
        digitalWrite(grePin, LOW);
      }

      if (digitalRead(buttonPin) == HIGH) {
        Serial.println("Button is not pressed...");
      } else {
        Serial.println("Button pressed!!!");
      }
      
      // Serial.print("|");
      // Serial.print(monoPeak);
      // for (int cnt=0; cnt<monoPeak; cnt++) {
      //   Serial.print(">");
      // }

      
      
      // Serial.println();
    }
  }

}
