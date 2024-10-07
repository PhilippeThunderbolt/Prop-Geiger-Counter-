/* Fake geiger counter V1.0
  By Possum Dreaming 2016 
  
  Modified by Philippe Thunderbolt to allow for the original geiger counter meter to also work.  Make sure you use
  the appropriate resister suited for your meter to avoid too much current into the vintage meter.

  Setup for Adafruit Itsy Bitsy 12Jan2023

  Currently configured for Civil Defense CDV-715 meter

*/

const uint8_t SLOW_TICK_DELAY = 80;
const uint8_t FAST_TICK_DELAY = 5;
const uint8_t LOOP_DELAY = 10;

const int minMeter = 100;
const int maxMeter = 130;   //change to suit your meterPin
const int meterFactor = 1;  //
int currentMeter = minMeter;

//pins - change to suit your arduino board. Setup for an Adafruit Itsby Bitsy
const uint8_t switchPin = 7;
const uint8_t ledPin = 11;
const uint8_t speakerPin = 6;
const uint8_t meterPin = A1;  //Pin A1 on Itsy Bitsy

int max_off_delay;

void setup() {

  //Serial.begin(115200);
  pinMode(switchPin, INPUT_PULLUP);  //sensor to detect that the geiger counter is trending up (mercury switchPin etc)
  pinMode(ledPin, OUTPUT);           //goes to LED
  pinMode(speakerPin, OUTPUT);       //goes to speakerPin
  pinMode(meterPin, OUTPUT);         //goes to the geiger counter analog meterPin so need to use a PWN pinout

  max_off_delay = SLOW_TICK_DELAY;

  analogWrite(meterPin, minMeter);
}

void loop() {
  int off_delay = random(max_off_delay) + 1;

  for (int i = 0; i < off_delay; i++) {
    // Poll the button while waiting
    // Button pulls down, so 0=pressed 1=released
    if (digitalRead(switchPin)) {
      // Released, so ramp speed down (i.e. increase the delay between ticks)

      if (max_off_delay < SLOW_TICK_DELAY) max_off_delay++;
      if (currentMeter > minMeter) {
        currentMeter = (currentMeter - meterFactor);
      }


    } else {
      // Activated, so ramp speed up (i.e. decrease the delay between ticks)
      if (max_off_delay > FAST_TICK_DELAY) {
        max_off_delay -= 1;

        //Serial.print("CurrentMeter is: ");
        //Serial.println(currentMeter);
        if (currentMeter < maxMeter) {
          currentMeter = (currentMeter + meterFactor);
          //Serial.println("In the addition loop)");
        }
      }
    }
    // To make the ramp up less jumpy, we exit the delay loop at once if the maximum delay has reduced to below the current delay.
    if (max_off_delay < i) break;
    delay(LOOP_DELAY);
  }

  digitalWrite(speakerPin, HIGH);  //Make the click
  digitalWrite(ledPin, HIGH);      // Flash the light
  analogWrite(meterPin, currentMeter);
  //Serial.println(currentMeter);
  delay(10);
  digitalWrite(speakerPin, LOW);  //Turn speakerPin off
  digitalWrite(ledPin, LOW);      // Turn LED ioff
}
