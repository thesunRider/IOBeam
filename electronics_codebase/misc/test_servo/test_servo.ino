/* Example sketch to control a 28BYJ-48 stepper motor with ULN2003 driver board, AccelStepper and Arduino UNO: continuous rotation. More info: https://www.makerguides.com */

// Include the AccelStepper library:
#include <AccelStepper.h>

// Define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 8


#define motor2_B 2
#define motor2_P 3
#define motor2_Y 4
#define motor2_O 5

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:

AccelStepper stepper = AccelStepper(MotorInterfaceType, motor2_O, motor2_Y, motor2_P, motor2_B);

void setup() {
  // Set the maximum steps per second:
  stepper.setMaxSpeed(3000);
  
  // Set the speed of the motor in steps per second:
  stepper.setSpeed(1200);
}

void loop() {
  // Step the motor with constant speed as set by setSpeed():
  stepper.runSpeed();
}
