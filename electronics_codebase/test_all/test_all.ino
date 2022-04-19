/*
 * Example using the Rotary library, dumping integers to the serial
 * port. The integers increment or decrement depending on the direction
 * of rotation.
 *
 * This example uses polling rather than interrupts.
 */

#include <rotary.h>
#include <LiquidCrystal.h>

//initialize pin

#define PINA2 A0
#define PINB2 A1
#define PUSHB2 A3

const int rs = 11, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2, back = 9;
bool  initial = LOW;

//initialize prog
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Rotary r = Rotary(PINA2, PINB2);      

// Counter that will be incremented or decremented by rotation.
int counter = 0;

void setup() {
    lcd.begin (16, 2);
    lcd.print("YOOOOOOO");
}

void loop() {
  unsigned char result = r.process();
  if (result == DIR_CW) {
    counter++;
  lcd.clear();
  lcd.print (String(counter));
  } else if (result == DIR_CCW) {
    counter--;
    lcd.clear();
  lcd.print (String(counter));
  
  }
}
