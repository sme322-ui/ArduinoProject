#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <U8g2lib.h>
#include <WiFiClientSecure.h>
#include <TridentTD_LineNotify.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFiMulti.h>
#include <MySQL_Connection.h>   
#include <MySQL_Cursor.h>
#include "HTTPSRedirect.h"
#include <Adafruit_Sensor.h>
#include <Streaming.h>
#include <Hash.h>
#include <Ethernet.h>
#include <Wire.h> 
#include <mysql.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
//#include <Adafruit_SH1106.h>
#include "DHT.h"      // DHT庫 
const char* ssid = "55Y8817";
const char* password = "HN73108378";

// 設定OLED
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
int val,val2;
#define DHTPIN 2     // 定義DHT的引腳
#define DHTTYPE DHT22 // 定義dht類型

double temp;
int i = 0;
String msg;
char* db_user="root";
char* db_password="123456";
char INSERT_SQL[] = "INSERT INTO temp.temp(timStamp,temperature,hemidity) VALUES (2,'22.30','53.60')";
int RawValue = 0;
double Voltage = 0;
int j=0;
const byte LM35 = A0;
//int ledPin = 13;
byte mac_addr[] = { 0x14, 0x98, 0x77, 0x62, 0x5e, 0x5e };
WiFiServer server(80);
DHT dht(DHTPIN, DHTTYPE); // 初始化DHT11傳感器
IPAddress server_addr(192,168,1,103);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,1,1);
IPAddress ip(192,168,1,104); 
String apiKey = "8E4AYSKLEHK5UT0Z";
//String apiKey = "bzg0A-N-O7mJ9KvZFHEVIX";
String LineTocken = "9s4tEtJbOyGVGsZ5xTuhPsmTvvxy1THgGRUtuYBUPag";
ESP8266WiFiMulti WiFiMulti;
WiFiClientSecure clientS;
char host[] = "notify-api.line.me";
HTTPSRedirect* clientR;
float Temperature;
float Humidity;
EthernetClient client,clientE;
ESP8266WebServer serverE(80);
WiFiClient clientW;                 // 聲明一個Mysql客戶端，在lianjieMysql中使用
MySQL_Connection conn(&clientW);
MySQL_Cursor* cursor;		
String jsonStr = "{'temp':30}";
void setup() {
  dht.begin();
  Ethernet.begin(mac_addr,ip,gateway,subnet);
  Serial.begin(115200);
 //Ethernet.begin(mac_addr);
  WiFi.mode(WIFI_STA);
  Serial.println( ssid );
  WiFi.begin(ssid,password);
  Serial.println();
  Serial.print("Connecting");
  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }
  Serial.println("WiFi connected");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  server.begin();
  Serial.println("NodeMCU Server started");
  delay(2000);
  
  if(client.connect(server_addr,5461)){
    Serial.println("connected");

    client.println("GET /th?t=25.79&h=60 HTTP/1.1");
    client.println();
  }else{
    Serial.println("connection failed");
  }


  clientS.setInsecure();
 //Ethernet.begin(mac_addr);
   cursor = new MySQL_Cursor(&conn);    // 創建一個數據庫遊標實例
  // httpSend();
 //  readAndRecordData();
 if (conn.connected()) {
      // do something your work 

    cursor->execute(INSERT_SQL); 
    delete cursor; 
  } else {
    conn.close();
    Serial.println("Connecting...");
    delay(200); //
    if (conn.connect(server_addr, 3306, db_user, db_password)) {
      delay(500);
      Serial.println("Successful reconnect!");
    } else {
      Serial.println("Cannot reconnect! Drat.");
    }
  }
 
}

void loop() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
 /* do {
    draw();
    u8g2.drawStr(5,10,msg.c_str());
  } while ( u8g2.nextPage() );*/
  WiFiClient client = server.available();
  HTTPClient http;
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  RawValue = analogRead(LM35);//RawValue=analogRead(analogIn)->RawValue:val analogIn->LM35
  Voltage = (RawValue/204.8)*3301;


  temp = Voltage*0.12+4;

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("<meta charset='utf-8'>");  
    client.print("<style>");
    client.print(".text1{background-color:#ff69b4;}");
    client.print(".bodyDesign { margin: auto;border: solid 2px #0094ff;font-size: 15px;font-family: Verdana;background-color: #dffe91;}");

    client.print(".text3{background-color:#ffffc5;}");
    client.print(".text2{background-color:gold;}");
    client.print("</style>");
    client.print("<center><h1 style='color:purple;'><p class='text3'>雲端農業送貨系統</p></h1></center>");
     client.print("<script type = 'text/javascript' src='js/jquery-3.6.3.min.js'></script>");
     client.print("<body class='bodyDesign'>");
     client.print("<center><h2 style='color:blue;'><p class='text2'>");
  Serial.print("Temp : ");
 //Serial.println(sensors.getTempCByIndex(0));
