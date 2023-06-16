int const RMB_FWD_MS = 2000;

int detect_tape() {
  /*
   * Returns 2 if it's a boundary.
   * Returns 1 if it's a cliff.
   * Returns 0 if it's neither.
   */
   
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
    spin_right_rad(PI/2);    // might need to be changed since the robot doesn't always approach at a 90 degree angle to the boundary

    move_forward_ms(2000); // drive for 2s to see if the tape is still there

    color_out = analogRead(A2);

    if (color_out < 10)
    {
      return 2; // Boundary detected
    }
    else {
      // Cliff detected
      spin_right_rad(PI/2);
      move_forward_ms(100);
    }
  }
  else {
//    move_forward_ms(1000);  // Move forward continuously
  }
}

void find_corner() 
{  
  while (1) {             // loop is terminated by break; in loop
    move_forward_ms(1000);
    int tape_measurement = detect_tape();

    // todo: ultrasound check for mountains
    
    if (tape_measurement == 2) { 
        // boundary
        move_forward_until_edge();
        // spin until in the correct orientation (function not yet written)
        break;
    } 
    else if (tape_measurement == 1) {
      // cliff, go around
      
    }
  }

  // orient yourself so roomba() starts right
}

void roomba() // repeatedly call this function in main until end of sequence
{
  move_forward_ms(RMB_FWD_MS);
  spin_left_rad(PI/2);
  move_forward_until_edge();
  spin_right_rad(PI/2);
  move_forward_ms(RMB_FWD_MS);
}
