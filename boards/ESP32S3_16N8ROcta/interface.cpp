#include "CYD28_TouchscreenR.h"
#include "core/powerSave.h"
#include "core/utils.h"
#include <Arduino.h>
#include <Wire.h>
#include <globals.h>
#include <interface.h>

#define BOARD_DISPLAY_HOR_RES 320
#define BOARD_DISPLAY_VER_RES 240
CYD28_TouchR touch(BOARD_DISPLAY_HOR_RES, BOARD_DISPLAY_VER_RES);

#define BOARD_BTN_PIN 0
#define BOARD_BTN_ACT LOW

/***************************************************************************************
** Function name: _setup_gpio()
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {
    // Keep XPT2046 CS high until needed
    pinMode(XPT2046_SPI_CONFIG_CS_GPIO_NUM, OUTPUT);
    digitalWrite(XPT2046_SPI_CONFIG_CS_GPIO_NUM, HIGH);

    // Start with default module configs
    bruceConfigPins.rfModule = CC1101_SPI_MODULE;
    bruceConfigPins.rfidModule = PN532_I2C_MODULE;
    bruceConfigPins.irRx = RXLED;
    bruceConfigPins.irTx = TXLED;

    Serial.begin(115200);
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Description:   second stage gpio setup after TFT is initialized
***************************************************************************************/
void _post_setup_gpio() {
    // Initialize XPT2046 touch using hardware SPI (shares bus with display)
    if (!touch.begin(&tft.getSPIinstance())) {
        Serial.println("[TOUCH] XPT2046 not started");
    } else {
        Serial.println("[TOUCH] XPT2046 started OK");
    }
    touch.setRotation(ROTATION);

    // Backlight control via PWM
    pinMode(TFT_BL, OUTPUT);
    analogWrite(TFT_BL, 255);
}

/***************************************************************************************
** Function name: getBattery()
** Description:   Delivers the battery value from 0-100 (not available on this board)
***************************************************************************************/
int getBattery() { return 0; }

/*********************************************************************
** Function: setBrightness
** set brightness value (0-100)
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        analogWrite(TFT_BL, 0);
    } else {
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
        analogWrite(TFT_BL, bl);
    }
}

/*********************************************************************
** Function: InputHandler
** Handles touch and button inputs
** Maps to PrevPress, NextPress, SelPress, AnyKeyPress, EscPress
**********************************************************************/
void InputHandler(void) {
    static long d_tmp = 0;

    if (millis() - d_tmp > 200 || LongPress) {
        // ---- Touch Screen Input (XPT2046) ----
        if (touch.touched()) {
            auto t = touch.getPointScaled();
            d_tmp = millis();

            // Apply rotation transformation
            if (bruceConfigPins.rotation == 3) {
                t.y = (tftHeight + 20) - t.y;
                t.x = tftWidth - t.x;
            }
            if (bruceConfigPins.rotation == 0) {
                int tmp = t.x;
                t.x = tftWidth - t.y;
                t.y = tmp;
            }
            if (bruceConfigPins.rotation == 2) {
                int tmp = t.x;
                t.x = t.y;
                t.y = (tftHeight + 20) - tmp;
            }

            if (!wakeUpScreen()) AnyKeyPress = true;
            else goto END;

            touchPoint.x = t.x;
            touchPoint.y = t.y;
            touchPoint.pressed = true;
            touchHeatMap(touchPoint);
        }
    END:

        // ---- BOOT Button Input ----
        checkPowerSaveTime();
        if (digitalRead(BOARD_BTN_PIN) == BOARD_BTN_ACT) {
            if (!wakeUpScreen()) {
                AnyKeyPress = true;
                SelPress = true;
            }
            long tmp = millis();
            while ((millis() - tmp) < 200 && digitalRead(BOARD_BTN_PIN) == BOARD_BTN_ACT);
        }
    }
}

/*********************************************************************
** Function: powerOff
** Turns off the device (deep sleep with BOOT button wakeup)
**********************************************************************/
void powerOff() {
    analogWrite(TFT_BL, 0);
    tft.writecommand(0x10); // SLPIN
    esp_sleep_enable_ext0_wakeup((gpio_num_t)BOARD_BTN_PIN, BOARD_BTN_ACT);
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: goToDeepSleep
** Puts the device into DeepSleep
**********************************************************************/
void goToDeepSleep() { powerOff(); }

/*********************************************************************
** Function: checkReboot
** Button logic to turn off the device
**********************************************************************/
void checkReboot() {
    int c = 0;
    while (digitalRead(BOARD_BTN_PIN) == BOARD_BTN_ACT) {
        delay(100);
        c++;
        if (c > 20) { // 2 second long press
            powerOff();
        }
    }
}

/***************************************************************************************
** Function name: isCharging()
** Description:   Determines if the device is charging (not available on this board)
***************************************************************************************/
bool isCharging() { return false; }
