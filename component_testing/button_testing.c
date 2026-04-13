int pushButton = A0;


void setup() {

  Serial.begin(9600);

  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
}


void loop() {
 
  int buttonState = digitalRead(pushButton);
  Serial.println(buttonState);
  
  delay(100);        
}
