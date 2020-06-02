int leds_1[8] = {A0, A2, A4, A6, A1, A3, A5, A7};
int leds_2[8] = {A7, A6, A5, A4, A3, A2, A1, A0};

void setup(){
  int i;
  Serial.begin(9600);
  pinMode (27, INPUT_PULLUP);
  pinMode (29, INPUT_PULLUP);
  for (i = 0; i < 8; i++) {
  digitalWrite(leds_1[i], LOW);
  }
  for (i = 0; i < 8; i++) {
    pinMode (leds_1[i], OUTPUT);
  }
  for (i = 0; i < 8; i++) {
  digitalWrite(leds_2[i], LOW);
  }
  for (i = 0; i < 8; i++) {
    pinMode (leds_2[i], OUTPUT);
  }
}

void loop() {
  int i;
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == 0xA1) {
    for (i = 0; i < 8; i++) {
    digitalWrite (leds_1[i], HIGH);
    delay (1000);
    digitalWrite (leds_1[i], LOW);
    delay (750);
     }
    } else if (inByte == 0xB1) {
     for (i = 0; i < 8; i++) {
    digitalWrite (leds_2[i], HIGH);
    delay (850);
    digitalWrite (leds_2[i], LOW);
    delay (850);
     }
    }
 }
 
  // write to port 0:
  if(digitalRead(27)==LOW){
    Serial.write(0xB1);
    delay(10);
  }
   if(digitalRead(29)==LOW){
    Serial.write(0xB2);
    delay(10);
  }
}
