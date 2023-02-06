#ifndef ESP32_COLOR_MUSIC_COLORMUSIC_H
#define ESP32_COLOR_MUSIC_COLORMUSIC_H

#include <cstdio>
#include "fft.h"
#include "BluetoothA2DPSink.h"

class ColorMusic {
public:
    ColorMusic();
//    void setBluetoothA2DPSink(BluetoothA2DPSink *a2dp);
    void enable();
    void disable();
    void show();

    void setSampleRate(uint16_t sampleRate);
    void addSamples(const uint8_t *data, uint32_t length);

    FFTConfig getConfigFFT();
    void setConfigFFT(FFTConfig &config);
private:
//    BluetoothA2DPSink *a2dp = nullptr;
    FFTColorMusic *fft = nullptr;
    FFTConfig fftConfig;
};

#endif //ESP32_COLOR_MUSIC_COLORMUSIC_H
