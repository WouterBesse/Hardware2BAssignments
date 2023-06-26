#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=184,238
AudioSynthWaveform       waveform1;      //xy=260,458
AudioAnalyzePeak         peak1;          //xy=366,177
AudioAnalyzeNoteFrequency notefreq1;      //xy=382,362
AudioAmplifier           amp1;           //xy=426,459
AudioOutputI2S           i2s2;           //xy=638,237
AudioConnection          patchCord1(adc1, peak1);
AudioConnection          patchCord2(adc1, notefreq1);
AudioConnection          patchCord4(waveform1, 0, i2s2, 0);
AudioConnection          patchCord5(waveform1, 0, i2s2, 1);
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
  AudioMemory(30);
  audioShield.enable();
  audioShield.volume(1.0);
  notefreq1.begin(0.65);
  amp1.gain(0.0);
  waveform1.begin(WAVEFORM_TRIANGLE);
  waveform1.amplitude(0.0);
  
}

int newval;
float freq;
elapsedMillis fps;
int note = 0;

int midivalue = 440;

bool notePlayed = true;
bool firstNote = true;
int transpositions[] = {0, 4, 7};

float noteToFreq(int note) {
    float a = 440; //frequency of A (coomon value is 440Hz)
    return (a / 32) * pow(2, ((note - 9) / 12.0));
}

void loop() {
 
  if (note == 0) {
    digitalWrite(redPin, LOW); 
    digitalWrite(yelPin, LOW);
    digitalWrite(grePin, HIGH);
  }
  else if (note == 1) {
    digitalWrite(redPin, LOW); 
    digitalWrite(yelPin, HIGH);
    digitalWrite(grePin, LOW);
  }
  else if (note == 2) {
    digitalWrite(redPin, HIGH); 
    digitalWrite(yelPin, LOW);
    digitalWrite(grePin, LOW);
  }

  
  // newval = analogRead(A8);
  if (notefreq1.available()) {
    // Serial.println(notefreq1.read());
    freq = notefreq1.read();
    midivalue = 12*log(freq/440) + 69;
    
    
  }

  if (digitalRead(buttonPin) == LOW) {
    if (notePlayed) {
      if (firstNote) {
        waveform1.frequency(noteToFreq(midivalue));
        waveform1.amplitude(0.8);
        firstNote = false;
      }
      else {
        if (note + 1 > 2) {
          note = 0;
        } else {
          note += 1;
        }
        Serial.println("play note");
        float freq2play = noteToFreq(midivalue + transpositions[note]);
        Serial.println(midivalue);
        Serial.println(freq2play);
        waveform1.frequency(freq2play);
        waveform1.amplitude(0.8);
        
      }
      notePlayed = false;
    }
  } else {
    if (!notePlayed) {
      notePlayed = true;
      waveform1.amplitude(0.0);
    }
  }
}
  


