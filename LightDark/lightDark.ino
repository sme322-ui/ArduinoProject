int sensorPin = A5;    //類比輸入的PIN腳
int ledPin = 5;      // 類比輸出的PIN腳，接到LED的正極
int sensorValue = 0;  // 類比輸入得到數值

void setup() {
  pinMode(ledPin, OUTPUT);  //LED的PIN腳設為輸出
}

void loop() {
  sensorValue = analogRead(sensorPin);   //讀取類比輸入的值會得到0~1023
  sensorValue = map(sensorValue,0,1023,0,255);  //將0~1023轉化成0~255
  analogWrite(ledPin,sensorValue);  //將結果輸出給LED，改變亮度
}
