
int stepPin  = 10;                  // Stepstick STEP pin to Arduino digital pin 5
int dirPin   = 11;                  // Stepstick DIR pin to Arduino digital  pin 6

int numSteps = 708;                 //720/5.625 degree = 128/2 = 64 //int numSteps = 48;Number of steps in 360 degree rotation
int rotations = 1;                  // Number of rotations of the rotor for each

int delay1  =   40;                  // Delay between coil activations (ms)
int delay2  =   2000;               // Delay between subsequent rotations
int i;                              // int to use in for loop

//------------------------------
void setup() {

  pinMode(stepPin,         OUTPUT); // stepPin set to receive Arduino signals
  pinMode(dirPin,          OUTPUT); // DIR set to receive Arduino signals
  
}

//------------------------------

void loop() {
  
  digitalWrite(dirPin, HIGH);

  for (i = 1; i <= (numSteps * 1 * rotations); ++i) {
    digitalWrite(stepPin, LOW);    // Prepare to take a step
    digitalWrite(stepPin, HIGH) ;  // Take a step
    delay(delay1);                 // Allow some delay between energizing
                                   // the coils to allow stepper rotor time to respond.
  }
  delay(delay2);

  digitalWrite(dirPin, LOW);

  for (i = 1; i <= (numSteps * 1 * rotations); ++i) {
    digitalWrite(stepPin, LOW);    // Prepare to take a step
    digitalWrite(stepPin, HIGH) ;  // Take a step
    delay(delay1);                 // Allow some delay between energizing
                                   // the coils to allow stepper rotor time to respond.
  }
  delay(delay2);

}
