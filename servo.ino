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
