/*
  Created by Suryasaradhi,Alex Shajan

*/

//IMPORTS-------------------------------
#include <LiquidCrystal.h>
#include <rotary.h>                 // rotary handler
#include <Wire.h>                   // i2c handler
#include <EEPROM.h>                 // memory handler

//ENDIMPORTS-------------------------------------------

//defines ---------------------------------------------

//initialize pin
#define PINA A0
#define PINB A1
#define PUSHB A2
#define PINA2 6
#define PINB2 7
#define PUSHB2 A3

#define SLAVE_DRIVER 4


void about();
void login();
int getinput(int rotary_step);
void showLetters(int printStart, int startLetter, char*  messagePadded);

const int rs = 11, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2, back = 9;
bool  initial = LOW;
int scroll_letter = 0;


//initialize prog
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Rotary r = Rotary(PINA, PINB, PUSHB);        // there is no must for using interrupt pins !!
Rotary s = Rotary(PINA2, PINB2, PUSHB2);

int columnsLCD = 16;
char *MenuLine[] = {" Home", " Network Settings", " Hardware Settings", "Record"," About"};
int MenuItems = 5;
int CursorLine = 0;


char id[5];
char pin[4];
const unsigned long period = 10000;  //the value is a number of milliseconds

unsigned long start_millis;
unsigned long saved_millis;


//minimum step for ui of rotary encoder
int rotary_minstep = 4;


//counter for the rotary encoder
int counter_rotary1 = 0;
int counter_rotary2 = 0;



//settings
bool connection_status = true;
int angle_unit = 0; //0 - deg , 1 - rad
float angle1 = 45.0000, angle2 = 45.0000;
float initial_select = 0;
int angle_multiplier = 1;

byte wifi_connected_icon[] = {
  B00000,
  B01110,
  B10001,
  B00100,
  B01010,
  B00000,
  B00100,
  B00000
};

//enddefines-----------------------------------------------

void setup ()
{


  //rotary setups
  pinMode(PINA, INPUT_PULLUP); //RotaryCLK
  pinMode(PINB, INPUT_PULLUP); //RotaryDT
  pinMode(PUSHB, INPUT_PULLUP); //Button

  pinMode(PINA2, INPUT_PULLUP); //RotaryCLK
  pinMode(PINB2, INPUT_PULLUP); //RotaryDT
  pinMode(PUSHB2, INPUT_PULLUP); //Button


  digitalWrite (PINA, HIGH);     // enable pull-ups
  digitalWrite (PINB, HIGH);
  digitalWrite (PUSHB, HIGH);

  digitalWrite (PINA2, HIGH);     // enable pull-ups
  digitalWrite (PINB2, HIGH);
  digitalWrite (PUSHB2, HIGH);

  pinMode(back, INPUT);

  lcd.begin (16, 2);
  lcd.clear (); // go home
  lcd.print ("IOBeam v1.5");
  lcd.setCursor(0, 1);
  lcd.print("Beam Steeering");


  lcd.createChar(0, wifi_connected_icon);

  delay(2000);
  print_menu();


  Serial.begin(9600);
  Wire.begin();
  start_millis = millis();
  saved_millis = start_millis;
}


void loop ()
{

  int inp = getinput(rotary_minstep);
  //Serial.println(inp);
  switch (inp) {

    case 0x32:
      scroll_letter = 0;
      CursorLine -= 1;
      break;

    case 0x34:
      CursorLine += 1;
      scroll_letter = 0;
      break;

    case 0x35:
      lcd.setCursor(0, 1);  //(col, row)
      Serial.println("butnclick");
      selection();
      break;

    case 0x42:
      break;

    case 0x44:
      break;


  }

  if (inp == 0x32 || inp == 0x34 ) {
    if (CursorLine < 0) {
      CursorLine = MenuItems - 1;
    }
    if (CursorLine > MenuItems - 1) {
      CursorLine = 0;                 // roll over to first item
    }
    print_menu();
  }

  //update every 400ms
  if (millis() - saved_millis  >= 700 ) {
    saved_millis = millis();
    if ((int)strlen(MenuLine[CursorLine]) - 16 > 0) {

      if (scroll_letter - (int)strlen(MenuLine[CursorLine]) >= 0 )
        scroll_letter = 0;


      //Serial.println(strlen(MenuLine[CursorLine]));
      scroll_letter += 1;
      showLetters(1, scroll_letter, MenuLine[CursorLine]);


    }
    else {
      scroll_letter = 0;
    }
  }

} //End loop()

/************FUNCTIONS**************/

