/*
* File: config.h
* Description: Global configuration, pin definitions, and user data structures.
* Note: Contains sensitive data (WiFi/Discord) that should be redacted in public repos.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

struct User {
  const char* name;
  const char* discordId;
};

//-----REPLACE ME-----
static const char* WIFI_SSID = "WIFI_NAME";
static const char* WIFI_PASSWORD = "WIFI_PASSWORD";
static const char* DISCORD_WEBHOOK_URL = "DISCORD_WEBHOOK_LINK";

static const User users[] = {
  {"USER1_NAME", "USER1_DISCORD_ID"}, // This line can be copied to add multiple users
};
//-----REPLACE ME (END)-----

static const int USER_COUNT = sizeof(users) / sizeof(users[0]);

static const bool IS_DRYER = true; 
static const uint8_t ENCODER_CLK_PIN = 35;
static const uint8_t ENCODER_DT_PIN = 32;
static const uint8_t BUTTON_PIN = 23;
static const uint8_t LIGHT_SENSOR_PIN = 34;

static const uint8_t LCD_ADDR = 0x27;
static const uint8_t LCD_COLS = 16;
static const uint8_t LCD_ROWS = 2;

static const uint8_t DISCORD_MESSAGE_BUFFER_SIZE = 100;
static const uint8_t DISCORD_SEND_ATTEMPTS = 3;
static const uint32_t DISCORD_RETRY_DELAY_MS = 10000;

#endif