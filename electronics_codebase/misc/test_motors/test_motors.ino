#define motor2_B 2
#define motor2_P 3
#define motor2_Y 4
#define motor2_O 5

long motorDelay = 1200L;  // fiddle with this to control the speed

byte patterns[8] = {
  0b00001000,
  0b00001100,
  0b00000100,
  0b00000110,
  0b00000010,
  0b00000011,
  0b00000001,
  0b00001001,
};


void runsingle_step(long motorDelay);

void setup() {
  // put your setup code here, to run once:

  pinMode(motor2_B, OUTPUT);
  pinMode(motor2_P, OUTPUT);
  pinMode(motor2_Y, OUTPUT);
  pinMode(motor2_O, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  runsingle_step(motorDelay);
}


void runsingle_step(long motorDelay) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(motor2_B, bitRead(patterns[i], 0));
    digitalWrite(motor2_P, bitRead(patterns[i], 1));
    digitalWrite(motor2_Y, bitRead(patterns[i], 2));
    digitalWrite(motor2_O, bitRead(patterns[i], 3));
    delayMicroseconds(motorDelay);
  }
}
