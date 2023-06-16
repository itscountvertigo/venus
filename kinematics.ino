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


Position kine_ms(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, float driving_ms) 
{
  Position inter_pos = curr_pos;
   
  unsigned long timer_ms = millis();
  unsigned long encoder_ms = millis();
  unsigned long kine_ms = millis();
  
  while (millis() < timer_ms + driving_ms) {
    if (millis() >= encoder_ms + ENCODER_DELTA_MS) {
      spoke_read();
      
      encoder_ms = millis();
    }

    if (millis() >= kine_ms + KINE_DELTA_MS) {
      inter_pos = kine_math(curr_pos, l_wheel_clockwise, r_wheel_clockwise, passed_spokes_l, passed_spokes_r);

      // reset counters and timers
      passed_spokes_l = 0;
      passed_spokes_r = 0;  
      
      kine_ms = millis();
    }
  }
  
  return inter_pos;
   
}

Position kine_until_bound(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise)
{
  Position inter_pos = curr_pos;
   
  unsigned long timer_ms = millis();
  unsigned long encoder_ms = millis();
  unsigned long kine_ms = millis();
  unsigned long tape_ms = millis();

  float color_l = analogRead(A0);
  float color_r = analogRead(A1);
  float color_out = analogRead(A2);

//  Serial.println(color_l);
//  Serial.println(color_r);
  
  while (color_l > 5 && color_r > 5) {
    if (millis() >= encoder_ms + ENCODER_DELTA_MS) {
      spoke_read();
      
      encoder_ms = millis();
    }

    if (millis() >= kine_ms + KINE_DELTA_MS) {
      inter_pos = kine_math(curr_pos, l_wheel_clockwise, r_wheel_clockwise, passed_spokes_l, passed_spokes_r);

      // reset counters and timers
      passed_spokes_l = 0;
      passed_spokes_r = 0;  
      
      kine_ms = millis();

      Serial.println("In kine_until_bound: ");
      print_pos(inter_pos);
      Serial.println();
    }
    
    if (millis() >= tape_ms + 100) {
      color_l = analogRead(A0);
      color_r = analogRead(A1);
      color_out = analogRead(A2);

//      Serial.println(color_l);
//      Serial.println(color_r);

      tape_ms = millis();
    }
  }

  return inter_pos;
}

Position kine_target_pos(Position curr_pos, Position target_pos, int l_wheel_clockwise, int r_wheel_clockwise) 
{
  Position inter_pos = curr_pos;
   
  unsigned long timer_ms = millis();
  unsigned long encoder_ms = millis();
  unsigned long kine_ms = millis();
  
  while (!(round(100 * inter_pos.x) == round(100 * target_pos.x) && 
           round(100 * inter_pos.y) == round(100 * target_pos.y) &&
           round(100 * normalize_radians(inter_pos.angle)) == round(100 * normalize_radians(target_pos.angle)) 
          )) {
    if (millis() >= encoder_ms + ENCODER_DELTA_MS) {
      spoke_read();
      
      encoder_ms = millis();
    }

    if (millis() >= kine_ms + KINE_DELTA_MS) {
      inter_pos = kine_math(curr_pos, l_wheel_clockwise, r_wheel_clockwise, passed_spokes_l, passed_spokes_r);

      // reset counters and timers
      passed_spokes_l = 0;
      passed_spokes_r = 0;  
      
      kine_ms = millis();

      print_pos(inter_pos);
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

Position kine_math(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, int passed_spokes_l, int passed_spokes_r)
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
  
  avg_vel_l = meters_traveled_l / (KINE_DELTA_MS / 1000);      // unit m/s
  avg_vel_r = meters_traveled_r / (KINE_DELTA_MS / 1000);      // unit m/s

  // implementation of formulas found in source [1]
  int roundL = round(10 * avg_vel_l);
  int roundR = round(10 * avg_vel_r);
  Serial.print("L = ");
  Serial.println(roundL);
  Serial.print("R = ");
  Serial.println(roundR);
  
  if (round(10 * avg_vel_l) == round(10 * avg_vel_r)) {                  // straight line movement!
    inter_pos.x = inter_pos.x + (avg_vel_l + avg_vel_r) / 2 * cos(inter_pos.angle) * (KINE_DELTA_MS / 1000);
    inter_pos.y = inter_pos.y + (avg_vel_l + avg_vel_r) / 2 * sin(inter_pos.angle) * (KINE_DELTA_MS / 1000);

    Serial.println("In kine math fwd: ");
    print_pos(inter_pos);
    Serial.println();
  }
  else if (round(10 * avg_vel_l) == -round(10 * avg_vel_r)) {        // you spin me right round baby right round
    inter_pos.angle = inter_pos.angle + (avg_vel_l + avg_vel_r) * (KINE_DELTA_MS / 1000) / DIST_BETWEEN_WHEELS;
  }
  else {
      Serial.println("In kine_math else: ");
      print_pos(inter_pos);
      Serial.println();
    
    ICC_R = DIST_BETWEEN_WHEELS / 2 * (avg_vel_l + avg_vel_r) / (avg_vel_r - avg_vel_l);
    ICC_omega = (avg_vel_r - avg_vel_l) / DIST_BETWEEN_WHEELS;
    
    ICC_x = inter_pos.x - ICC_R * sin(inter_pos.angle);
    ICC_y = inter_pos.y + ICC_R * cos(inter_pos.angle);
    
    inter_pos.x = cos(ICC_omega * (KINE_DELTA_MS / 1000)) * ICC_R * sin(inter_pos.angle)
                + sin(ICC_omega * (KINE_DELTA_MS / 1000)) * ICC_R * cos(inter_pos.angle)
                + ICC_x;

    inter_pos.y = sin(ICC_omega * (KINE_DELTA_MS / 1000)) * ICC_R * sin(inter_pos.angle)
                - cos(ICC_omega * (KINE_DELTA_MS / 1000)) * ICC_R * cos(inter_pos.angle)
                + ICC_y;

    inter_pos.angle = inter_pos.angle + ICC_omega * (KINE_DELTA_MS / 1000);
  }

  return inter_pos;
}

float normalize_radians(float rad) 
{
  rad = fmod(rad, 2*PI);
  
  if (rad < 0) {
    rad += 2*PI;
  }

  return rad;
}
