 
#include <SPI.h> 
#include "WiFiEsp.h"
 
//#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial softserial(6, 7); // RX, TX
//#endif
float temp=0;
int analog= 0;
char ssid[] = "CHT-8117";   // replace *** with your router wifi SSID (name)
char pass[] = "HN73108378";   // replace *** with your router wifi password
int status = WL_IDLE_STATUS;
int reqCount = 0;                // number of requests received
String connID;
WiFiEspServer server(80);
String readString; 

void setup(){
  Serial.begin(115200);

  // initialize serial for ESP module
    softserial.begin(115200);
  softserial.write("AT+CIOBAUD=9600\r\n");
  softserial.write("AT+RST\r\n");
  softserial.begin(115200);
  // initialize ESP module
  WiFi.init(&softserial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  sendATcmd("AT+RST\r\n",2000);
  sendATcmd("AT+CWMODE=2\r\n",1000);
  sendATcmd("AT+CIFSR\r\n",1000);
  sendATcmd("AT+CIPSERVER=1,80\r\n",1000);

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}

void loop(){ 
  // Create a client connection 
  WiFiEspClient client = server.available();
  if (client) { 
    while (client.connected()) { 
      if (client.available()) { 
        char c = client.read();
        //read char by char HTTP request 
        while (readString.length() < 100 && c != 0x0D  ) {
        //store characters to string 
        readString += c; 
        c = client.read();
      }
      Serial.println(readString);
     
      String msg="HTTP/1.1 200 OK\n\rContent-Type: text/html\n\r\n\r";
      msg +="<HTML><meta http-equiv=\"refresh\" content=\"20\"><BODY><H1 style=\"color:green;\">ESP8266 Module Lesson 2</H1><br>";
      msg +="<p>Real time temperature: ";
      
      msg +=  " C ;real time Humidity: " ;
      msg+="Temperature:";
      analog= analogRead(A0);
      temp = analog*0.48828125;
      msg+=temp;
      msg += "%</p></BODY></HTML>";
      client.println(msg);
      delay(100); 
      String cipSend = "AT+CIPSEND";
      cipSend +=connID;
      cipSend +=",";
            
       cipSend+="\r\n";

//sendATcmd(cipSend,1000);


   String cipClose = "AT+CIPCLOSE=";
   cipClose +=connID;
   cipClose+="\r\n";


      //stopping client 
      client.stop();
      //clearing string for next read 
      readString="";

      if(Serial.available()){
        softserial.print(Serial.read());
      }
      if(softserial.available())
      {
        Serial.print(softserial.read());
      }

    } 
  } 
 } 
}
void sendATcmd(char* cmd, unsigned int delay) {
}

void sendHTML(byte connID,char* msg) {
    extern int connCount; 
    String html;
    char cipSend[128];
    char cipClose[128];


   
    html += "<html><head>";
    html += "<meta http-equiv=\"refresh\" content=\"10\">";
    html +="<title>From ESP-01</title></head><body>\n\r";
    html += "<p>ClientMsg: ";
    html += msg;
    html += "</p>\n\r";
    html += "<p>Humidity:";

    html += "%</p>\n\r";
    html += "<p>Temperature:";

    html += "*C</p>\n\r";
    html += "</body></html>";
    Serial.println(html);
    sprintf(cipSend,"AT+CIPSEND=%d,%d\r\n",connID,html.length());
    sprintf(cipClose,"AT+CIPCLOSE=%d\r\n",connID);
    sendATcmd(cipSend,1000);
    sendATcmd(html.c_str(),1000);
    sendATcmd(cipClose,1000);
}
void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:t ");
  Serial.println(ip);
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
