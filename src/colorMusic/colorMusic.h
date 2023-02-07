#ifndef ESP32_COLOR_MUSIC_COLORMUSIC_H
#define ESP32_COLOR_MUSIC_COLORMUSIC_H

#include <cstdio>
#include "fft.h"
#include "CustomBluetoothA2DPSink.h"

class ColorMusic {
public:
    ColorMusic();
    void enable();
    ~ColorMusic();
    void disable();
    void setupCallbacks(CustomBluetoothA2DPSink *a2dp);

    void static setSampleRate(uint16_t sampleRate, ColorMusic *thisPointer);
    void static addSamples(const uint8_t *data, uint32_t length, ColorMusic *thisPointer);

    FFTConfig getConfigFFT();
    void setConfigFFT(FFTConfig &config);
private:
    FFTColorMusic *fft = nullptr;
    FFTConfig fftConfig;
    CustomBluetoothA2DPSink *a2dp = nullptr;
};

#endif //ESP32_COLOR_MUSIC_COLORMUSIC_H
