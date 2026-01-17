/*
* File: LaundryController.cpp
*/

#include "LaundryController.h"

LaundryController::LaundryController() 
  : lcd(LCD_ADDR, LCD_COLS, LCD_ROWS), 
    isCycleStarted(false), 
    currentPosition(0), 
    previousPosition(-1), 
    initialLightLevel(-1.0f), 
    isWashFinished(false), 
    initCycle(false) 
{
}

void LaundryController::begin() {
    Serial.begin(115200);
    setupHardware();
    connectToNetwork();
    connectToDiscord();
}

void LaundryController::update() {
    handleButtonClick();

    if (isCycleStarted) {
        checkLaundryStatus();
    } else {
        showUserSelection();
    }
}



//-----SETUP-----

void LaundryController::connectToNetwork() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi Connected!");
}

void LaundryController::connectToDiscord() {
    String url = String(DISCORD_WEBHOOK_URL);
    int lastSlash = url.lastIndexOf('/');
    int secondLastSlash = url.lastIndexOf('/', lastSlash - 1);
    String id = url.substring(secondLastSlash + 1, lastSlash);
    String token = url.substring(lastSlash + 1);
    discord.begin(id, token);
}

void LaundryController::setupHardware() {
    encoder.attachHalfQuad(ENCODER_DT_PIN, ENCODER_CLK_PIN);
    encoder.setCount(0);
    
    lcd.begin(27, 26);
    lcd.backlight();
    updateLCDUser();
    
    pinMode(BUTTON_PIN, INPUT);
    pinMode(LIGHT_SENSOR_PIN, INPUT);
}



//-----SCREEN-----

void LaundryController::updateLCDUser() {
    printLCD(0, users[currentPosition].name);
}

void LaundryController::printLCD(uint8_t row, String text) {
    clearLineLCD(row);
    lcd.print(text);
}

void LaundryController::clearLineLCD(uint8_t row) {
    lcd.setCursor(0, row);
    lcd.print("                ");
    lcd.setCursor(0, row);
}



//-----MAIN LOGIC-----

void LaundryController::handleButtonClick() {
    if (!digitalRead(BUTTON_PIN)) {
        delay(50); // Debounce
        if (!digitalRead(BUTTON_PIN)) {
            // Intentional blocking delay to prevent button bouncing.
            while (!digitalRead(BUTTON_PIN)) { delay(10); }

            initialLightLevel = analogRead(LIGHT_SENSOR_PIN);
            if (initialLightLevel == 0.00) initialLightLevel = 1.0f;

            isCycleStarted = !isCycleStarted;

            lcd.clear();
            if (!isCycleStarted) {
                updateLCDUser();
            } else {
                initCycle = false;
                isWashFinished = false; 
            }
        }
    }
}

void LaundryController::showUserSelection() {
    if (isCycleStarted) return;
    
    encoder.resumeCount();
    currentPosition = abs(((encoder.getCount() / 2) % USER_COUNT));
    
    if (currentPosition != previousPosition) {
        updateLCDUser();
        previousPosition = currentPosition;
    }
}

void LaundryController::checkLaundryStatus() {
    // initCycle is used for executing certain actions only once when a cycle is started
    if (!initCycle) {
        printLCD(0, users[currentPosition].name);
        printLCD(1, "In Progress");
        sendDiscordNotification("Cycle in progress");
        initCycle = true;
    }

    encoder.pauseCount();
    detectWashFinished();
}

void LaundryController::detectWashFinished() {
    // Check if light increased by 70% (Threshold calibrated experimentally)
    if (initialLightLevel >= 0.0f && analogRead(LIGHT_SENSOR_PIN) >= initialLightLevel * 1.7f && !isWashFinished) {
        printLCD(1, "Finished");
        sendDiscordNotification("Cycle finished");
        isWashFinished = true;
    }
}

void LaundryController::sendDiscordNotification(String customMessage) {
    char message[DISCORD_MESSAGE_BUFFER_SIZE];

    // Format the message with Discord's specific mention syntax: <@USER_ID>
    snprintf(message, DISCORD_MESSAGE_BUFFER_SIZE, "<@%s> %s", users[currentPosition].discordId, customMessage.c_str());

    for (int attempt = 0; attempt < DISCORD_SEND_ATTEMPTS; attempt++) {
        if (discord.send(message)) {
            Serial.println("Discord sent!");
            return;
        } else {
            delay(DISCORD_RETRY_DELAY_MS);
        }
    }
}