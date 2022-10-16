void setup() {
  Serial.begin(9600);
}

void loop() {
  float sensorValue = analogRead(A0)*(5.0/1023.0);
  Serial.println(sensorValue);
  delay(100);
}
