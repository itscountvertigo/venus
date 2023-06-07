void detect_cliff() {
  // Detecting black tape on either sensor
  color_l = analogRead(CNY1);
  color_r = analogRead(CNY2);
  color_out = analogRead(CNY3);
  
  if (color_l < 10 && color_r < 10)
    {
      turn_right(900);
      servo_attach(); 
      delay(4000); // Delay for 4 seconds to check color_out

      if (color_out < 10)
      {
        // Boundary detected
        move_forward(100); // Move forward continuously
      }
      else
      {
        // Cliff detected
        turn_right(900);
        move_forward(100);
      }
    }
    else
    {
      move_forward(100); // Move forward continuously
    }
  }
}