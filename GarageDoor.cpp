#include "GarageDoor.h"

GarageDoor::GarageDoor(int relayPin, int fullyOpenSensorPin) : Service::GarageDoorOpener() {
  CurrentDoorState = new Characteristic::CurrentDoorState(CURRENT_DOOR_STATE_OPEN);
  TargetDoorState = new Characteristic::TargetDoorState(TARGET_DOOR_STATE_OPEN);
  ObstructionDetected = new Characteristic::ObstructionDetected();

  this->doorInMotion = false;
  this->relayPin = relayPin;
  this->fullyOpenSensorPin = fullyOpenSensorPin;
  pinMode(relayPin, OUTPUT);
  pinMode(fullyOpenSensorPin, INPUT_PULLUP);
}

boolean GarageDoor::update() {
  int targetState = TargetDoorState->getNewVal();
  int currentState = CurrentDoorState->getVal();

  Serial.print("Door is switcing from state ");
  Serial.println(currentState);
  Serial.print(" to ");
  Serial.println(targetState);

  if ((targetState == TARGET_DOOR_STATE_OPEN && currentState == CURRENT_DOOR_STATE_CLOSED) ||
      (targetState == TARGET_DOOR_STATE_CLOSED && currentState == CURRENT_DOOR_STATE_OPEN)) {
    LOG1("Garage door triggered\n");

    // Activate the relay
    digitalWrite(relayPin, HIGH);
    delay(1000);
    digitalWrite(relayPin, LOW);

    // Update the door state
    if (targetState == TARGET_DOOR_STATE_OPEN) {
      CurrentDoorState->setVal(CURRENT_DOOR_STATE_OPENING);
      doorInMotion = true;
      moveStartTime = millis();
    } else if (targetState == TARGET_DOOR_STATE_CLOSED) {
      CurrentDoorState->setVal(CURRENT_DOOR_STATE_CLOSING);
      doorInMotion = true;
      moveStartTime = millis();
    }
  }

  return true;
}

void GarageDoor::loop() {
  unsigned long currentTime = millis();

  // Check if the door was supposed to finish moving
  if (doorInMotion && currentTime - moveStartTime > 15000) {
    doorInMotion = false;

    if (TargetDoorState->getVal() == TARGET_DOOR_STATE_OPEN) {
      CurrentDoorState->setVal(CURRENT_DOOR_STATE_OPEN);
      doorClosedDetectionTime = 0;
    }
    else if (TargetDoorState->getVal() == TARGET_DOOR_STATE_CLOSED) {
      CurrentDoorState->setVal(CURRENT_DOOR_STATE_CLOSED);
    }
  }

  // Check the sensor state
  if (digitalRead(fullyOpenSensorPin) == LOW) {
    doorClosedDetectionTime = 0;
    if (!doorInMotion && CurrentDoorState->getVal() != CURRENT_DOOR_STATE_OPEN) {
      Serial.println("Door state updated to: open");
      TargetDoorState->setVal(TARGET_DOOR_STATE_OPEN);
      CurrentDoorState->setVal(CURRENT_DOOR_STATE_OPEN);
    }
  } else {
    // If the door is detected as not fully open, start or continue the closed detection timer
    if (!doorInMotion && CurrentDoorState->getVal() != CURRENT_DOOR_STATE_CLOSED) {
      if (doorClosedDetectionTime == 0) {
        doorClosedDetectionTime = millis();
      } 
      else {
        unsigned long elapsedTime = currentTime - doorClosedDetectionTime;
        if (elapsedTime > 20000) {
          Serial.println("Door state updated to: closed");
          TargetDoorState->setVal(TARGET_DOOR_STATE_CLOSED);
          CurrentDoorState->setVal(CURRENT_DOOR_STATE_CLOSED);
        } else {
          static unsigned long lastPrintTime = 0;
          if (currentTime - lastPrintTime >= 1000) {
            Serial.print("Door closing detected... ");
            Serial.print(20 - elapsedTime / 1000);
            Serial.println(" seconds remaining until state updates.");
            lastPrintTime = currentTime;
          }
        }
      }
    }
  }
}
