#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Wire.begin(4);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent); // when recieving data
  Wire.onRequest(sendEvent);    // when sending data
}

void loop() {
  // put your main code here, to run repeatedly:

}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int numbytes)
{
  while (Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

void sendEvent(int numbytes) {


}
