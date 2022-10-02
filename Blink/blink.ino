const byte LED = 22;
char val;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);

  Serial.begin(9600);
  Serial.println("Arduino Ready");
}

void loop() {
  // put your main code here, to run repeatedly:
   if(Serial.available()){
       val = Serial.read();
       switch(val){
           case '0':
             digitalWrite(LED,LOW);
             Serial.println("LED OFF");
             break;
           case '1':
             digitalWrite(LED,HIGH);
             Serial.println("LED ON");
             break;
        }
    }
}
