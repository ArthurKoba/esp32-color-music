//
// Created by Koba on 31.01.2023.
//

#ifndef ESP32_COLOR_MUSIC_COLORMUSICCORE_H
#define ESP32_COLOR_MUSIC_COLORMUSICCORE_H


#ifndef SAMPLES_SIZE
#define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE SAMPLES_SIZE / 2

#include "BluetoothA2DPSink.h"
#include <FastLED.h>
#include "esp_dsp.h"
#include "cmath"

enum AmplitudesType : uint8_t {
    LIN, BARK, LOG
};


struct Samples {
    int16_t left[SAMPLES_SIZE];
    int16_t right[SAMPLES_SIZE];
};


struct Amplitudes {
    float left[AMPLITUDES_SIZE];
    float right[AMPLITUDES_SIZE];
};


class ColorMusic {
public:
    ColorMusic(CRGB*);
    ~ColorMusic();

    void static callbackAddSamples(const uint8_t*, uint32_t);
    void static callbackUpdateSampleRate(uint16_t);
    void setAmplitudesType(AmplitudesType);
    void setSampleRate(uint16_t);
    CRGB *leds = nullptr;
    uint8_t *fastAmplitudes;

private:
    [[noreturn]] void static fftExecutor(void*);
    [[noreturn]] void static colorsExecutor(void*); // todo rename to normal

    void addSamples(const uint8_t*, uint32_t);
    void calcFFT(const int16_t *, float *);
    void generateBarkScaleTable();

    Samples *samples;
    Amplitudes *amplitudes;

    //    float fftWindow[SAMPLES_SIZE] __attribute__((aligned(16))); // todo dynamic memory
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16))); // todo dynamic memory
    float *barkScale = nullptr;
    uint16_t samplesFullness;
    TaskHandle_t fftTask = nullptr;
    TaskHandle_t colorsTask = nullptr;
    AmplitudesType amplitudesType = BARK;
    float frequencyStep = 0;
//    bool useWindow;
};

#endif //ESP32_COLOR_MUSIC_COLORMUSICCORE_H
