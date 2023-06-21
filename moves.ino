/*
 *   1300 clockwise
 *   1500 still
 *   1700 counterclockwise
 */

void move_forward_ms(float ms) 
{
  servo_attach();
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);

  current_pos = kine_ms(current_pos, 0, 1, ms);

  servo_detach();
}

void move_forward_until_edge() 
{
  servo_attach();
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);

  Serial.println("In move_forward_until_edge: ");
  print_pos(current_pos);
  Serial.println();

  current_pos = kine_until_bound(current_pos, 0, 1);
  
  servo_detach();
}

void spin_right_rad(float rad) 
{
  servo_attach();
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1700);

  Position target_pos = current_pos;
  target_pos.angle -= rad;

  current_pos = kine_target_pos(current_pos, target_pos, 0, 0);
  
  servo_detach();
}

void spin_left_rad(float rad) 
{
  servo_attach();
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1300);

  Position target_pos = current_pos;
  target_pos.angle += PI/2;

  current_pos = kine_target_pos(current_pos, target_pos, 1, 1);
  
  servo_detach();
}

void spin_right_ms(float ms) 
{
  servo_attach();
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1700);

  current_pos = kine_ms(current_pos, 0, 0, ms);
  
  servo_detach();
}

void spin_left_ms(float ms) 
{
  servo_attach();
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1300);

  current_pos = kine_ms(current_pos, 0, 0, ms);
  
  servo_detach();
}
