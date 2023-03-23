#include <Arduino.h>
#include "getID.h"

#define GET_ID 1

void setup() {
  Serial.begin(9600);
  
  #if GET_ID
    setupID();
  #else
    // My code
  #endif

  delay(500);

}

void loop() {
  
  #if GET_ID
    loopID();
  #else
    Serial.println("Hello World!");
  #endif
  
  delay(900);
}