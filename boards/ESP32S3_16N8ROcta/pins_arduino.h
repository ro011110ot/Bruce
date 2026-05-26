#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "soc/soc_caps.h"
#include <stdint.h>

#ifndef DEVICE_NAME
#define DEVICE_NAME "ESP32S3_16N8ROcta"
#endif

// =============================================
// USB
// =============================================
#define USB_VID 0x303a
#define USB_PID 0x1001

// =============================================
// UART0
// =============================================
static const uint8_t TX = 43;
static const uint8_t RX = 44;

// =============================================
// Main SPI Bus (FSPI — shared with TFT, Touch, external radios)
// =============================================
#define SPI_SCK_PIN 12
#define SPI_MOSI_PIN 11
#define SPI_MISO_PIN 13
#define SPI_SS_PIN 10

static const uint8_t SS = SPI_SS_PIN;
static const uint8_t MOSI = SPI_MOSI_PIN;
static const uint8_t SCK = SPI_SCK_PIN;
static const uint8_t MISO = SPI_MISO_PIN;

// =============================================
// I2C Bus (for external grove modules)
// =============================================
#define GROVE_SDA 8
#define GROVE_SCL 15
static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// =============================================
// TFT Display — config is in .ini via -D flags
// =============================================
#define HAS_SCREEN 1
#define ROTATION 1
#define MINBRIGHT 1
#define BACKLIGHT 38

// =============================================
// Touch Screen (XPT2046 via SPI, shares FSPI with TFT)
// =============================================
#define HAS_TOUCH 1
#define TOUCH_XPT2046_SPI 1
#define XPT2046_SPI_BUS_MOSI_IO_NUM 11
#define XPT2046_SPI_BUS_MISO_IO_NUM 13
#define XPT2046_SPI_BUS_SCLK_IO_NUM 12
#define XPT2046_SPI_CONFIG_CS_GPIO_NUM 3
#define XPT2046_TOUCH_CONFIG_INT_GPIO_NUM -1

// =============================================
// Font Sizes
// =============================================
#define FP 1
#define FM 2
#define FG 3

// =============================================
// Buttons
// =============================================
#define HAS_BTN 1
#define BTN_ALIAS "\"OK\""
#define BTN_PIN 0
#define BTN_ACT LOW
#define SEL_BTN 0

// =============================================
// Infrared (external modules via expansion pins)
// =============================================
#define TXLED 40
#define RXLED 41
#define LED_ON HIGH
#define LED_OFF LOW

#define IR_TX_PINS '{{"GPIO2", 2}, {"GPIO3", 3}, {"GPIO14", 14}, {"GPIO21", 21}}'
#define IR_RX_PINS '{{"GPIO2", 2}, {"GPIO3", 3}, {"GPIO14", 14}, {"GPIO21", 21}}'

// =============================================
// RF (external modules via expansion pins)
// =============================================
#define RF_TX_PINS '{{"GPIO2", 2}, {"GPIO3", 3}, {"GPIO14", 14}, {"GPIO21", 21}}'
#define RF_RX_PINS '{{"GPIO2", 2}, {"GPIO3", 3}, {"GPIO14", 14}, {"GPIO21", 21}}'

// =============================================
// Serial (GPS) dedicated pins
// =============================================
#define SERIAL_TX 43
#define SERIAL_RX 44
#define GPS_SERIAL_TX SERIAL_TX
#define GPS_SERIAL_RX SERIAL_RX

// =============================================
// BadUSB (USB HID)
// =============================================
#define USB_as_HID 1
#define BAD_TX GROVE_SDA
#define BAD_RX GROVE_SCL

// =============================================
// Deep Sleep
// =============================================
#define DEEPSLEEP_WAKEUP_PIN 0
#define DEEPSLEEP_PIN_ACT LOW

#endif /* Pins_Arduino_h */
