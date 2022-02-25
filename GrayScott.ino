#include "menu.h"
#include "display.h"
#include "simulation.h"

void setup() {
  //while(!Serial);
  delay(1000);
  Serial.begin(115200);

  randomSeed(analogRead(0));

  initMenu();
  initDisplay();
  initSimulation();
}

void loop() {
  unsigned long now = millis();

  if (!loopMenu(now)) {
    loopSimulation(now);
  }
}
