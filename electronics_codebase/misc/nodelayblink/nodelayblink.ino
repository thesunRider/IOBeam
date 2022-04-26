const int ledPin =  6;      // the number of the LED pin
byte level = 0;
unsigned long currentTime;  // could use micros also for more precise timing
unsigned long previousTime;
unsigned long elapsedTime;
unsigned long halfPeriod = 30;

void setup() {
  // set the digital pin as output: = 
  pinMode(ledPin, OUTPUT);      
}

void loop()
{    currentTime = millis(); // capture the current time
    elapsedTime = currentTime - previousTime; // how much time elapsed since last pass thru loop
    if (elapsedTime >= halfPeriod) { // time to change levels?
      previousTime = previousTime + halfPeriod ;// set up for next level change
      level = 1 - level; // will change 1,0,1,0,1,0 ...
      digitalWrite (ledPin, level);
    
  }
}
