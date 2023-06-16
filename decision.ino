void detect_cliff() {
  // Detecting black tape on either sensor
  float color_l = analogRead(A0);
  float color_r = analogRead(A1);
  float color_out = analogRead(A2);

  Serial.println("L: ");
  Serial.println(color_l);
  Serial.println("R: ");
  Serial.println(color_r);
  Serial.println("Out: ");
  Serial.println(color_out);
  
  if (color_l < 10 && color_r < 10) {
    spin_right_rad(PI/2);
    delay(4000); // Delay for 4 seconds to check color_out

    if (color_out < 10)
    {
      // Boundary detected
      move_forward_ms(1000); // Move forward continuously
    }
    else {
      // Cliff detected
      spin_right_rad(PI/2);
      move_forward_ms(100);
    }
  }
  else {
    move_forward_ms(1000);  // Move forward continuously
  }
}
