const int analogPin = 17;
const int potPin1 = 18;
const int potPin2 = 19;
const int thresholdMin = 60;
const int peakTrackMillis = 12;
const int aftershockMillis = 25;

// piezo drum code modified

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2500) /* wait for serial monitor */ ;
  Serial.println("Piezo Peak Capture");
}


void loop() {
  int piezo = analogRead(analogPin);
  peakDetect(piezo);

  int pot1 = analogRead(potPin1);
  int pot2 = analogRead(potPin2);
  
  usbMIDI.sendControlChange(1, pot1 / 8, 10);
  usbMIDI.sendControlChange(2, pot2 / 8, 10);


  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}


void peakDetect(int voltage) {
  static int state;
  static int peak; 
  static elapsedMillis msec;

  switch (state) {
    case 0:
      if (voltage > thresholdMin) {
        peak = voltage;
        msec = 0;
        state = 1;
      }
      return;

    case 1:
      if (voltage > peak) {
        peak = voltage;     
      }
      if (msec >= peakTrackMillis) {
        int velocity = map(peak, thresholdMin, 1023, 1, 127);
        usbMIDI.sendNoteOn(38, velocity, 10);
        msec = 0;
        state = 2;
      }
      return;

    default:
      if (voltage > thresholdMin) {
        msec = 0; // keep resetting timer if above threshold
      } else if (msec > aftershockMillis) {
        usbMIDI.sendNoteOff(38, 0, 10);
        state = 0; // go back to idle when
      }
  }
}

