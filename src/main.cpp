#define ENCODER_CLK_PIN 32
#define ENCODER_DT_PIN 33
#define ENCODER_BTN_PIN 27

#define I2S_BCK_PIN 14
#define I2S_DATA_OUT_PIN 25
#define I2S_WS_PIN 26

#define BLUETOOTH_DEVICE_NAME "Koba Audio Receiver"

#include "Arduino.h"
#include "serialPortInteraction.h"
#include "colorMusic.h"
#include "audioReceiver.h"
#include "encoder.h"


void setup() {
    Serial.begin(500000);
    setupA2DP();
    a2dp_sink.set_volume(64);
    setupEncoder();
    setupColorMusic();
    Serial.print(BLUETOOTH_DEVICE_NAME);
    Serial.println(" Started!");
}



void loop() {
    if (encoderData.isUpdated) execEncoder();
//    colorMusic();
}


