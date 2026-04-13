#include <Servo.h>

const int buttonPin = 7;
const int servoPin  = 9;

const int midPos = 90;
const int posA   = 43;
const int posB   = 138;

Servo myservo;
float pos = midPos;

float targetPos = midPos;
float nextTarget = posA;


bool buttonHeld = false;
bool returning  = false;
bool lastButtonState = HIGH;

//to deal with jitter, sometimes the button input jumps (101) this makes it switch too early and then go the same direction twice
const unsigned long debounceDelay = 50; 
unsigned long lastButtonChangeTime = 0;

const float stepSize = 1.0;           
const unsigned long moveInterval = 20; 
unsigned long lastMoveTime = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  myservo.attach(servoPin);
  myservo.write(pos);
  Serial.begin(9600);
  //Serial.println("setup complete");
}

void loop() {
  bool reading = (digitalRead(buttonPin) == LOW);
  unsigned long now = millis();

  //button jitter part
  if (reading != lastButtonState && now - lastButtonChangeTime > debounceDelay) {
    lastButtonChangeTime = now;

    if (reading && !buttonHeld && !returning) {.  // if button pressed
  
      buttonHeld = true;
      targetPos = nextTarget;
      //Serial.print("button pressed, moving to target: ");
      //Serial.println(targetPos);
    } else if (!reading && buttonHeld) { //if button released
      
      buttonHeld = false;
      targetPos = midPos;
      returning = true;
      nextTarget = (nextTarget == posA) ? posB : posA;
     // Serial.print("button released, returning to mid. next target: ");
     // Serial.println(nextTarget);
    }
  }

  lastButtonState = reading;

//detaching servo to avoid the servo jumping back and forth a couple degrees when it isn't moving
  if (millis() - lastMoveTime >= moveInterval) {
    if (pos < targetPos) {
      if (!myservo.attached()) myservo.attach(servoPin);
      pos += stepSize;
      if (pos > targetPos) pos = targetPos;
      myservo.write(pos);
    } 
    else if (pos > targetPos) {
      if (!myservo.attached()) myservo.attach(servoPin);
      pos -= stepSize;
      if (pos < targetPos) pos = targetPos;
      myservo.write(pos);
    } 
    else {
 
          //if we reach the target and the servo is attached, detach it
      if (myservo.attached()) myservo.detach();
      if (returning && pos == midPos) {
        returning = false;
     //   Serial.println("returned to middle → ready for next press");
      }
    }
    lastMoveTime = millis();
  }
/*
// for debugging button jitter
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 100) {
    Serial.print("Pos: "); Serial.print(pos);
    Serial.print(" | Target: "); Serial.print(targetPos);
    Serial.print(" | Returning: "); Serial.print(returning);
    Serial.print(" | ButtonHeld: "); Serial.println(buttonHeld);
    lastPrint = millis();
  }
  */
}
