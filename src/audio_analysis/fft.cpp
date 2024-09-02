#include "fft.h"
#include <cmath>

FFTCore::FFTCore(FFTConfig &config) {
    dsps_fft2r_init_fc32(_fft_table, SAMPLES_SIZE);
    if (config.window_type != NO_WINDOW) {
        _fft_window = new float[SAMPLES_SIZE];
        _generate_window(config.window_type);
    }

    if (config.amplitudes_type == BARK or config.amplitudes_type == CUSTOM_BARK) {
        _bark_scale = new float[AMPLITUDES_SIZE];
        if (config.amplitudes_type == BARK)
            _generate_bark_scale(config.frequency_step);
        else if (config.amplitudes_type == CUSTOM_BARK)
            _generate_custom_bark_scale(config.frequency_step);
    }

    _cfg = config;
}

FFTCore::~FFTCore() {
    delete[] _fft_window;
    delete[] _bark_scale;
}

void FFTCore::set_configs(FFTConfig &config) {
    // todo need testing
    bool is_need_window = config.window_type != NO_WINDOW;
    bool is_window_generated = _cfg.window_type != NO_WINDOW;

    if (is_need_window and not _fft_window) _fft_window = new float[SAMPLES_SIZE];
    else if (not is_need_window and _fft_window) {
        printf("[%lu] freeing window memory\n", millis());
        delete[] _fft_window;
        _fft_window = nullptr;
    }
    if (is_need_window and config.window_type != _cfg.window_type) _generate_window(config.window_type);

    bool is_need_bark_scale = config.amplitudes_type == BARK or config.amplitudes_type == CUSTOM_BARK;
    bool is_need_update_bark_scale = (_cfg.amplitudes_type != config.amplitudes_type or config.frequency_step != _cfg.frequency_step);

    if (is_need_bark_scale and not _bark_scale) {
//        printf("[%lu] memory allocation for the bark scale\n", millis());
        _bark_scale = new float[AMPLITUDES_SIZE];

    } else if (not is_need_bark_scale and _bark_scale) {
//        printf("[%lu] freeing window memory bark scale\n", millis());
        delete[] _bark_scale;
        _bark_scale = nullptr;
    }

    if (is_need_update_bark_scale and config.amplitudes_type == BARK) {
        _generate_bark_scale(config.frequency_step);
    } else if (is_need_update_bark_scale and config.amplitudes_type == CUSTOM_BARK) {
        _generate_custom_bark_scale(config.frequency_step);
    }

    _cfg = config;
}

void FFTCore::add_samples(const uint8_t *data, uint32_t length) {
    length = length / 4;
    //todo use cast
    auto frame = (StereoFrame16*)data;
    if (length < SAMPLES_SIZE) {
        // offset with fill
        // 0,1,2,3,4 to 3,4,100,101,102 where 100,101,102 - data
        uint16_t count_of_out_of_bounds = SAMPLES_SIZE - length;
        for (uint16_t i = 0; i < count_of_out_of_bounds; i++) {
            samples.left[i] = samples.left[i + length];
            samples.right[i] = samples.right[i + length];
        }
        for (int i = 0; i < length; ++i) {
            samples.left[count_of_out_of_bounds + i] = frame[i].channel1;
            samples.right[count_of_out_of_bounds + i] = frame[i].channel2;
        }
    } else if (length == SAMPLES_SIZE) {
        // direct filling
        for (int i = 0; i < SAMPLES_SIZE; ++i) {
            samples.left[i] = frame[i].channel1;
            samples.right[i] = frame[i].channel2;
        }
    } else {
        // filling from the end
        // 0,1,2 to 102,103,104 where 100,101,102,103,104 - data
        uint16_t count_of_out_of_bounds = length - SAMPLES_SIZE;
        for (int i = 0; i < SAMPLES_SIZE; ++i) {
            samples.left[i] = frame[count_of_out_of_bounds + i].channel1;
            samples.right[i] = frame[count_of_out_of_bounds + i].channel2;
        }
    }
}

void FFTCore::_calculate_target(const int16_t *samples_in, float *amplitude_out) {
    // todo add mutex. maybe raise errors after change fft configs

    for (int i = 0; i < SAMPLES_SIZE; i++) {
        _buffer[i * 2 + 0] = static_cast<float>(samples_in[i]);
        if (_cfg.window_type not_eq NO_WINDOW) {
            _buffer[i * 2 + 0] *= _fft_window[i];
        }
        _buffer[i * 2 + 1] = 0;
    }

    dsps_fft2r_fc32(_buffer, SAMPLES_SIZE);
    dsps_bit_rev_fc32(_buffer, SAMPLES_SIZE);
    dsps_cplx2reC_fc32(_buffer, SAMPLES_SIZE);

    float temp;
    for (int i = 0 ; i < AMPLITUDES_SIZE ; i++) {
        temp = _buffer[i * 2 + 0] * _buffer[i * 2 + 0] + _buffer[i * 2 + 1] * _buffer[i * 2 + 1];
        if (_cfg.amplitudes_type == LIN || _cfg.amplitudes_type == BARK) {
            amplitude_out[i] = 2 * sqrtf(temp) / SAMPLES_SIZE;
        }
        switch(_cfg.amplitudes_type) {
            case BARK: amplitude_out[i] *= _bark_scale[i]; break;
            case LOG : amplitude_out[i] = 10 * log10f(temp / SAMPLES_SIZE); break;
            default: break;
        }

        if (amplitude_out[i] > 32767.0) amplitude_out[i] = 32767.0;
        if (amplitude_out[i] < 0.0 or std::isinf(amplitude_out[i]) or std::isnan(amplitude_out[i]))
            amplitude_out[i] = 0.0;
    }
}

void FFTCore::calculate() {
    _calculate_target(samples.left, amplitudes.left);
    _calculate_target(samples.right, amplitudes.right);
}

void FFTCore::_generate_bark_scale(float frequencyStep) {
//    printf("[%lu] Bark scale generation\n", millis());
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (_cfg.frequency_step / 650);
        _bark_scale[i] = 7 * logf(base + sqrtf(1 + base * base));
    }
}

void FFTCore::_generate_custom_bark_scale(float frequency_step) {
//    printf("[%lu] Custom Bark scale generation\n", millis());
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = static_cast<float>(i) * (frequency_step / 3000);
        _bark_scale[i] = logf(base + sqrtf(1 + base * base));
    }
}


void FFTCore::_generate_window(WindowType type) {
    switch (type) {
        case BLACKMAN:          dsps_wind_blackman_f32(_fft_window, SAMPLES_SIZE);            break;
        case BLACKMAN_HARRIS:   dsps_wind_blackman_harris_f32(_fft_window, SAMPLES_SIZE);     break;
        case BLACKMAN_NUTTALL:  dsps_wind_blackman_nuttall_f32(_fft_window, SAMPLES_SIZE);    break;
        case FLAT_TOP:          dsps_wind_flat_top_f32(_fft_window, SAMPLES_SIZE);            break;
        case HANN:              dsps_wind_hann_f32(_fft_window, SAMPLES_SIZE);                break;
        case NUTTALL:           dsps_wind_nuttall_f32(_fft_window, SAMPLES_SIZE);             break;
        default: break;
    }
}