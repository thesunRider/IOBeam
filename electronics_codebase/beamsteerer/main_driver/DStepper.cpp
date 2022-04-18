
#include <Arduino.h>
#include "DStepper.h"

DStepper::DStepper(int mode, int m_B, int m_P, int m_Y, int m_O, int POT) {


	motor_B = m_B;
	motor_P = m_P;
	motor_Y = m_Y;
	motor_O = m_O;

	motor_mode = mode;
	motor_pot = POT;


	pinMode(motor_B, OUTPUT);
	pinMode(motor_P, OUTPUT);
	pinMode(motor_Y, OUTPUT);
	pinMode(motor_O, OUTPUT);

	pinMode(motor_pot, INPUT);

	Serial.println("INIT  STEPPER");
}

int DStepper::readPotpos(int sample_number,int delay_val){
	int mean = 0;

	for (int i = 0; i < sample_number ; i++){
      mean += analogRead(motor_pot)/sample_number;
      delay(delay_val);
    }
	return analogRead(motor_pot);
}

void DStepper::setcurrentPos(int pos){
	current_pos = pos;
}

int DStepper::currentPos(){
	return current_pos;
}

void DStepper::homePot(int pot_mean,int margin){
	int cur_potpos = readPotpos(10,10);

	while (!(cur_potpos > pot_mean - margin && cur_potpos < pot_mean + margin)){
		if (cur_potpos > pot_mean - margin )
			single_step(true);

		if (cur_potpos < pot_mean - margin )
			single_step(false);

		delay(100);
	cur_potpos = readPotpos(10,10);
	Serial.println(cur_potpos);
	}

}

void DStepper::goto_angle(float angle){
	float dest_stp = 0;


	if(motor_mode == 0)
		dest_stp = angle / (0.1764*4);

	if(motor_mode == 1)
		dest_stp = angle / (0.176*4);

	if(motor_mode == 2)
		dest_stp = angle / (0.0879*8);

	Serial.print("Dest pos:");
	Serial.println(dest_stp);

	if (dest_stp - current_pos > 0){

			for (int i = current_pos; i <= dest_stp ; i++){
				single_step(true);
				Serial.println(current_pos);
				delay(50);
			}
		}
	else{

			for (int i = current_pos; i >= dest_stp ; i--){
				single_step(false);
				Serial.println(current_pos);

				delay(100);
			}
}
	

	

}

void DStepper::single_step( bool dir) {
	int pattern_length = 0;
	int motor_1;
	int motor_2;
	int motor_3;
	int motor_4;

	if (dir) {
		motor_1 = motor_B;
		motor_2 = motor_P;
		motor_3 = motor_Y;
		motor_4 = motor_O;
	} else {

		motor_1 = motor_O;
		motor_2 = motor_Y;
		motor_3 = motor_P;
		motor_4 = motor_B;

	}

	if (motor_mode == 2) {

		for (int i = 0; i < 8; i++) {
			digitalWrite(motor_1, bitRead(halfdrive_pattern[i], 0));
			digitalWrite(motor_2, bitRead(halfdrive_pattern[i], 1));
			digitalWrite(motor_3, bitRead(halfdrive_pattern[i], 2));
			digitalWrite(motor_4, bitRead(halfdrive_pattern[i], 3));
			delayMicroseconds(1000);
		}


		dir ? current_pos += 0.5 : current_pos -= 0.5;
	}
	else  if (motor_mode == 1) {

		for (int i = 0; i < 4; i++) {
			digitalWrite(motor_1, bitRead(fulldrive_pattern[i], 0));
			digitalWrite(motor_2, bitRead(fulldrive_pattern[i], 1));
			digitalWrite(motor_3, bitRead(fulldrive_pattern[i], 2));
			digitalWrite(motor_4, bitRead(fulldrive_pattern[i], 3));
			delayMicroseconds(2000);
		}


		dir ? current_pos += 1 : current_pos -= 1;
	}
	else  if (motor_mode == 0) {

		for (int i = 0; i < 4; i++) {
			digitalWrite(motor_1, bitRead(wavedrive_pattern[i], 0));
			digitalWrite(motor_2, bitRead(wavedrive_pattern[i], 1));
			digitalWrite(motor_3, bitRead(wavedrive_pattern[i], 2));
			digitalWrite(motor_4, bitRead(wavedrive_pattern[i], 3));
			delayMicroseconds(3500);
		}

		dir ? current_pos += 1 : current_pos -= 1;
	}


}
