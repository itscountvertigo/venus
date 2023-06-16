const int pingPin = 9;

long ultrasound_read() {
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

  return cm;

  /*
  Serial.print(cm);
  Serial.print("cm");
  int i = (cm/313)*10;
  for (int z=0;z<i;z++) {
    Serial.println("-");
  }
  i = 0;
  */
}

long microsecondsToCentimeters(long microseconds){
  return microseconds/29/2;
}
