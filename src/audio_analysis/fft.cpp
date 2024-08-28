#include "fft.h"

FFTColorMusic::FFTColorMusic(FFTConfig &config) {
    printf("[%lu] Constructor FFTColorMusic\n", millis());
    dsps_fft2r_init_fc32(fftTable, SAMPLES_SIZE);

    if (config.windowType != NO_WINDOW) {
        printf("[%lu] memory allocation for the window\n", millis());
        fftWindow = new float[SAMPLES_SIZE];
        generateWindow(config.windowType);
    }

    if (config.amplitudesType == BARK or config.amplitudesType == CUSTOM_BARK) {
        printf("[%lu] memory allocation for the bark scale\n", millis());
        barkScale = new float[AMPLITUDES_SIZE];
        if (config.amplitudesType == BARK)
            generateBarkScale(config.frequencyStep);
        else if (config.amplitudesType == CUSTOM_BARK)
            generateCustomBarkScale(config.frequencyStep);
    }

    cfg = config;
}

FFTColorMusic::~FFTColorMusic() {
//    printf("[%lu] Destructor FFTColorMusic\n", millis());
    delete[] fftWindow;
    delete[] barkScale;
}

void FFTColorMusic::setConfigs(FFTConfig &newCfg) {
    // todo need testing
    bool isNeedWindow = newCfg.windowType != NO_WINDOW;
    bool isWindowGenerated = cfg.windowType != NO_WINDOW;

    if (isNeedWindow and not fftWindow) this->fftWindow = new float[SAMPLES_SIZE];
    else if (not isNeedWindow and fftWindow) {
        printf("[%lu] freeing window memory\n", millis());
        delete[] fftWindow;
        fftWindow = nullptr;
    }
    if (isNeedWindow and newCfg.windowType != cfg.windowType) generateWindow(newCfg.windowType);

    bool isNeedBarkScale = newCfg.amplitudesType == BARK or newCfg.amplitudesType == CUSTOM_BARK;
    bool isNeedUpdateBarkScale = (cfg.amplitudesType != newCfg.amplitudesType or newCfg.frequencyStep != cfg.frequencyStep);

    if (isNeedBarkScale and not barkScale) {
//        printf("[%lu] memory allocation for the bark scale\n", millis());
        barkScale = new float[AMPLITUDES_SIZE];
    } else if (not isNeedBarkScale and barkScale) {
//        printf("[%lu] freeing window memory bark scale\n", millis());
        delete[] barkScale;
        barkScale = nullptr;
    }

    if (isNeedUpdateBarkScale and newCfg.amplitudesType == BARK) {
        generateBarkScale(newCfg.frequencyStep);
    } else if (isNeedUpdateBarkScale and newCfg.amplitudesType == CUSTOM_BARK) {
        generateCustomBarkScale(newCfg.frequencyStep);
    }

    cfg = newCfg;
}

void FFTColorMusic::addSamples(const uint8_t *data, uint32_t length) {
    length = length / 4;
    auto frame = (StereoFrame16*)data;

    if (length < SAMPLES_SIZE) {
        // offset with fill
        // 0,1,2,3,4 to 3,4,100,101,102 where 100,101,102 - data
        uint16_t countOfOutOfBounds = SAMPLES_SIZE - length;
        for (uint16_t i = 0; i < countOfOutOfBounds; i++) {
            samples.left[i] = samples.left[i + length];
            samples.right[i] = samples.right[i + length];
        }
        for (int i = 0; i < length; ++i) {
            samples.left[countOfOutOfBounds + i] = frame[i].channel1;
            samples.right[countOfOutOfBounds + i] = frame[i].channel2;
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
        uint16_t countOfOutOfBounds = length - SAMPLES_SIZE;
        for (int i = 0; i < SAMPLES_SIZE; ++i) {
            samples.left[i] = frame[countOfOutOfBounds + i].channel1;
            samples.right[i] = frame[countOfOutOfBounds + i].channel2;
        }
    }
}

void FFTColorMusic::calculateTarget(const int16_t *samplesIn, float *amplitudeOut) {
    // todo add mutex. maybe raise errors after change fft configs

    if (cfg.windowType == NO_WINDOW) {
        for (int i = 0; i < SAMPLES_SIZE; i++) {
            buffer[i * 2 + 0] = (float) samplesIn[i];
            buffer[i * 2 + 1] = 0;
        }
    } else {
        for (int i = 0; i < SAMPLES_SIZE; i++) {
            buffer[i * 2 + 0] = (float) samplesIn[i] * fftWindow[i];
            buffer[i * 2 + 1] = 0;
        }
    }

    dsps_fft2r_fc32(buffer, SAMPLES_SIZE);
    dsps_bit_rev_fc32(buffer, SAMPLES_SIZE);
    dsps_cplx2reC_fc32(buffer, SAMPLES_SIZE);

    float temp;
    for (int i = 0 ; i < AMPLITUDES_SIZE ; i++) {
        temp = buffer[i * 2 + 0] * buffer[i * 2 + 0] + buffer[i * 2 + 1] * buffer[i * 2 + 1];
        if (cfg.amplitudesType == LIN || cfg.amplitudesType == BARK)
            amplitudeOut[i] = 2 * sqrtf(temp)/SAMPLES_SIZE;
        switch(cfg.amplitudesType) {
            case BARK: amplitudeOut[i] *= barkScale[i]; break;
            case LOG : amplitudeOut[i] = 10 * log10f(temp/SAMPLES_SIZE); break;
            default: break;
        }

        if (amplitudeOut[i] > 32767.0) amplitudeOut[i] = 32767.0;
        if (amplitudeOut[i] < 0.0 or std::isinf(amplitudeOut[i]) or std::isnan(amplitudeOut[i]))
            amplitudeOut[i] = 0.0;
    }
}

void FFTColorMusic::calculate() {
    calculateTarget(samples.left, amplitudes.left);
    calculateTarget(samples.right, amplitudes.right);
}

void FFTColorMusic::generateBarkScale(float frequencyStep) {
//    printf("[%lu] Bark scale generation\n", millis());
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (frequencyStep / 650);
        barkScale[i] = 7 * logf(base + sqrtf(1 + base * base));
    }
}

void FFTColorMusic::generateCustomBarkScale(float frequencyStep) {
//    printf("[%lu] Custom Bark scale generation\n", millis());
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (frequencyStep / 3000);
        barkScale[i] = logf(base + sqrtf(1 + base * base));
    }
}


void FFTColorMusic::generateWindow(WindowType type) {
    switch (type) {
        case BLACKMAN:          dsps_wind_blackman_f32(fftWindow, SAMPLES_SIZE);            break;
        case BLACKMAN_HARRIS:   dsps_wind_blackman_harris_f32(fftWindow, SAMPLES_SIZE);     break;
        case BLACKMAN_NUTTALL:  dsps_wind_blackman_nuttall_f32(fftWindow, SAMPLES_SIZE);    break;
        case FLAT_TOP:          dsps_wind_flat_top_f32(fftWindow, SAMPLES_SIZE);            break;
        case HANN:              dsps_wind_hann_f32(fftWindow, SAMPLES_SIZE);                break;
        case NUTTALL:           dsps_wind_nuttall_f32(fftWindow, SAMPLES_SIZE);             break;
        default: break;
    }
}