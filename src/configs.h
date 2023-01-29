#define SERIAL_SPEED 1000000

#define ENCODER_CLK_PIN 32
#define ENCODER_DT_PIN 33
#define ENCODER_BTN_PIN 27

#define IR_RECEIVE_PIN 13

#define WS2812B_PIN 12
#define NUM_LEDS 230

#define I2S_BCK_PIN 14
#define I2S_DATA_OUT_PIN 25
#define I2S_WS_PIN 26

#define BLUETOOTH_DEVICE_NAME "Koba Audio Receiver"

#define DEFAULT_MODE COLOR_MUSIC_MODE
#define BRIGHT_STEP 10

#define NO_LED_FEEDBACK_CODE // remove warning IRRemote: No definition for LED_BUILTIN found -> default LED feedback is disabled.
#define SEND_PWM_BY_TIMER // remove warning IRRemote: For ESP32, RP2040, mbed and particle boards SEND_PWM_BY_TIMER is enabled by default.
#define FASTLED_INTERNAL // remove warning FastLED: No hardware SPI pins defined.  All SPI access will default to bitbanged output