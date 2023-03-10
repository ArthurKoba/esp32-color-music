#ifndef BLUETOOTH_DEVICE_NAME
    #error BLUETOOTH_DEVICE_NAME required.
#endif

#ifndef I2S_BCK_PIN
    #error I2S_BCK_PIN required.
#endif

#ifndef I2S_DATA_OUT_PIN
    #error I2S_DATA_OUT_PIN required.
#endif

#ifndef I2S_WS_PIN
    #error I2S_WS_PIN required.
#endif

i2s_pin_config_t i2sPins = {
        .bck_io_num = I2S_BCK_PIN,
        .ws_io_num = I2S_WS_PIN,
        .data_out_num = I2S_DATA_OUT_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE
};

void change_volume(int newVolume) {
    Serial.println("New bt event volume: " + String(newVolume));
}