# Home Kit garage control

This garage control system is implemented using [Home Span](https://github.com/HomeSpan/HomeSpan), which simplifies creating HomeKit-compatible accessories on an ESP32.

As of now, it only controls the garage door. The opener is controlled by a relay. When a user opens the garage through HomeKit, the relay is triggered for one second, causing the gate to open and updating the state accordingly.

Both to ensure the door state is correct, and because it is also possible to control the garage by a remote (RF), I have mounted a sensor (magnetic switch) for detecting if the gate is open. This sensor will update the door-state if a change is detected. Ideally there should also be another sensor for when the gate is closed, now a timer is used instead, because of this a jammed gate/obstruction is not possible to detect. In the future i might need to install an additional sensor.

## Installation

- By using the [Arduino IDE](https://www.arduino.cc/en/software) you can easily flash the code to any ESP32. I used an ESP32 cam as it features a connector for an external antenna.
  I used the board manager from [espressif](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html). Note that HomeSpan currently only support version 2. Version 2.0.17 worked great for me.
- Before flashing, replace the pairing code with a personal 8-digit code.
- When the ESP32 is up and running, you can connect to your preferred WIFI trough the serial monitor. Home Span will guide you trough the process. The WIFI credentials is remembered even if you need to change out some code and flash the ESP32 again.

## Wiring

- For the relay, i simply use a relay-module with the required components pre-installed. This also do not take up too much space in the box. The module requires 5v and GND, and is controlled by pin 2 in my case. The I/O pins is selected when initializing the GarageDoor object from [smartGarage.ino](/smartGarage.ino)
- For the sensor i utilize the built in pull-up resistor in the ESP32. Pin 14 is set to input with pull-up. The other side is connected to GND for pulling down the input voltage when triggered
