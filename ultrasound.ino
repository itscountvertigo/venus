const int pingPin = 9;
void setup() {
Serial.begin(9600);

}

void loop() {
long duration, inches, cm;
pinMode(pingPin,OUTPUT);
digitalWrite(pingPin,LOW);
delayMicroseconds(2);
digitalWrite(pingPin,HIGH);
delayMicroseconds(5);
digitalWrite(pingPin,LOW);


pinMode(pingPin,INPUT);
duration = pulseIn(pingPin,HIGH);
cm = microsecondsToCentimeters(duration);
Serial.print(cm);
Serial.print("cm");
int i = (cm/313)*10;
for (int z=0;z<i;z++) {
  Serial.println("-");
}
i = 0;
Serial.println();
delay(10);

}

long microsecondsToCentimeters(long microseconds){
  return microseconds/29/2;
  
  }
