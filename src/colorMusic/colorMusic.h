#ifndef ESP32_COLOR_MUSIC_COLORMUSIC_H
#define ESP32_COLOR_MUSIC_COLORMUSIC_H

#include <cstdio>
#include "fft.h"
#include "BluetoothA2DPSink.h"

class ColorMusic {
public:
    ColorMusic();
    void enable();
    void disable();
    void show();

    void static setSampleRate(uint16_t sampleRate, ColorMusic *thisPointer);
    void static addSamples(const uint8_t *data, uint32_t length, ColorMusic *thisPointer);

    FFTConfig getConfigFFT();
    void setConfigFFT(FFTConfig &config);
private:
    FFTColorMusic *fft = nullptr;
    FFTConfig fftConfig;
};

#endif //ESP32_COLOR_MUSIC_COLORMUSIC_H
