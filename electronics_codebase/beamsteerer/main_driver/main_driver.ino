#include <Wire.h>
#include "DStepper.h"

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


#define motor1_B 8
#define motor1_P 9
#define motor1_Y 10
#define motor1_O 11

#define motor_POT1 A0
#define motor_POT2 A1

#define IO0 12
#define IO2 13
#define chopper_output 8

bool frequency_state_loop = false;
int frequency_chopper = 1;
int stepper1_potmean = 511;
int stepper2_potmean = 512;
int current_mills = 0;

DStepper stepper1 = DStepper(1, motor1_B, motor1_P, motor1_Y, motor1_O, A0);
DStepper stepper2 = DStepper(1 , motor2_B, motor2_P, motor2_Y, motor2_O, A1);


void setup() {
  // put your setup code here, to run once:
  Wire.begin(SLAVE_DRIVER);

  Serial.begin(9600);
  Wire.onReceive(receiveEvent); // when recieving data
  Wire.onRequest(requestEvent);    // when sending data

  pinMode(IO0, OUTPUT);
  pinMode(IO2, OUTPUT);
  pinMode(chopper_output, OUTPUT);


  digitalWrite(IO0, LOW);
  digitalWrite(IO2, HIGH);
  digitalWrite(chopper_output, LOW);



  //stepper1.homePot(stepper1_potmean, 2);
  //stepper2.homePot(stepper1_potmean, 2);

  stepper1.setcurrentPos(0);
  stepper2.setcurrentPos(0);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (frequency_state_loop) {
    if (frequency_chopper < 31) {
      if (millis() - current_mills >  ((1 / frequency_chopper) * 1000)) {
        digitalWrite(chopper_output, !digitalRead(chopper_output));
        current_mills = millis();  //update current time
      }
    }
  }
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
  Serial.println(command_value, 4);

  parsecommand(command_in, command_value);

}

void requestEvent() {


}

//11 change angle of stepper 1
//12 change angle of stepper 2
//31 change frequency of chopper
//32 turn off chopper
void parsecommand(int command_in, float command_value) {
  switch (command_in) {
    case 11:
      stepper1.goto_angle(command_value - 45 );
      Serial.print(">READ POT1:");
      Serial.println(analogRead(motor_POT1));
      break;


    case 12:
      stepper2.goto_angle(command_value - 45);
      Serial.print(">READ POT2:");
      Serial.println(analogRead(motor_POT2));
      break;

    case 14:
      Serial.println(">Stepper reset");
      stepper1.setcurrentPos(0);
      stepper2.setcurrentPos(0);
      break;

    case 31:
      Serial.println(">Change frequency stepper");
      if (command_value > 31) {
        frequency_state_loop = false;
        noTone(chopper_output);
        tone(chopper_output, (int) command_value);

      } else {
        frequency_state_loop = true;
        frequency_chopper = (int) command_value;
      }
      break;

    case 32:
      noTone(chopper_output);
      frequency_state_loop = false;
      break;

  }
}
