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

class ColorMusic {
public:
    explicit ColorMusic(CustomLedStrip &targetStrip);
    void enable();
    ~ColorMusic();
    void disable();
    void setupCallbacks(CustomBluetoothA2DPSink *a2dp);

    void static setSampleRate(uint16_t sampleRate, void *thisPointer);
    void static addSamples(const uint8_t *data, uint32_t length, void *thisPointer);

    FFTConfig getConfigFFT();
    void setConfigFFT(FFTConfig &config);

    void setSerialPortInteraction(SerialPortInteraction *serialPortInteractionPointer);

private:
    [[noreturn]] void static showTask(void *context);
    void show();
    ChannelBright calculateBrightFromChannel(const float *channel) const;
    FFTColorMusic *fft = nullptr;
    TaskHandle_t handleColorMusic = nullptr;
    FFTConfig fftConfig;
    CustomLedStrip &strip;
    CustomBluetoothA2DPSink *a2dp = nullptr;
    SerialPortInteraction *serialPortInteraction;
};

#endif //ESP32_COLOR_MUSIC_COLORMUSIC_H