void print_menu()
{
  lcd.clear();
  lcd.setCursor(0, 0);     //(col, row)
  lcd.print("   Main Menu  ");
  lcd.setCursor(0, 1);    //2nd row
  lcd.print("<"); lcd.setCursor(columnsLCD - 1, 1); lcd.print(">");
  lcd.setCursor(1, 1);
  lcd.print(MenuLine[CursorLine]);
}

void selection()
{
  Serial.println(CursorLine);
  switch (CursorLine) {
    case 0:
      lcd.print("   Home Page");
      homepage();
      break;
    case 1:
      lcd.print("   Login ");
      login();
      break;
    case 2:
      lcd.print("   Hardware Settings");
      hardware_settings();
      break;
    case 4:
      about();
      break;

    case 3:
      record();
      break;
      

    default:
      break;
  } //end switch

  CursorLine = 0;     // reset to start position
} //End selection

void record(){
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">a1:");
  lcd.print(angle1, 4);
  angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");

  lcd.setCursor(0, 1);
  lcd.print(" a2:");
  lcd.print(angle2, 4);
  angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");

  boolean flag = true;
  int angle_select = 0;

  lcd.setCursor(15 - String(angle_multiplier).length(), 1);
  lcd.print(String(angle_multiplier) + "x");

  if (angle_unit == 0 ) {
    lcd.setCursor(15, 0);
    lcd.print("d");
  } else {
    lcd.setCursor(15, 0);
    lcd.print("r");
  }


  while (flag)
  {
    int inp = getinput(rotary_minstep);
    switch (inp) {
      case 0x55:
        Serial.println("going back");
        print_menu();
        return;

      case 0x32:
        if (angle_select == 0) {
          angle1 -= 0.1000 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 -= 0.1000 * angle_multiplier;
        }

        break;

      case 0x34:
        if (angle_select == 0) {
          angle1 += 0.1000 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 += 0.1000 * angle_multiplier;
        }

        break;

      case 0x42:
        if (angle_select == 0) {
          angle1 -= 0.0001 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 -= 0.0001 * angle_multiplier;
        }

        break;

      case 0x44:
        if (angle_select == 0) {
          angle1 += 0.0001 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 += 0.0001 * angle_multiplier;
        }

        break;

      //if button pressed tab through the options
      case 0x35:
        angle_select += 1;
        if (angle_select >= 5)
          angle_select = 0;
        break;

    }

    if (inp == 0x32 || inp == 0x34 || inp == 0x42 || inp == 0x44) {

      if (angle_select == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");

        if (angle2 < 0)
          angle2 = 90;

        if (angle2 > 90)
          angle2 = 0;


        stepper_move(angle2, 1);

      } else if (angle_select == 5){
        lcd.setCursor(14, 0);
        lcd.print("8");

      } else if (angle_select == 0) {
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(0, 1);
        lcd.print(" ");

        if (angle1 < 0)
          angle1 = 90;

        if (angle1 > 90)
          angle1 = 0;

        stepper_move(angle1, 0);

      }

      if (angle_select == 2) { //change multiplier
        angle_multiplier *= 10;

        if (angle_multiplier == 100)
          angle_multiplier = 50;
        else if (angle_multiplier > 100)
          angle_multiplier = 1;

        lcd.setCursor(12, 1);
        lcd.print("   "); //clear the screen buffer of 100x

        lcd.setCursor(15 - String(angle_multiplier).length(), 1);
        lcd.print(String(angle_multiplier) + "x");
        lcd.cursor();

      } else if (angle_select == 3) { //change unit
        lcd.setCursor(15, 0);
        ++angle_unit;
        if (angle_unit > 1)
          angle_unit = 0;

      }

      lcd.setCursor(15, 0);
      angle_unit == 0 ? lcd.print("d") : lcd.print("r");


      lcd.setCursor(1, 0);
      lcd.print("a1:");
      lcd.print(angle1, 4);
      angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");
      lcd.print(" ");

      lcd.setCursor(1, 1);
      lcd.print("a2:");
      lcd.print(angle2, 4);
      angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");
      lcd.print(" ");


    }

    if ( inp == 0x35) {
      if (angle_select == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");
      } else if (angle_select == 0) {
        lcd.noCursor();
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(0, 1);
        lcd.print(" ");

      } else if (angle_select == 2) { //change multiplier
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.setCursor(15 - String(angle_multiplier).length(), 1);
        lcd.cursor();

      } else if (angle_select == 3) { //change unit
        lcd.setCursor(15, 0);
      }
    }

    if (angle_select == 2) { //change multiplier
      lcd.setCursor(15 - String(angle_multiplier).length(), 1);
      lcd.cursor();

    } else if (angle_select == 3) { //change unit
      lcd.setCursor(15, 0);
    } else if (angle_select == 5){
        lcd.setCursor(14, 0);

    }
  }
}

