int i;
int button_pin=2;
void setup() {
Serial.begin(9600);
pinMode(button_pin, INPUT);
for (i=3;i<11;i++){
  pinMode(i, OUTPUT);
  }
}
 
void loop() {
  int sensorValue = digitalRead(button_pin);
  Serial.println(sensorValue, DEC);
  if(digitalRead(button_pin)==HIGH)
  {
    for(i=11;i>2;i--)
      {
      digitalWrite(i,HIGH);
      delay(450);
      digitalWrite(i,LOW);
      delay(450);
      
      }
  }
  else 
  {
  digitalWrite(i,LOW);
  }
} 
 
