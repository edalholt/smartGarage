#include "HomeSpan.h"
#include "GarageDoor.h"

void setup() {
  Serial.begin(115200);

  homeSpan.setPairingCode("12322123");

  homeSpan.begin(Category::Bridges, "Smart Garage Bridge");

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Name("Garage door");
  new GarageDoor(2, 14);
}

void loop() {
  homeSpan.poll();
}
