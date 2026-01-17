/*
* File: LaundryController.h
*/

#ifndef LAUNDRY_CONTROLLER_H
#define LAUNDRY_CONTROLLER_H

#include "config.h"
#include <ESP32Encoder.h>
#include <LCD_I2C.h>
#include <WiFi.h>
#include <Discord_WebHook.h>

class LaundryController {
public:
    LaundryController();

    void begin();
    void update();

private:
    ESP32Encoder encoder;
    LCD_I2C lcd;
    Discord_Webhook discord;

    bool isCycleStarted;
    long currentPosition;
    long previousPosition;
    float initialLightLevel;
    bool isWashFinished;
    bool initCycle;

    //-----SETUP-----
    void connectToNetwork();
    void connectToDiscord();
    void setupHardware();
    
    //-----SCREEN-----
    void updateLCDUser();
    void printLCD(uint8_t row, String text);
    void clearLineLCD(uint8_t row);
    
    //-----MAIN LOGIC-----
    void handleButtonClick();
    void showUserSelection();
    void checkLaundryStatus();
    void detectWashFinished();
    void sendDiscordNotification(String customMessage);
};

#endif