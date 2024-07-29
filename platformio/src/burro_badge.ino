#include <Arduino.h>

#include "burro_mesh.h"
#include "burro_leds.h"
#include "burro_server.h"
#include "burro_mem.h"


void setup() {  
  delay(1000);
  //Serial.begin(115200);
  initEEPROM();
  loadFlags();
  initLEDs();
  initMesh();
  initServer();
}

void loop() {
  uint32_t currentMeshTime = updateMesh();
  updateLEDs(currentMeshTime / 1000);
  updateDNS();
}