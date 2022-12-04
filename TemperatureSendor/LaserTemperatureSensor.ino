#include <SoftwareSerial.h>

//SoftwareSerial btSerial(8,9);//RX,TX
float vol;
char LEDstatus;
int laser = 7;         //設定雷射頭紅線接 Arduino Pin 7
char inputByte;
const int lm35Vout=0;                      //LM35 輸出連接至類比接腳 A0
const int led[3]={2,3,4};                  //LED 連接數位接腳 2~4
int showLED;                   
int val;
int tempPin = 1;
const int ledPin =  13; 
int n;
SoftwareSerial BT(10,11);
void setup()
{
  Serial.begin(57600);
   BT.begin(57600); // 設定和 HC-06 通訊的速度 (預設 9600)

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT); 
  pinMode(laser,OUTPUT);
    pinMode(7,OUTPUT);
  for(int i=0;i<=2;i++)
     {
         pinMode(led[i],OUTPUT);           //設定數位接腳 2~7 為輸出模式
         digitalWrite(led[i],LOW);         //關閉LED
     }

}
void loop()
{
  long degree;                          //宣告 degree 變數
     degree=lm35(lm35Vout);   

n = analogRead(A0);   
  vol = n * (5.0 / 1023.0*100);  
  Serial.print("temperature\n");
  Serial.print(vol);    
  Serial.print("C'");
   if(degree>=40)                        //溫度大於等於 40°C？
        showLED=0;                         //溫度大於等於 40°C，點亮紅色 LED
        digitalWrite(laser, HIGH);     //當 b=0，即按鍵按下時，開啟雷射
        
     if(degree<40 && degree>20)
        showLED=1;  
        digitalWrite(laser, LOW);                  //溫度介於 20~40°C，點亮藍色 LED
     if(degree<=20)
        showLED=2;                         //溫度小於等於 20°C，點亮綠色 LED
        digitalWrite(laser, LOW);     //當 b=0，即按鍵按下時，開啟雷射
    
  if(degree > 40){
    Serial.print("temp is high . notice!!!\n");
    digitalWrite(laser, HIGH);  
  }
  delay(500); 
/* uncomment this to get temperature in farenhite
Serial.print("TEMPRATURE = ");
Serial.print(farh);
Serial.print("*F");
Serial.println();
*/

  /*
   if(btSerial.available()){
        char i = btSerial.read();
    Serial.println(i);
    if(i == 1){
      Serial.println("Led On");
      digitalWrite(2,HIGH);
       digitalWrite(laser, HIGH);     //當 b=0，即按鍵按下時，開啟雷射      
    }
    if(i == 2){
      Serial.println("Led Off");
      digitalWrite(2,LOW);
       digitalWrite(laser, LOW);     //當 b=0，即按鍵按下時，開啟雷射      
    }
  
   }
   */
  if(BT.available()){
        char i = BT.read();
    Serial.println(i);
    if(i == 3){
      Serial.println("Led On");
      digitalWrite(7,HIGH);
      digitalWrite(laser, HIGH);   
    }
    if(i == 2){
      Serial.println("Led Off");
      digitalWrite(7,LOW);
      digitalWrite(laser,LOW);
    }
  
   }
 //判斷LED開關條件
 switch (LEDstatus){ 
  case '1':
  digitalWrite(laser,HIGH);
  break;
  case '2':
  digitalWrite(laser,LOW);
  break;
  }
  if(Serial.available()){
    BT.print(Serial.read());
  }
  if(BT.available()){
    Serial.print(BT.read());
  }
}
long lm35(int lm35Vout)
{
     int value;                            //宣告 value 變數
     long degree;                          //宣告 degree 變數
     value=analogRead(lm35Vout);           //讀取溫度數位值
     degree=(long)value*500/1024;          //轉換成攝氏溫度值
     return degree;                        //傳回攝氏溫度
}
void displayLED()
{
     for(int i=0;i<=2;i++)                 //關閉所有 LED
         digitalWrite(led[i],LOW);
    digitalWrite(led[showLED],HIGH);      //依溫度範圍條件點亮 LED
        
}