void hardware_settings() {
  lcd.clear();
  int cur_sor;

  lcd.setCursor(0, 0);
  lcd.print(">Current Homing");

  while (1) {

    int inp = getinput(rotary_minstep);
    //Serial.println(inp);
    switch (inp) {
      case 0x55:
        Serial.println("going back");
        print_menu();
        return;
        
      case 0x32:
        cur_sor -= 1;
        if (cur_sor < 0)
          cur_sor = 0;
          
        break;

      case 0x34:
        cur_sor += 1;
        if (cur_sor > 4)
          cur_sor = 4;
          
        break;

      case 0x35:
        Serial.println("Stepper reset");
        if(cur_sor == 0)
          stepper_move(0,4); //set as home all stepper
        break;
    }

    delay(60);
  }

}

void homepage()
{

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">a1:");
  lcd.print(angle1, 4);
  angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");

  lcd.setCursor(0, 1);
  lcd.print(" a2:");
  lcd.print(angle2, 4);
  angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");

  boolean flag = true;
  int angle_select = 0;

  lcd.setCursor(15 - String(angle_multiplier).length(), 1);
  lcd.print(String(angle_multiplier) + "x");

  if (connection_status ) {
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
  }

  if (angle_unit == 0 ) {
    lcd.setCursor(14, 0);
    lcd.print("d");
  } else {
    lcd.setCursor(14, 0);
    lcd.print("r");
  }


  while (flag)
  {
    int inp = getinput(rotary_minstep);
    switch (inp) {
      case 0x55:
        Serial.println("going back");
        print_menu();
        return;

      case 0x32:
        if (angle_select == 0) {
          angle1 -= 0.1000 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 -= 0.1000 * angle_multiplier;
        }

        break;

      case 0x34:
        if (angle_select == 0) {
          angle1 += 0.1000 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 += 0.1000 * angle_multiplier;
        }

        break;

      case 0x42:
        if (angle_select == 0) {
          angle1 -= 0.0001 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 -= 0.0001 * angle_multiplier;
        }

        break;

      case 0x44:
        if (angle_select == 0) {
          angle1 += 0.0001 * angle_multiplier;
        }
        else if (angle_select == 1) {
          angle2 += 0.0001 * angle_multiplier;
        }

        break;

      //if button pressed tab through the options
      case 0x35:
        angle_select += 1;
        if (angle_select >= 4)
          angle_select = 0;
        break;

    }

    if (inp == 0x32 || inp == 0x34 || inp == 0x42 || inp == 0x44) {

      if (angle_select == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");

        if (angle2 < 0)
          angle2 = 90;

        if (angle2 > 90)
          angle2 = 0;


        stepper_move(angle2, 1);

      } else if (angle_select == 0) {
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(0, 1);
        lcd.print(" ");

        if (angle1 < 0)
          angle1 = 90;

        if (angle1 > 90)
          angle1 = 0;

        stepper_move(angle1, 0);

      }
      if (connection_status ) {
        lcd.setCursor(15, 0);
        lcd.write(byte(0));

      }

      if (angle_select == 2) { //change multiplier
        angle_multiplier *= 10;

        if (angle_multiplier == 100)
          angle_multiplier = 50;
        else if (angle_multiplier > 100)
          angle_multiplier = 1;

        lcd.setCursor(12, 1);
        lcd.print("   "); //clear the screen buffer of 100x

        lcd.setCursor(15 - String(angle_multiplier).length(), 1);
        lcd.print(String(angle_multiplier) + "x");
        lcd.cursor();

      } else if (angle_select == 3) { //change unit
        lcd.setCursor(14, 0);
        ++angle_unit;
        if (angle_unit > 1)
          angle_unit = 0;

      }

      lcd.setCursor(14, 0);
      angle_unit == 0 ? lcd.print("d") : lcd.print("r");


      lcd.setCursor(1, 0);
      lcd.print("a1:");
      lcd.print(angle1, 4);
      angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");
      lcd.print(" ");

      lcd.setCursor(1, 1);
      lcd.print("a2:");
      lcd.print(angle2, 4);
      angle_unit == 0 ? lcd.print((char)223) : lcd.print("r");
      lcd.print(" ");


    }

    if ( inp == 0x35) {
      if (angle_select == 1) {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(">");
      } else if (angle_select == 0) {
        lcd.noCursor();
        lcd.setCursor(0, 0);
        lcd.print(">");
        lcd.setCursor(0, 1);
        lcd.print(" ");

      } else if (angle_select == 2) { //change multiplier
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.setCursor(15 - String(angle_multiplier).length(), 1);
        lcd.cursor();

      } else if (angle_select == 3) { //change unit
        lcd.setCursor(14, 0);
      }
    }

    if (angle_select == 2) { //change multiplier
      lcd.setCursor(15 - String(angle_multiplier).length(), 1);
      lcd.cursor();

    } else if (angle_select == 3) { //change unit
      lcd.setCursor(14, 0);
    }

  }



}

