/*
 *  Contains code to measure and calculate relative position of the robot based on encoder data.
 *  Applies forward kinematics formulas from https://www.cs.columbia.edu/~allen/F19/NOTES/icckinematics.pdf ([1])
 * 
 */

int spoke_stat_l;
int spoke_stat_r;

int spoke_stat_l_prev;
int spoke_stat_r_prev;

float meters_traveled_l;
float meters_traveled_r;

int passed_spokes_l;
int passed_spokes_r;

float avg_vel_l;
float avg_vel_r;

float ICC_R;
float ICC_omega;
float ICC_x;
float ICC_y;

Position kine_ms(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, float driving_ms, float encoder_delta_ms, float kine_delta_ms) 
{
  Position inter_pos = curr_pos;
   
  unsigned long timer_ms = millis();
  unsigned long encoder_ms = millis();
  unsigned long kine_ms = millis();
  
  while (millis() < timer_ms + driving_ms) {
    if (millis() >= encoder_ms + encoder_delta_ms) {
      spoke_read();
      
      encoder_ms = millis();
    }

    if (millis() >= kine_ms + kine_delta_ms) {
      inter_pos = kine_math(curr_pos, l_wheel_clockwise, r_wheel_clockwise, kine_delta_ms, passed_spokes_l, passed_spokes_r);

      // reset counters and timers
      passed_spokes_l = 0;
      passed_spokes_r = 0;  
      
      kine_ms = millis();
    }
  }
  
  return inter_pos;
}

Position kine_target_pos(Position curr_pos, Position target_pos, int l_wheel_clockwise, int r_wheel_clockwise, float encoder_delta_ms, float kine_delta_ms) 
{
  Position inter_pos = curr_pos;
   
  unsigned long timer_ms = millis();
  unsigned long encoder_ms = millis();
  unsigned long kine_ms = millis();
  
  while (!(round(1000 * inter_pos.x) == round(1000 * target_pos.x) && 
           round(1000 * inter_pos.y) == round(1000 * target_pos.y) &&
           round(1000 * inter_pos.angle) == round(1000 * target_pos.angle) 
          )) {
    if (millis() >= encoder_ms + encoder_delta_ms) {
      spoke_read();
      
      encoder_ms = millis();
    }

    if (millis() >= kine_ms + kine_delta_ms) {
      inter_pos = kine_math(curr_pos, l_wheel_clockwise, r_wheel_clockwise, kine_delta_ms, passed_spokes_l, passed_spokes_r);

      // reset counters and timers
      passed_spokes_l = 0;
      passed_spokes_r = 0;  
      
      kine_ms = millis();
    }
  }

  return inter_pos;
}

void spoke_read() 
{
  spoke_stat_l_prev = spoke_stat_l;
  spoke_stat_r_prev = spoke_stat_r;

  spoke_stat_l = digitalRead(8);
  spoke_stat_r = digitalRead(7);

  // update passed spokes on encoder
  if (spoke_stat_l != spoke_stat_l_prev) {
    passed_spokes_l++;
    }
  if (spoke_stat_r != spoke_stat_r_prev) {
    passed_spokes_r++;
  }
}

Position kine_math(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, int kine_delta_ms, int passed_spokes_l, int passed_spokes_r)
{
  Position inter_pos = curr_pos;
  if (l_wheel_clockwise == 0) {
    passed_spokes_l *= -1;
  }
  if (r_wheel_clockwise == 1) {
    passed_spokes_r *= -1;
  }
  
  meters_traveled_l = 0.5 * passed_spokes_l / NUM_OF_SPOKES * PI * WHEEL_DIAMETER;
  meters_traveled_r = 0.5 * passed_spokes_r / NUM_OF_SPOKES * PI * WHEEL_DIAMETER;
  
  avg_vel_l = meters_traveled_l / (kine_delta_ms / 1000);      // unit m/s
  avg_vel_r = meters_traveled_r / (kine_delta_ms / 1000);      // unit m/s

  // implementation of formulas found in source [1]
  
  if (round(1000 * avg_vel_l) == round(1000 * avg_vel_r)) {                  // straight line movement!
    inter_pos.x = inter_pos.x + (avg_vel_l + avg_vel_r) / 2 * cos(inter_pos.angle) * (kine_delta_ms / 1000);
    inter_pos.y = inter_pos.y + (avg_vel_l + avg_vel_r) / 2 * sin(inter_pos.angle) * (kine_delta_ms / 1000);
  }
  else if (/*value_near(avg_vel_l, avg_vel_r * -1, 0.1) == 1*/ 0) {        // you spin me right round baby right round
    inter_pos.angle = inter_pos.angle + (avg_vel_l + avg_vel_r) * (kine_delta_ms / 1000) / DIST_BETWEEN_WHEELS;
  }
  else {
    ICC_R = DIST_BETWEEN_WHEELS / 2 * (avg_vel_l + avg_vel_r) / (avg_vel_r - avg_vel_l);
    ICC_omega = (avg_vel_r - avg_vel_l) / DIST_BETWEEN_WHEELS;
    
    ICC_x = inter_pos.x - ICC_R * sin(inter_pos.angle);
    ICC_y = inter_pos.y + ICC_R * cos(inter_pos.angle);
    
    inter_pos.x = cos(ICC_omega * (kine_delta_ms / 1000)) * ICC_R * sin(inter_pos.angle)
                + sin(ICC_omega * (kine_delta_ms / 1000)) * ICC_R * cos(inter_pos.angle)
                + ICC_x;

    inter_pos.y = sin(ICC_omega * (kine_delta_ms / 1000)) * ICC_R * sin(inter_pos.angle)
                - cos(ICC_omega * (kine_delta_ms / 1000)) * ICC_R * cos(inter_pos.angle)
                + ICC_y;

    inter_pos.angle = inter_pos.angle + ICC_omega * (kine_delta_ms / 1000);
  }

  return inter_pos;
}
