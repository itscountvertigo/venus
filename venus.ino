#include <stdio.h>
#include <string.h>
#include <Servo.h>

const float WHEEL_DIAMETER = 0.066;     // m
const int NUM_OF_SPOKES = 32;
const float DIST_BETWEEN_WHEELS = 0.11;   // m

const float left_wheel_scaling = 1.2;

typedef struct Position {
  float x;
  float y;
  float angle;
} Position;

typedef struct IR 
{
  int left_sensor = 0;
  int right_sensor = 0;
} IR;

// function declarations:
Position kine_measure(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, float driving_ms, float encoder_delta_ms, float kine_delta_ms);
IR ir_measure();

Position current_pos = {0, 0, 3.1415/2};

Servo servoLeft;
Servo servoRight;

Servo servoGrabber;
Servo servoSensor;

void setup()
{
  Serial.begin(9600);
}  

void loop()
{
  servo_attach();
  
  // 1300 clockwise, 1500 still, 1700 counterclockwise
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);

  current_pos = kine_measure(current_pos, 0, 1, 20000, 5, 500);

  servo_detach();

  Serial.println("X: ");
  Serial.println(current_pos.x);
  Serial.println("Y: ");
  Serial.println(current_pos.y);
  Serial.println("Angle: ");
  Serial.println(current_pos.angle);

  delay(10000);

}

void servo_attach()
{
  servoLeft.attach(12);
  servoRight.attach(13);

  servoGrabber.attach(10);
  servoSensor.attach(11);
}

void servo_detach()
{
  servoLeft.detach();
  servoRight.detach();

  servoGrabber.detach();
  servoSensor.detach();
}
