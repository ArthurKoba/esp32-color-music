#ifndef ESP32_COLOR_MUSIC_FFT_H
#define ESP32_COLOR_MUSIC_FFT_H

#include "configs.h"

#ifndef SAMPLES_SIZE
#define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE (SAMPLES_SIZE / 2)

#include "esp32-hal.h"
#include "esp_dsp.h"

enum AmplitudesType {LIN, LOG, BARK, CUSTOM_BARK};


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
    AmplitudesType amplitudes_type = LIN;
    WindowType window_type = NO_WINDOW;
    float frequency_step = 0.0;
};


struct __attribute__((packed)) StereoFrame16 {
    int16_t channel1 = 0;
    int16_t channel2 = 0;
};


class FFTCore {
    float *_bark_scale = nullptr;
    float *_fft_window = nullptr;
    float _buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16))){};
    float _fft_table[SAMPLES_SIZE]{};
    FFTConfig _cfg;

public:
    Samples samples{};
    Amplitudes amplitudes{};

    explicit FFTCore(FFTConfig &config);
    ~FFTCore();
    void add_samples(const uint8_t *data, uint32_t length);
    void set_configs(FFTConfig &config);
    void calculate();
private:
    void _calculate_target(const int16_t *samples_in, float *amplitude_out);
    void _generate_window(WindowType type);
    void _generate_bark_scale(float frequency_step);
    void _generate_custom_bark_scale(float frequency_step);
};

#endif //ESP32_COLOR_MUSIC_FFT_H
