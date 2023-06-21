// 1 => forwards, 0 => backwards
void setMotorPolarity (bool Polarity) {
    const int intakePin = 4;
    if (Polarity == 0) {
        digitalWrite(intakePin, HIGH);
    } else {
        digitalWrite(intakePin, LOW);
    }
    return Polarity;
}


/*insert this code into the main program loop
const int blocksPin = 4;

bool blockVal[3] = { 0 };
int blocks = 0;
unsigned long intake_t = millis();
unsigned long intake_t_prev = millis();

void setup() {
  pinMode(blocksPin, INPUT_PULLUP);
}

void loop() {

    intake_t = millis();
    if (intake_t >= (intake_t_prev + 10)) {
        blockVal[2] = blockVal[1];
        blockVal[1] = blockVal[0];
        blockVal[0] = digitalRead(blocksPin);
        if (blockVal[0] == 1 && blockVal[1] == 1 && blockVal[2] == 1) {
            blocks++;
            intake_t_prev = intake_t + 3000;
        } else {
            intake_t_prev = intake_t;
        }
    }
}
*/
