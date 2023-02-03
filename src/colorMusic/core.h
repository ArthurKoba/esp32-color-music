#ifndef ESP32_COLOR_MUSIC_CORE_H
#define ESP32_COLOR_MUSIC_CORE_H


#ifndef SAMPLES_SIZE
    #define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE SAMPLES_SIZE / 2

#include "BluetoothA2DPSink.h"
#include "FastLED.h"
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
    explicit ColorMusic(CRGB*);
    ~ColorMusic();

    void setAmplitudesType(AmplitudesType type);
    void setSampleRate(uint16_t sampleRate);
    void setWindowType(WindowType type);
    void addSamples(const uint8_t *data, uint32_t length);

    CRGB *leds = nullptr;
    uint8_t *fastAmplitudes;
    uint16_t samplesFullness = 0;

private:
    [[noreturn]] void static fftExecutor(void *pvParam);
    [[noreturn]] void static colorsExecutor(void *pvParam); // todo rename to normal
    [[noreturn]] void static sendExecutor(void *pvParam);

    void calcFFT(const int16_t *, float *);
    void generateBarkScaleTable();
    void generateCustomBarkScaleTable();

    TaskHandle_t fftTask = nullptr;
    TaskHandle_t colorsTask = nullptr;
    TaskHandle_t sendTask = nullptr;

    Samples *samples;
    Amplitudes *amplitudes;
    float *barkScale;
    float fftWindow[SAMPLES_SIZE] __attribute__((aligned(16))); // todo dynamic memory
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16))); // todo dynamic memory

    AmplitudesType amplitudesType = BARK;
    WindowType windowType = NO_WINDOW;
    float frequencyStep = 0.0;
};

#endif //ESP32_COLOR_MUSIC_CORE_H
