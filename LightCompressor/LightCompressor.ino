#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=201,221
AudioAnalyzePeak         peak1;          //xy=347,292
AudioAmplifier           amp1;           //xy=377,221
AudioAmplifier           amp2;           //xy=377,221
AudioOutputI2S           i2s2;           //xy=638,237
AudioConnection          patchCord1(adc1, amp1);
AudioConnection          patchCord2(amp1, peak1);
AudioConnection          patchCord3(amp1, amp2);
AudioConnection          patchCord4(amp2, 0, i2s2, 0);
AudioConnection          patchCord5(amp2, 0, i2s2, 1);
// GUItool: end automatically generated code


AudioControlSGTL5000 audioShield;

const int redPin = 3;
const int ldrPin = 18;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redPin, OUTPUT);

  while (!Serial) ;
  AudioMemory(30);
  audioShield.enable();
  audioShield.volume(1.0);
  amp1.gain(0.25);
  amp2.gain(0.2);
}
int val = 0;
double inAudio = 0.0;
double compAmt = 0.0;

double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(ldrPin) / 4;
  val = val * -1 + 255; // invert to make pow work better (I'm too lazy to figure out log)
  
  inAudio = peak1.read() * 4;
  analogWrite(redPin, inAudio * 255);
  // Serial.println(val);
  compAmt = ((val*val)/65536.0); // 256 * 256 = 65536, we devide to get a number in the balpark of range -1. 1.
  compAmt = compAmt * -1.0 + 1.0;
  Serial.println((compAmt * 2) - 1.0);
  amp2.gain(clamp((compAmt * 2) - 1.0, 0.0, 1.0)*0.2);
  // digitalWrite(redPin, HIGH); 
  delay(4);
}
