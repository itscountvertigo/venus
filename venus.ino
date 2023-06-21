#include <stdio.h>
#include <string.h>
#include <Servo.h>

const float WHEEL_DIAMETER = 0.066;     // m
const int NUM_OF_SPOKES = 32;
const float DIST_BETWEEN_WHEELS = 0.11;   // m
const float KINE_DELTA_MS = 500;
const float ENCODER_DELTA_MS = 5;


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

Position kine_ms(Position curr_pos, int l_wheel_clockwise, int r_wheel_clockwise, float driving_ms);
Position kine_target_pos(Position curr_pos, Position target_pos, int l_wheel_clockwise, int r_wheel_clockwise);

void move_forward_ms(float ms);
void move_forward_until_edge();
void spin_right_rad(float rad);
void spin_left_rad(float rad);
void spin_right_ms(float ms);
void spin_left_ms(float ms);

void setMotorPolarity (bool Polarity);

long ultrasound_read();

Position current_pos = {0, 0, PI/2};

void setup()
{
  Serial.begin(9600);
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT); 

  pinMode(4, OUTPUT);

  setMotorPolarity(1);
}  

void loop()
{
  move_forward_until_edge();
//  spin_right_ms(600);

//  move_forward_ms(10000);

  for (int i = 0; i < 5; i++) {
    setMotorPolarity(0);
    delay(2500);

    setMotorPolarity(1);
    delay(500);
  }

  setMotorPolarity(0);

  delay(2000);
  return 0;

//  roomba();
  
  Serial.println("X: ");
  Serial.println(current_pos.x);
  Serial.println("Y: ");
  Serial.println(current_pos.y);
  Serial.println("Angle: ");
  Serial.println(current_pos.angle);
//  delay(2000);

   
  //detect_cliff();
  
  //if (ultrasound_read() < 50) {
    //while (ultrasound_read() >= 27){
      //move_forward_ms(500); 
   // }
  //}
  

  //Serial.println(ultrasound_read()); 

//  delay(100);
  
}
