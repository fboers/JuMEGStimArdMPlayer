#include <DirectIO.h>

InputPort<PORT_L> port;

void setup() {
Serial.begin(115200);
 
}

uint8_t keys_old=0;
uint8_t keys=0;
uint8_t mask=15;

void loop() {
  u8 i = 0;
  
  while(1) {
    //port = i++;
    keys = port & mask;
    if ( keys != keys_old)
      { 
       Serial.print("--> R ");
       Serial.print(keys);
       Serial.print(",");
   
       Serial.println( keys_old );
      keys_old = keys;
      }
  }
}

