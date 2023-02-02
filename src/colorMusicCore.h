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
    LIN, LOG, BARK, CUSTOM_BARK
};


enum WindowType : uint8_t {
    NO_WINDOW, BLACKMAN, BLACKMAN_HARRIS, BLACKMAN_NUTTALL, FLAT_TOP, HANN, NUTTALL
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

    void setAmplitudesType(AmplitudesType);
    void setSampleRate(uint16_t);
    void setWindowType(WindowType);
    CRGB *leds = nullptr;
    uint8_t *fastAmplitudes;
    uint16_t samplesFullness = 0;

    void addSamples(const uint8_t*, uint32_t);

private:
    [[noreturn]] void static fftExecutor(void*);
    [[noreturn]] void static colorsExecutor(void*); // todo rename to normal
    [[noreturn]] void static sendExecutor(void*);

    void calcFFT(const int16_t *, float *);
    void generateBarkScaleTable();
    void generateCustomBarkScaleTable();

    Samples *samples;
    Amplitudes *amplitudes;

    float fftWindow[SAMPLES_SIZE] __attribute__((aligned(16))); // todo dynamic memory
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16))); // todo dynamic memory
    float *barkScale;
    AmplitudesType amplitudesType = BARK;
    WindowType windowType = NO_WINDOW;
    float frequencyStep = 0.0;
    TaskHandle_t fftTask = nullptr;
    TaskHandle_t colorsTask = nullptr;
    TaskHandle_t sendTask = nullptr;
};


#endif //ESP32_COLOR_MUSIC_COLORMUSICCORE_H
