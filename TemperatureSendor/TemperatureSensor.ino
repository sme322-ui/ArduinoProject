int val;
int tempPin = 1;
const int ledPin =  2; 
void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT); 
}
void loop()
{
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000;
  float cel = mv/10;
  float farh = (cel*9)/5 + 32;
  Serial.print("TEMPRATURE = ");
  Serial.print(cel);
  Serial.print("*C");
  if(cel>40){
       Serial.println("temp is hight,notice");
      digitalWrite(ledPin, HIGH);
    }else{
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(ledPin, LOW);
      }
  Serial.println();
  delay(1000);
/* uncomment this to get temperature in farenhite
Serial.print("TEMPRATURE = ");
Serial.print(farh);
Serial.print("*F");
Serial.println();
*/
}