void about()
{
  lcd.clear();
  lcd.print("Designed By: ");
  delay(1000);
  lcd.clear();
  lcd.print("Suryasaradhi");
  lcd.setCursor(0, 1);
  lcd.print("B180976EP");
  delay(2000);
  lcd.clear();
  lcd.print("Alex Shajan");
  lcd.setCursor(0, 1);
  lcd.print("B180649EP");
  delay(2000);
  lcd.clear();
  lcd.print("Project guide:");
  lcd.setCursor(0, 1);
  lcd.print("Dr.SubramanyanNV");
  delay(2000);
  lcd.clear();
  lcd.print("IOBeam v1.5");
  lcd.setCursor(0, 1);
  lcd.print("Beam Steering");
  delay(2000);
  print_menu();
}

void login()
{
  lcd.clear();
  lcd.print("Enter User ID:");
  lcd.setCursor(0, 1);
  UserId();
  lcd.clear();
  lcd.print("Enter password:");
  password();
}


// 0x31 - movement in rotary 1
// 0x32 - ccw minstep rotary 1
// 0x34 - acw minstep rotary 1
// 0x35 - butn pressd rotary 1
// 0x41 - movement in rotary 2
// 0x42 - ccw min step rotary 2
// 0x44 - acw minstep rotary 2
// 0x45 - butn pressd rotary 2
// 0x00 - none
int getinput(int rotary_step) {
  volatile unsigned char result = r.process();
  volatile unsigned char result2 = s.process();

  if (result) {
    result == DIR_CCW ? counter_rotary1++ : counter_rotary1--;
    if (counter_rotary1 >= rotary_step) {
      counter_rotary1 = 0;
      return 0x32;
    }
    if ( counter_rotary1 <= -1 * rotary_step ) {
      counter_rotary1 = 0;
      return 0x34;
    }
    return 0x31;
  }

  if (result2) {
    result2 == DIR_CCW ? counter_rotary2++ : counter_rotary2--;
    if (counter_rotary2 >= rotary_step) {
      counter_rotary2 = 0;
      return 0x42;
    }
    if ( counter_rotary2 <= -1 * rotary_step ) {
      counter_rotary2 = 0;
      return 0x44;
    }

    return 0x41;
  }

  if (r.buttonPressedReleased(25)) {
    return 0x35;
  }
  if (s.buttonPressedReleased(25)) {
    return 0x45;
  }

  if (buttonpressed(back)) {
    return 0x55;
  }

  return 0;

}

bool buttonpressed(int buton_indx)
{
  if (digitalRead(buton_indx) != initial)
  {
    initial = digitalRead(buton_indx);
    if (digitalRead(buton_indx) == HIGH)
      return true;
  }
  return false;
}


void showLetters(int printStart, int startLetter, char*  messagePadded)
{
  lcd.setCursor(printStart, 1);
  for (int letter = startLetter; letter <= (int) strlen(messagePadded) - 1; letter++) // Print only 16 chars in Line #2 starting 'startLetter'
  {
    lcd.print(messagePadded[letter]);
  }
  lcd.print("                    ");
  lcd.setCursor(columnsLCD - 1, 1);
  lcd.print(">");
}

void UserId()
{

}

void password()
{

}

void sendwire(char *str) {
  Serial.println();
  Serial.print("sent from main>> ");
  Serial.print(str);
  Serial.println();

  Wire.beginTransmission(SLAVE_DRIVER); // transmit to device #4
  Wire.write(str);
  Wire.endTransmission();
}


void stepper_move(float stepper_angle, int stepper_index) {

  char sendstring[11];

  switch (stepper_index) {
    case 0x0:
      dtostrf(stepper_angle, 10, 4, sendstring);
      sendstring[0] = '1';
      sendstring[1] = '1';
      sendstring[2] = 'x';
      sendwire(sendstring);
      break;

    case 0x1:
      dtostrf(stepper_angle, 10, 4, sendstring);
      sendstring[0] = '1';
      sendstring[1] = '2';
      sendstring[2] = 'x';
      sendwire(sendstring);
      break;

    case 0x3: //home the steppers

      break;

    case 0x4:
      dtostrf(stepper_angle, 10, 4, sendstring);
      sendstring[0] = '1';
      sendstring[1] = '4';
      sendstring[2] = 'x';
      sendwire(sendstring);
      break;


  }

}
