#ifndef ESP32_COLOR_MUSIC_COLORMUSIC_H
#define ESP32_COLOR_MUSIC_COLORMUSIC_H

#include <cstdio>
#include "configs.h"
#include "ledStrip/customLedStrip.h"
#include "fft.h"
#include "CustomBluetoothA2DPSink.h"
#include "serialPortInteraction/serialPortInteraction.h"


struct ChannelBright {
    uint8_t low = 0;
    uint8_t middle = 0;
    uint8_t high = 0;
};

struct SamplesBuffer {
    uint8_t *data;
    uint32_t length;
};


class ColorMusic {
public:
    ColorMusic();
    ~ColorMusic();
    void enable();
    void disable();
    void setupCallbacks(CustomBluetoothA2DPSink *a2dp);

    void static setSampleRate(uint16_t sampleRate, void *thisPointer);
    void static addSamples(const uint8_t *data, uint32_t length, void *thisPointer);

    FFTConfig getConfigFFT();
    void setConfigFFT(FFTConfig &config);

    void setStrip(CustomLedStrip *strip_ptr);
    void setSerialPortInteraction(SerialPortInteraction *serialPortInteractionPointer);

    float amplitudes[AMPLITUDES_SIZE];

private:
    [[noreturn]] void static showTask(void *pvParam);
    void show();
    ChannelBright calculateBrightFromChannel(const float *channel) const;
    QueueHandle_t samplesQueue = nullptr;
    FFTColorMusic *fft = nullptr;
    TaskHandle_t handleColorMusic = nullptr;
    FFTConfig fftConfig;
    CustomLedStrip *strip = nullptr;
    SerialPortInteraction *serialPortInteraction = nullptr;
    CustomBluetoothA2DPSink *a2dp = nullptr;
    uint32_t lastTimeRed = millis();
    uint32_t lastTimeGreen = millis();
    uint32_t lastTimeBlue = millis();
};

#endif //ESP32_COLOR_MUSIC_COLORMUSIC_H
