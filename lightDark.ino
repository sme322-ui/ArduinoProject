int sensorPin = A5;    
int ledPin = 5;      
int sensorValue = 0; 

void setup() {
  pinMode(ledPin, OUTPUT);  
}

void loop() {
  sensorValue = analogRead(sensorPin);   
  sensorValue = map(sensorValue,0,1023,0,255);  
  analogWrite(ledPin,sensorValue);  
}
