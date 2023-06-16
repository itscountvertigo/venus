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

Servo servoLeft;
Servo servoRight;

Servo servoGrabber;
Servo servoSensor;

// function declarations:

void servo_attach();
void servo_detach();

Position kine_ms(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, float driving_ms, float encoder_delta_ms, float kine_delta_ms);

Position move_forward_ms(float ms);
Position spin_right_rad(float rad);
Position spin_left_rad(float rad);

long ultrasound_read();

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
  Serial.println("X: ");
  Serial.println(current_pos.x);
  Serial.println("Y: ");
  Serial.println(current_pos.y);
  Serial.println("Angle: ");
  Serial.println(current_pos.angle);

  //detect_cliff();
  
  //if (ultrasound_read() < 50) {
    //while (ultrasound_read() >= 27){
      //move_forward_ms(500); 
   // }
  //}
  

  //Serial.println(ultrasound_read()); 

  delay(100);
  
}

