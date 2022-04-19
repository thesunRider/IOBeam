#include <Wire.h>
#include <AccelStepper.h>

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
#define mean_stepper1 512
#define mean_stepper2 512

#define MotorInterfaceType 8

void stepper_home(int stepper_id);
int getstepper_pos(int stepper_id,int sample_number,int delay_val);
 
// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper stepper1 = AccelStepper(MotorInterfaceType, motor1_O, motor1_Y, motor1_P, motor1_B);
AccelStepper stepper2 = AccelStepper(MotorInterfaceType, motor2_O, motor2_Y, motor2_P, motor2_B);

void setup() {
  // put your setup code here, to run once:
  Wire.begin(SLAVE_DRIVER);

  Serial.begin(9600);
  Wire.onReceive(receiveEvent); // when recieving data
  Wire.onRequest(requestEvent);    // when sending data

  stepper1.setMaxSpeed(0.1);
  stepper2.setMaxSpeed(0.1);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void stepper_home(int stepper_id){
  int motor_no;
  int mean_pos;
  AccelStepper stepper;

  stepper_id == 0? stepper = stepper1 : stepper = stepper2;
  stepper_id == 0? motor_no = motor_POT1: motor_no = motor_POT2;
  stepper_id == 0? mean_pos = mean_stepper1: mean_pos = mean_stepper2;

  int cur_pospot = 0,cur_pos = 0;
  int read_stepper = analogRead(motor_no);
  cur_pospot = getstepper_pos(motor_no,50,10);
  
  stepper_id == 0?stepper1.setCurrentPosition((long)0):stepper2.setCurrentPosition((long)0);
  Serial.print("Homing:");
  Serial.println(stepper_id);

  
  while(cur_pospot != mean_pos){
    cur_pospot < mean_pos? stepper.moveTo(++cur_pos):stepper.moveTo(--cur_pos);
    stepper.runToPosition();
    
    cur_pospot = getstepper_pos(motor_no,50,10);
    Serial.print("Stepper pos:");
    stepper.currentPosition();
    Serial.print("POT pos:");
    Serial.println(cur_pospot);
    delay(1000);
  }
  stepper.setCurrentPosition(0);
 }

 int getstepper_pos(int stepper_id,int sample_number,int delay_val){
    int mean = 0;
    int motor_no;
    stepper_id == 0? motor_no = motor_POT1: motor_no = motor_POT2;

    for (int i = 0; i < sample_number ; i++){
      mean += analogRead(motor_no)/sample_number;
      delay(delay_val);
    }

    return mean;
  
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
void parsecommand(int command_in, float command_value) {
  switch (command_in) {
    case 11:
      stepper_home(0);  
      break;


    case 12:
      stepper_home(1);  
      break;


  }


}
