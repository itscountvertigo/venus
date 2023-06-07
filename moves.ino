/*
 *   1300 clockwise
 *   1500 still
 *   1700 counterclockwise
 */

Position move_forward_ms(float ms) 
{
  servo_attach();
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);

  current_pos = kine_ms(current_pos, 0, 1, ms, 5, 500);

  servo_detach();
  return current_pos;
}

Position spin_right_rad(float rad) 
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1700);

  Position target_pos = current_pos;
  target_pos.angle -= PI/2;

  current_pos = kine_target_pos(current_pos, target_pos, 0, 0, 5, 500);
}

Position spin_left_rad(float rad) 
{
  servoLeft.writeMicroseconds(1300);
  servoRight.writeMicroseconds(1300);

  Position target_pos = current_pos;
  target_pos.angle += PI/2;

  current_pos = kine_target_pos(current_pos, target_pos, 1, 1, 5, 500);
}
