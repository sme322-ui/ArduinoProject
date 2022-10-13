void setup() {
  Serial.begin(9600);
  pinMode(3,OUTPUT);
}

void loop() {
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  if(sensorValue < 100){   //當光敏電阻給的值小於基準值。這個基準值依個別情況修改
    digitalWrite(3,HIGH);  //燈亮
  }else{
    digitalWrite(3,LOW);   //燈滅
  }
  delay(10);
}