int jsonStr = http.POST("{\"temp\":17.70, \"humid\":59.70}");
   
  client.print("監控溫度 : ");
  client.print("</p>"); 
// client.print(sensors.getTempCByIndex(0));

 client.print("<p class='text1'>");

  client.print(temp);//client.print((val*0.489+1)*6);
 
  client.print("<h2>°C</h2>");
  
  client.print("</h2></p></center>");
  client.print("</body>");
  client.println("</html>");
  String response2 = client.readString();
  Serial.print(response2);
  
  client.stop();
sendData();
 
  if(temp>50){
    String message = "監控溫度：";

    
         int LEN = message.length();

         String url2 = "/api/notify/";
         clientS.println("POST " + url2 + "HTTP/1.1");

       //  clientS.print("Host:");
     //    clientS.println(host);
         clientS.println("Authorization: Bearer "+LineTocken);
         clientS.println("Content-Type: application/x-www-form-urlencoded");

         clientS.println("Content-Length:"+String((LEN+8)));
         clientS.println();
         clientS.println("message="+message);
         clientS.println("POST /temp HTTP/1.1");
         clientS.println("Content-Type: application/json");
         clientS.print("Content-Length: ");
//client.println(jsonStr.length());
         clientS.println();
         clientS.print(jsonStr);
         delay(2000);
         String response = clientS.readString();
         Serial.println(response);
         Serial.println(message);
         clientS.stop();
           Serial.println(LINE.getVersion());
           i++;
    data_to_web(String(123),String(i));
  LINE.setToken(LineTocken);

  // 先換行再顯示
  LINE.notify(message+temp/2);
// readAndRecordData();
  
    }
    else{
      Serial.println("Connect failed");
    }
   
  Serial.println(LINE.getVersion());

  LINE.setToken(LineTocken);


   readAndRecordData();
}
// 讀取傳感器的數據並寫入到數據庫
void readAndRecordData(){
				  Serial.print(dht.readTemperature());      	// 在串口中打印讀取到的溫度
				  Serial.print(",\t"); 
                  Serial.println(dht.readHumidity());             // 在串口中打印讀取到的溼度
                  char buff[128];                                         // 定義存儲傳感器數據的數組
                  char tem[5];			
                  char hem[4];				
                  // 將傳感器採集的浮點數轉換爲3位整數一位小數的字串放入temp
                  dtostrf(dht.readHumidity(),3,1,tem);
                  dtostrf(dht.readTemperature(),2,1,hem);
                  sprintf(buff,INSERT_SQL,tem,hem);                 // 講tem和hem中數據放入SQL中
                  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);         // 創建一個Mysql實例
                  cur_mem->execute(INSERT_SQL);         // 將採集到的溫溼度值插入數據庫中
                  Serial.println("讀取傳感器數據，並寫入數據庫");
                 // delete cur_mem;        // 刪除mysql實例爲下次採集作準備
}
void handle_OnConnect() {

 Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  serverE.send(200, "text/html", SendHTML(Temperature,Humidity)); 
}

void sendData(){
 
   WiFiClient wifiClient;
   HTTPClient http;
   //String urlStr = "https://maker.ifttt.com/trigger/Room/with/key/bzg0A-N-O7mJ9KvZFHEVIX";
  String urlStr = "http://api.thingspeak.com/update?api_key";
     urlStr += apiKey +"&field1=";
  urlStr += String(temp);
  urlStr+=" HTTP/1.1\n";
   String urlE = urlStr+"?value1="+(int)temp;
 
  http.begin(wifiClient,urlE);
    
  http.end();
  

}
String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 NodeMCU Weather Report</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


void data_to_web(String data3, String data4){
   WiFiClient wifiClient;
String url4 = "http://";
       url4+=host;
       url4+="/";
       url4+=data3;
       url4+=",";
       url4+=data4;
  // This will send the request to the server
if((WiFiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        http.begin(wifiClient,url4);
        int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();  }
        } else {

        }
        http.end(); 
        }
 }
 
void httpSend(){
  RawValue = analogRead(LM35);//RawValue=analogRead(analogIn)->RawValue:val analogIn->LM35
  Voltage = (RawValue/204.8)*3301;


  temp = Voltage*0.3+5;

  char tBuffer[6] = "19.47";
  char hBuffer[6] = "57.00";
 Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humi
  if(clientE.connect(server_addr,5502)){
       Serial.print("connected");
       client <<"GET /th?t="
      <<dtostrf(Temperature,5,2,tBuffer)
       <<"&h"
       <<dtostrf(Humidity,5,2,hBuffer)
       <<" HTTP/1.1\n";
  }
  
}
 
