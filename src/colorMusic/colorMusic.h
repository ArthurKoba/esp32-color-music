#ifndef ESP32_COLOR_MUSIC_COLORMUSIC_H
#define ESP32_COLOR_MUSIC_COLORMUSIC_H

#include <cstdio>
#include "ledStrip/customLedStrip.h"
#include "fft.h"
#include "CustomBluetoothA2DPSink.h"
#include "serialPortInteraction/serialPortInteraction.h"

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
    FFTColorMusic *fft = nullptr;
    TaskHandle_t handleColorMusic = nullptr;
    FFTConfig fftConfig;
    CustomLedStrip &strip;
    CustomBluetoothA2DPSink *a2dp = nullptr;
    SerialPortInteraction *serialPortInteraction;
    uint8_t amplitudesLeft[AMPLITUDES_SIZE];
    uint8_t amplitudesRight[AMPLITUDES_SIZE];
};

#endif //ESP32_COLOR_MUSIC_COLORMUSIC_H
