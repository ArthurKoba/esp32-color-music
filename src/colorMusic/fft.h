#ifndef ESP32_COLOR_MUSIC_FFT_H
#define ESP32_COLOR_MUSIC_FFT_H


#ifndef SAMPLES_SIZE
    #define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE SAMPLES_SIZE / 2

#include "CustomBluetoothA2DPSink.h"
#include "FastLED.h"
#include "esp_dsp.h"
#include "cmath"
#include "configs.h"

enum AmplitudesType : uint8_t {
    LIN, LOG, BARK, CUSTOM_BARK
};


enum WindowType : uint8_t {
    NO_WINDOW, BLACKMAN, BLACKMAN_HARRIS, BLACKMAN_NUTTALL, FLAT_TOP, HANN, NUTTALL
};


struct Samples {
    int16_t left[SAMPLES_SIZE]{};
    int16_t right[SAMPLES_SIZE]{};
    uint16_t fullness = 0;
};


struct Amplitudes {
    float left[AMPLITUDES_SIZE]{};
    float right[AMPLITUDES_SIZE]{};
};


struct FFTConfig {
    AmplitudesType amplitudesType = LIN;
    WindowType windowType = NO_WINDOW;
    float frequencyStep = 0.0;
};


class FFTColorMusic {
public:
    explicit FFTColorMusic(FFTConfig &config, TaskHandle_t &handleEndCalculatePointer);
    ~FFTColorMusic();

    void addSamples(const uint8_t *data, uint32_t length);
    void setConfigs(FFTConfig &config);
    uint16_t static getDeltaMinMaxSample(const int16_t *samples);
    uint32_t static getMaxObjectSize();
    Samples samples;
    Amplitudes amplitudes;

private:
    [[noreturn]] void static fftExecutor(void *thisPointer);
    void calcFFT(const int16_t *samplesIn, float *amplitudeOut);
    void generateWindow(WindowType type);
    void generateBarkScale(float frequencyStep);
    void generateCustomBarkScale(float frequencyStep);

    float *barkScale;
    float *fftWindow;
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16))){};
    float *fftTable;
    TaskHandle_t &handleEndCalculate;
    FFTConfig cfg;
    TaskHandle_t handleFFTTask = nullptr;
};

#endif //ESP32_COLOR_MUSIC_FFT_H
