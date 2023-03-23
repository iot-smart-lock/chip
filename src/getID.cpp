#include <Arduino.h>

uint64_t chipId = 0;

void setupID() {
  Serial.begin(9600);
  delay(500);
}

void loopID() {
  chipId=ESP.getEfuseMac();
  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.printf("ESP32ChipID=%04X",(uint16_t)(chipId>>32));//print High 2bytes
  Serial.printf("%08X\r\n",(uint32_t)chipId);//print Low 4bytes.
  delay(3000);
}
// CHIP: ESP32-D0WDQ6 Rev 1
// ID: 085003BF713C