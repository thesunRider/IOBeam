#include "SerialTransfer.h"


SerialTransfer transferdata;

struct STRUCT {
  char payload;
  int response_flag;
} response;

struct STRUCT {
  String command_value;  
  int command_flag;
} request;

void setup() {
  Serial.begin(9600);
  transferdata.begin(Serial);

}

void loop() {
  if(myTransfer.available())
  {
    // use this variable to keep track of how many
    // bytes we've processed from the receive buffer
    uint16_t recSize = 0;

    recSize = transferdata.rxObj(request, recSize);
    switch (request.command_flag){
      
      }
    
    
  }

  
}
