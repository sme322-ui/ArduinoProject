#include <SoftwareSerial.h>
#define SENSOR A0
SoftwareSerial btSerial(2, 3); // RX, TX
int laser = 8;

float temp, val;

void setup() {
 Serial.begin(9600);           //設定 Arduino 序列螢幕和電腦傳輸資料的速率為 9600 bits/s
  btSerial.begin(9600);
 pinMode(laser, OUTPUT);       // Arduino 輸出電壓至雷射頭
 Serial.println("t (ms), T(°C)");
}

void loop() {
if (btSerial.available()){  //如果藍牙有傳資料過來
    int i = btSerial.read();  //把讀到的資料丟給i
        val = analogRead(SENSOR);
   temp = val*(0.5/1023.0*100);
    Serial.print(millis());
    Serial.print(", ");
    Serial.println(temp);
    delay(10000);
    Serial.println(i);
    if(i == 1){    //如果是1，亮燈
      Serial.println("Led OFF");
      digitalWrite(laser,LOW);
    }
    if(i == 3){    //如果是2，熄燈
      Serial.println("Raser on");
      digitalWrite(laser,HIGH);
   
    }
   
  }
}
