/*
  Input Pull-up Serial

  This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital
  input on pin 2 and prints the results to the Serial Monitor.

  The circuit:
  - momentary switch attached from pin 2 to ground
  - built-in LED on pin 13

  Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
  20K-ohm resistor is pulled to 5V. This configuration causes the input to read
  HIGH when the switch is open, and LOW when it is closed.

  created 14 Mar 2012
  by Scott Fitzgerald

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/InputPullupSerial
*/

bool state = false;
int previousButton = 0;
int button = 0;

#define GREEN 11
#define RED 12
#define BUTTON 10

void setup() {
  //start serial connection
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(GREEN, OUTPUT);
  pinMode(GREEN, HIGH);
  pinMode(RED, OUTPUT);
  pinMode(RED, HIGH);
  previousButton = digitalRead(BUTTON);
  button = digitalRead(BUTTON);
}

void loop() {
  button = digitalRead(BUTTON);


  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  
  if (previousButton != button) {
    if (!button) {
      Serial.println("Button = ");
      Serial.println(button);
      Serial.println("State = ");
      Serial.println(state);
      Serial.println("");
      state = !state;
    }
    previousButton = button;
    delay(100);
  }
  
  if (button == LOW) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
  } 
  else if (state){
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
  }
  else {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, HIGH);
  }
}
