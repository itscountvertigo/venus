#include <stdio.h>
#include <string.h>
#include <Servo.h>

const float WHEEL_DIAMETER = 0.066;     // m
const int NUM_OF_SPOKES = 32;
const float DIST_BETWEEN_WHEELS = 0.11;   // m

typedef struct Position {
  float x;
  float y;
  float angle;
} Position;

void print_pos(Position pos) 
{
  Serial.println("x: "); Serial.print(pos.x);
  Serial.println("y: "); Serial.print(pos.y);
  Serial.println("a: "); Serial.println(pos.angle);  
}

Servo servoLeft;
Servo servoRight;

Servo servoGrabber;
Servo servoSensor;

// function declarations:

void servo_attach();
void servo_detach();

Position kine_ms(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, float driving_ms, float encoder_delta_ms, float kine_delta_ms);
Position kine_target_pos(Position curr_pos, Position target_pos, int l_wheel_clockwise, int r_wheel_clockwise, float encoder_delta_ms, float kine_delta_ms);

void move_forward_ms(float ms);
void move_forward_until_edge();
void spin_right_rad(float rad);
void spin_left_rad(float rad);

Position current_pos = {0, 0, PI/2};

void setup()
{
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT); 
}  

void loop()
{  
//  find_corner();

  move_forward_until_edge();
  spin_right_rad(PI/2);

  
  Serial.println("X: ");
  Serial.println(current_pos.x);
  Serial.println("Y: ");
  Serial.println(current_pos.y);
  Serial.println("Angle: ");
  Serial.println(current_pos.angle);

  delay(10000);

}
