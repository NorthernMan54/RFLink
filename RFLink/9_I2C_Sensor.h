// ************************************* //
// * Arduino Project RFLink-esp        * //
// * https://github.com/couin3/RFLink  * //
// * 2018..2020 Stormteam - Marc RIVES * //
// * More details in RFLink.ino file   * //
// ************************************* //

#ifndef I2C_SENSOR_h
#define I2C_SENSOR_h

#include <Arduino.h>
#include "RFLink.h"

#ifdef ESP8266
#define PIN_I2C_SCL D5        // I2C SCL GPIO14
#define PIN_I2C_SDA D6        // I2C SDA GPIO12
#elif ESP32
#define PIN_OLED_SCL 22        // I2C SCL
#define PIN_OLED_SDA 21        // I2C SDA
#endif

#define I2C_ADDRESS 0x76        // I2C Address

#define FREQUENCY 300000 // Every 5 minutes

void I2CSensorSetup();
boolean I2CSensorLoop();

#endif