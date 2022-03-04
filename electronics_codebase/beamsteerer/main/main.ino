/*
 * Created by Suryasaradhi,Alex Shajan
 * 
 */

#include <LiquidCrystal.h>
#include <rotary.h>                 // rotary handler


//initialize pins
const int rs = 11, en = 12, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int back = 9,PINA = A0,PINB = A1,PUSHB = A2;
const int PINA2 = 6,PINB2 = 7,PUSHB2 = A3;
bool  initial = LOW;



//initialize prog
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Rotary r = Rotary(PINA, PINB, PUSHB);        // there is no must for using interrupt pins !!
Rotary s = Rotary(PINA2, PINB2, PUSHB2);
 
int columnsLCD = 16;
char* MenuLine[] = {" Home", " Change User", " Option 3", " Option 4", " Option 5", " Option 6", " About"};
int MenuItems = 7;
int CursorLine = 0;

char id[5];
char pin[4];
 
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;  //the value is a number of milliseconds



void setup ()
{
  
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
 
  pinMode(backlightPin, OUTPUT);          //backlightPin as an output
  digitalWrite(backlightPin, HIGH);
 
  lcd.begin (16, 2);
  lcd.clear (); // go home
  lcd.print ("Menu Master");
  lcd.setCursor(0, 1);
  lcd.print("Please Select");
  
  delay(2000);
  startMillis = millis();  //initial start time
  print_menu();

  
  Serial.begin(9600);
  Wire.begin();
}


void loop ()
{
  volatile unsigned char result = r.process();
 
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (result) {
    startMillis = millis();;  //wake up LCD...
    result == DIR_CCW ? CursorLine = CursorLine - 1 : CursorLine = CursorLine + 1;
    if (CursorLine < 0) { CursorLine = MenuItems - 1;} 
    if (CursorLine > MenuItems - 1) {
      CursorLine = 0;                 // roll over to first item
    }
    print_menu();
  } //End if result
 
  if (r.buttonPressedReleased(25)) {
    startMillis = millis();;     //wake up LCD...
    lcd.setCursor(0, 0);  //(col, row)
    lcd.print("You selected:");
    lcd.setCursor(0, 1);  //(col, row)
    selection();
   // print_menu();
  } //endif buttonPressedReleased
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
      lcd.print("Option 3    ");
      //set a flag or do something....
      break;
    case 3:
      lcd.print("Option 4    ");
      //set a flag or do something....
      break;
    case 4:
      lcd.print("Option 5    ");
      //set a flag or do something....
      break;
    case 5:
      lcd.print("Option 6    ");
      //set a flag or do something....
      break;
    case 6:
      lcd.print("Option 7    ");
      about();
      break;
    default:
      break;
  } //end switch
 
  delay(2000);
  CursorLine = 0;     // reset to start position
} //End selection

void homepage()
{
  float angle1 = 0, angle2 = 0;
  float initial_select = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">angle 1: " + String(angle1 ));
  lcd.setCursor(0,1);
  lcd.print(" angle 2: " + String(angle2));
  boolean flag=true;
  while (flag)
  {
    volatile unsigned char result = r.process();
    volatile unsigned char fine = s.process();

if (result)
{
    startMillis = millis();;  //wake up LCD...
    if (initial_select == 0)
    {
       result == DIR_CCW ? angle1 =angle1-.5 : angle1 = angle1+.5;
    }
    else
    {
       result == DIR_CCW ? angle2 =angle2-.5 : angle2 = angle2+.5;
    }

    if(angle1<0)
    angle1=angle1+360;
    if(angle1>360)
    angle1=angle1-360;
    if(angle2<0)
    angle1=angle1+360;
    if(angle2>360)
    angle1=angle1-360;
  
  lcd.clear();
  if (initial_select == 0)
  {
  lcd.setCursor(0,0);
  lcd.print(">Angle 1: " + String(angle1 ));
  lcd.setCursor(0,1);
  lcd.print(" Angle 2: " + String(angle2)); 
  }
  
  else
  {
  lcd.setCursor(0,0);
  lcd.print(" Angle 1: " + String(angle1));
  lcd.setCursor(0,1);
  lcd.print(">Angle 2: " + String(angle2));
  }
  
}

if (fine)
{
    startMillis = millis();;
    if (initial_select == 0)
    {
       fine == DIR_CCW ? angle1 =angle1-0.001 : angle1 = angle1 +0.001;
    }
    else
    {
       fine == DIR_CCW ? angle2 =angle2-0.001 : angle2 = angle2 +0.001;
    }

    if(angle1<0)
    angle1=angle1+360;
    if(angle1>360)
    angle1=angle1-360;
    if(angle2<0)
    angle1=angle1+360;
    if(angle2>360)
    angle1=angle1-360;
  
  lcd.clear();
  if (initial_select == 0)
  {
  lcd.setCursor(0,0);
  lcd.print(">Angle 1: " + String(angle1 ));
  lcd.setCursor(0,1);
  lcd.print(" Angle 2: " + String(angle2)); 
  }
  
  else
  {
  lcd.setCursor(0,0);
  lcd.print(" Angle 1: " + String(angle1));
  lcd.setCursor(0,1);
  lcd.print(">Angle 2: " + String(angle2)); 
  }
  
}
 
  if (r.buttonPressedReleased(25)) 
  {
    startMillis = millis();;
    if (initial_select == 0)
    initial_select = 1;
    else
    initial_select = 0;

    lcd.clear();

    if (initial_select == 0)
  {
  lcd.setCursor(0,0);
  lcd.print(">Angle 1: " + String(angle1 ));
  lcd.setCursor(0,1);
  lcd.print(" Angle 2: " + String(angle2)); 
  }
  else
  {
  lcd.setCursor(0,0);
  lcd.print(" Angle 1: " + String(angle1));
  lcd.setCursor(0,1);
  lcd.print(">Angle 2: " + String(angle2));
  }
  }
  if(buttonpressed())
  break;
} 
}

void about()
{
  lcd.clear();
  lcd.print("Designed By: ");
  delay(1000);
  lcd.clear();
  lcd.print("Suryasaradhi");
  lcd.setCursor(0,1);
  lcd.print("B180976EP");
  delay(2000);
  lcd.clear();
  lcd.print("Alex Shajan");
  lcd.setCursor(0,1);
  lcd.print("B180649EP");
  delay(2000);
}

void login()
{
  lcd.clear();
  lcd.print("Enter User ID:");
  lcd.setCursor(0,1);
  UserId();
  lcd.clear();
  lcd.print("Enter password:");
  password();
}

bool buttonpressed()
{
  if (digitalRead(back) != initial)
  {
    initial = digitalRead(back);
    if (digitalRead(back)==HIGH)
      return true;
  }
  return false;
}

void UserId()
{
  
}

void password()
{
  
}
