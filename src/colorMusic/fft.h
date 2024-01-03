#ifndef ESP32_COLOR_MUSIC_FFT_H
#define ESP32_COLOR_MUSIC_FFT_H


#ifndef SAMPLES_SIZE
#define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE (SAMPLES_SIZE / 2)

#include "esp32-hal.h"
#include "esp_dsp.h"
#include <cmath>
#include "configs.h"

enum AmplitudesType {
    LIN, LOG, BARK, CUSTOM_BARK
};


enum WindowType {
    NO_WINDOW, BLACKMAN, BLACKMAN_HARRIS, BLACKMAN_NUTTALL, FLAT_TOP, HANN, NUTTALL
};


struct Samples {
    int16_t left[SAMPLES_SIZE]{};
    int16_t right[SAMPLES_SIZE]{};
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


struct __attribute__((packed)) StereoFrame16 {
    int16_t channel1 = 0;
    int16_t channel2 = 0;
};


class FFTColorMusic {
public:
    explicit FFTColorMusic(FFTConfig &config);
    ~FFTColorMusic();
    void addSamples(const uint8_t *data, uint32_t length);
    void setConfigs(FFTConfig &config);
    void calculate();

    Samples samples{};
    Amplitudes amplitudes{};

private:
    void calculateTarget(const int16_t *samplesIn, float *amplitudeOut);
    void generateWindow(WindowType type);
    void generateBarkScale(float frequencyStep);
    void generateCustomBarkScale(float frequencyStep);

    float *barkScale = nullptr;
    float *fftWindow = nullptr;
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16))){};
    float fftTable[SAMPLES_SIZE]{};
    FFTConfig cfg;
};

#endif //ESP32_COLOR_MUSIC_FFT_H
