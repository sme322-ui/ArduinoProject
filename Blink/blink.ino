void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on 
  delay(1000);              // wait for 1 second
  digitalWrite(13, LOW);    // turn the LED off
  delay(1000);              // wait for 1 second
}
