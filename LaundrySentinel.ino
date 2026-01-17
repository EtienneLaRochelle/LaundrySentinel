/*
* Project: Laundry Sentinel
*/

#include "LaundryController.h"

LaundryController controller;

void setup() {
  controller.begin();
}

void loop() {
  controller.update();
  delay(10);
}