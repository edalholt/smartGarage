#ifndef GARAGEDOOR_H
#define GARAGEDOOR_H
#include "HomeSpan.h" 

#define CURRENT_DOOR_STATE_OPEN    0
#define CURRENT_DOOR_STATE_CLOSED  1
#define CURRENT_DOOR_STATE_OPENING 2
#define CURRENT_DOOR_STATE_CLOSING 3
#define CURRENT_DOOR_STATE_STOPPED 4
#define CURRENT_DOOR_STATE_UNKNOWN 255

#define TARGET_DOOR_STATE_OPEN    0
#define TARGET_DOOR_STATE_CLOSED  1
#define TARGET_DOOR_STATE_UNKNOWN 255

struct GarageDoor : Service::GarageDoorOpener {

  int relayPin;
  int fullyOpenSensorPin;
  unsigned long moveStartTime;
  unsigned long doorClosedDetectionTime;
  bool doorInMotion;

  Characteristic::CurrentDoorState *CurrentDoorState;
  Characteristic::TargetDoorState *TargetDoorState;
  Characteristic::ObstructionDetected *ObstructionDetected;

  GarageDoor(int relayPin, int fullyOpenSensorPin);
  bool update();
  void loop();
};

#endif
