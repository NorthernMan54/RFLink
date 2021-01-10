// ************************************* //
// * Arduino Project RFLink-esp        * //
// * https://github.com/couin3/RFLink  * //
// * 2018..2020 Stormteam - Marc RIVES * //
// * More details in RFLink.ino file   * //
// ************************************* //

#include <Arduino.h>
#include "RFLink.h"
#include "4_Display.h"
#include "9_I2C_Sensor.h"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C

unsigned long sensorRead = 0;

void I2CSensorSetup()
{
    Serial.print(F("I2C SDA :\t\t"));
    Serial.println(PIN_I2C_SDA);
    Serial.print(F("I2C SCL :\t\t"));
    Serial.println(PIN_I2C_SCL);
    Serial.print(F("FREQUENCY :\t\t"));
    Serial.println((int)(FREQUENCY / 1000) );
     Serial.println();
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    // Wire.begin(D6, D5);
    if (!bme.begin(I2C_ADDRESS, &Wire))
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
};

//

boolean I2CSensorLoop()
{
    if (millis() > (sensorRead + FREQUENCY) || sensorRead == 0)
    {
        sensorRead = millis();
        bme.takeForcedMeasurement();

        int temperature = (unsigned int)bme.readTemperature() * 10;

        if (temperature < 0)
        {
            temperature = (temperature * -1) | 0x8000;
        }

        //==================================================================================
        // Output
        //==================================================================================
        display_Header();
        display_Name(PSTR("BME280"));
        char c_ID[5];
        sprintf(c_ID, "%04x", I2C_ADDRESS);
        display_IDc(c_ID);
        display_TEMP((unsigned int)temperature);
        display_HUM((byte)bme.readHumidity(), HUM_HEX);
        display_BARO((unsigned int)bme.readPressure() / 100.0F);
        display_Footer();
        return true;
    }
    return false;
};