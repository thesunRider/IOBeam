#ifndef DStepper_header
#define DStepper_header


#include<Arduino.h>

class DStepper {

  private:

    float current_pos;
    int motor_B;
    int motor_P;
    int motor_Y;
    int motor_O;
    int motor_mode;
    int motor_pot;
    long motorDelay = 35000L;

    //mode 2
    byte halfdrive_pattern[8] = {
      0b00001000,
      0b00001100,
      0b00000100,
      0b00000110,
      0b00000010,
      0b00000011,
      0b00000001,
      0b00001001,
    };

    //mode 1
    byte fulldrive_pattern[4] = {
      0b00001100,
      0b00000110,
      0b00000011,
      0b00001001,
    };

    //mode 0 
    //wave drive sequence
    byte wavedrive_pattern[4] = {
      0b00001000,
      0b00000100,
      0b00000010,
      0b00001001,
    };


  public:
  	void changeSpeed(int speed);
  	void goto_angle(float angle);
  	int currentPos();
  	void setcurrentPos(int pos);
  	int readPotpos(int sample_number,int delay_val);
  	void homePot(int pot_mean,int margin);
    void single_step(bool dir); //true forward, false back
    DStepper(int motor_mode, int m_B, int m_P, int m_Y, int m_O, int POT);

  };

 #endif
