#include <Wire.h>


#define SLAVE_DRIVER 4


// 28BYJ-48, using an NTE2019 darlington (similar to uln2003) driver
//   Blue   - 28BYJ48 pin 1 <-- 2019 <-- arduino pin 8
//   Pink   - 28BYJ48 pin 2 <-- 2019 <-- arduino pin 9
//   Yellow - 28BYJ48 pin 3 <-- 2019 <-- arduino pin 10
//   Orange - 28BYJ48 pin 4 <-- 2019 <-- arduino pin 11
//   Red    - 28BYJ48 pin 5 <-- vcc

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


void runsingle_step(long motorDelay = motorDelay) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(motor2_B, bitRead(patterns[i], 0));
    digitalWrite(motor2_P, bitRead(patterns[i], 1));
    digitalWrite(motor2_Y, bitRead(patterns[i], 2));
    digitalWrite(motor2_O, bitRead(patterns[i], 3));
    delayMicroseconds(motorDelay);
  }
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin(SLAVE_DRIVER);

  Serial.begin(9600);
  Wire.onReceive(receiveEvent); // when recieving data
  Wire.onRequest(requestEvent);    // when sending data


  pinMode(motor2_B, OUTPUT);
  pinMode(motor2_P, OUTPUT);
  pinMode(motor2_Y, OUTPUT);
  pinMode(motor2_O, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

}


//YYxYYYYYY
//First two are command
//last are values
//x - seperator
void receiveEvent(int numbytes) {

  String dataString = "";

  while (Wire.available()) {
    char c = Wire.read();
    dataString = dataString + c;
  }

  float command_value = dataString.substring(3).toFloat();
  int command_in = dataString.substring(0, 2).toInt();

  Serial.print(command_in);
  Serial.print(":");
  Serial.println(command_value);

  parsecommand(command_in, command_value);

}

void requestEvent(int numbytes) {


}

//11 change angle of stepper 1
//12 change angle of stepper 2
void parsecommand(int command_in, float command_value) {
  switch (command_in) {
    case 11:
      break;


    case 12:
      break;


  }


}
