#include "fft.h"

FFTColorMusic::FFTColorMusic(FFTConfig &config, TaskHandle_t &handleEnd) : handleEndCalculate(handleEnd) {
    printf("[%lu] Constructor FFTColorMusic\n", millis());
    fftTable = new float[SAMPLES_SIZE];
    dsps_fft2r_init_fc32(fftTable, SAMPLES_SIZE);
    samples.fullness = 0;

    if (config.windowType != NO_WINDOW) {
        printf("[%lu] memory allocation for the window\n", millis());
        this->fftWindow = new float[SAMPLES_SIZE];
        generateWindow(config.windowType);
    }

    if (config.amplitudesType == BARK || config.amplitudesType == CUSTOM_BARK) {
        printf("[%lu] memory allocation for the bark scale\n", millis());
        this->barkScale = new float[AMPLITUDES_SIZE];
        if (config.amplitudesType == BARK)
            generateBarkScale(config.frequencyStep);
        else if (config.amplitudesType == CUSTOM_BARK)
            generateCustomBarkScale(config.frequencyStep);
    }

    cfg = config;

    xTaskCreate(FFTColorMusic::fftExecutor,
                "FFTcolorMusic",
                FFT_TASK_STACK_SIZE,
                this,
                FFT_TASK_PRIORITY,
                &handleFFTTask);
}

FFTColorMusic::~FFTColorMusic() {
    printf("[%lu] Destructor FFTColorMusic\n", millis());
    vTaskDelete(handleFFTTask);
    if (cfg.windowType != NO_WINDOW) {
        printf("[%lu] freeing window memory\n", millis());
        delete fftWindow;
    }
    if (cfg.amplitudesType == BARK || cfg.amplitudesType == CUSTOM_BARK) {
        printf("[%lu] freeing window memory bark scale\n", millis());
        delete barkScale;
    }
    delete fftTable;
}

void FFTColorMusic::setConfigs(FFTConfig &newCfg) {
    bool isNeedWindow = newCfg.windowType != NO_WINDOW;
    bool isWindowsGenerated = cfg.windowType != NO_WINDOW;

    if (newCfg.windowType != cfg.windowType) {
        printf("[%lu] change window type from %i to %i\n", millis(), cfg.windowType, newCfg.windowType);
    }
    if (isNeedWindow && !isWindowsGenerated) {
        printf("[%lu] memory allocation for the window\n", millis());
        this->fftWindow = new float[SAMPLES_SIZE];

    } else if (isWindowsGenerated && !isNeedWindow) {
        printf("[%lu] freeing window memory\n", millis());
        delete fftWindow;
    }
    if (isNeedWindow && newCfg.windowType != cfg.windowType) {
        generateWindow(newCfg.windowType);
    }

    bool isNeedBarkScale = newCfg.amplitudesType == BARK || newCfg.amplitudesType == CUSTOM_BARK;
    bool isBarkScaleGenerated = cfg.amplitudesType == BARK || cfg.amplitudesType == CUSTOM_BARK;
    bool isNeedUpdateBarkScale = (cfg.amplitudesType != newCfg.amplitudesType || newCfg.frequencyStep != cfg.frequencyStep);

    if (newCfg.amplitudesType != cfg.amplitudesType) {
        printf("[%lu] changing the type of amplitudes from %i to %i\n", millis(), cfg.amplitudesType, newCfg.amplitudesType);
    }

    if (isBarkScaleGenerated && !isNeedBarkScale) {
        printf("[%lu] freeing window memory bark scale\n", millis());
        delete barkScale;
    } else if (isNeedBarkScale && !isBarkScaleGenerated) {
        printf("[%lu] memory allocation for the bark scale\n", millis());
        this->barkScale = new float[AMPLITUDES_SIZE];
    }
    if (isNeedUpdateBarkScale && newCfg.amplitudesType == BARK) {
        generateBarkScale(newCfg.frequencyStep);
    } else if (isNeedUpdateBarkScale && newCfg.amplitudesType == CUSTOM_BARK) {
        generateCustomBarkScale(newCfg.frequencyStep);
    }
    cfg = newCfg;
}


[[noreturn]] void FFTColorMusic::fftExecutor(void *thisPointer) {
    FFTColorMusic &object = *(FFTColorMusic*)thisPointer;
    printf("[%lu] Start FFT Executor\n", millis());
    while (true) {
        if (xTaskNotifyWait(0, 0, 0, portMAX_DELAY) != pdPASS) continue;
//        printf("[%lu] fftExecutor | start calculating fft\n", millis());
        object.calcFFT(object.samples.left, object.amplitudes.left);
        object.calcFFT(object.samples.right, object.amplitudes.right);
//        printf("[%lu] fftExecutor | end calculating fft\n", millis());
        xTaskNotify(object.handleEndCalculate, 0, eNoAction);
    }
}



void FFTColorMusic::addSamples(const uint8_t *data, uint32_t length) {
    length = length/4;
    auto frame = (Frame*)data;

    if (length == SAMPLES_SIZE) samples.fullness = 0;

    if (samples.fullness + length > SAMPLES_SIZE && length < SAMPLES_SIZE) {
        uint16_t countOfOutOfBounds =  samples.fullness + length - SAMPLES_SIZE;
        // Offset of the samples array by countOfOutOfBounds
        for (uint16_t i = countOfOutOfBounds; i < SAMPLES_SIZE; i++) {
            samples.left[i - countOfOutOfBounds] = samples.left[i];
            samples.right[i - countOfOutOfBounds] = samples.right[i];
        }
        // The new full value of the samples array.
        samples.fullness -= countOfOutOfBounds;
    }

    // Filling the array with new samples
    if (SAMPLES_SIZE >= length) {
        for (uint32_t i = 0; i < length; ++i) {
            samples.left[samples.fullness] = frame[i].channel1;
            samples.right[samples.fullness++] = frame[i].channel2;
        }
    } else {
        for (samples.fullness = 0; samples.fullness < SAMPLES_SIZE; samples.fullness++) {
            samples.left[SAMPLES_SIZE - 1 - samples.fullness] = frame[length - 1 - samples.fullness].channel1;
            samples.right[SAMPLES_SIZE - 1 - samples.fullness] = frame[length - 1 - samples.fullness].channel2;
        }
    }
    if (samples.fullness == SAMPLES_SIZE) xTaskNotify(handleFFTTask, 0, eNoAction);
}

void FFTColorMusic::calcFFT(const int16_t *samplesIn, float *amplitudeOut) {
    // todo add mutex. maybe raise errors after change fft configs
    for (int i = 0; i < SAMPLES_SIZE; i++) {
        if (cfg.windowType != NO_WINDOW)
            buffer[i * 2 + 0] = (float) samplesIn[i] * fftWindow[i];
        else
            buffer[i * 2 + 0] = (float) samplesIn[i];
        buffer[i * 2 + 1] = 0;
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
            case BARK:
                amplitudeOut[i] *= barkScale[i];
                break;
            case LOG:
                amplitudeOut[i] = 10 * log10f(temp/SAMPLES_SIZE);
                break;
            default: break;
        }

        if (amplitudeOut[i] > 32767) amplitudeOut[i] = 32767.0;
        if (amplitudeOut[i] < 0 || isinf(amplitudeOut[i]) || isnan(amplitudeOut[i]))
            amplitudeOut[i] = 0.0;
    }
}

void FFTColorMusic::generateBarkScale(float frequencyStep) {
    printf("[%lu] Bark scale generation\n", millis());
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (frequencyStep/650);
        barkScale[i] = 7 * logf(base + sqrtf(1 + base * base));
    }
}

void FFTColorMusic::generateCustomBarkScale(float frequencyStep) {
    printf("[%lu] Custom Bark scale generation\n", millis());
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (frequencyStep / 3000);
        barkScale[i] = logf(base + sqrtf(1 + base * base));
    }
}


void FFTColorMusic::generateWindow(WindowType type) {
    switch (type) {
        case NO_WINDOW:
            break;
        case BLACKMAN:
            dsps_wind_blackman_f32(fftWindow, SAMPLES_SIZE);
            break;
        case BLACKMAN_HARRIS:
            dsps_wind_blackman_harris_f32(fftWindow, SAMPLES_SIZE);
            break;
        case BLACKMAN_NUTTALL:
            dsps_wind_blackman_nuttall_f32(fftWindow, SAMPLES_SIZE);
            break;
        case FLAT_TOP:
            dsps_wind_flat_top_f32(fftWindow, SAMPLES_SIZE);
            break;
        case HANN:
            dsps_wind_hann_f32(fftWindow, SAMPLES_SIZE);
            break;
        case NUTTALL:
            dsps_wind_nuttall_f32(fftWindow, SAMPLES_SIZE);
            break;
    }
}

uint32_t FFTColorMusic::getMaxObjectSize() {
    // MAX Size = sizeObject + fftTable(float*SAMPLES_SIZE) + window(float*SAMPLES_SIZE) + bark(float*AMPLITUDES_SIZE)
    // todo add stack size task and another. +- 57.1k size when SAMPLES_SIZE = 2048
    uint32_t sizeObject = sizeof(FFTColorMusic) + sizeof(float) * AMPLITUDES_SIZE * (1 + 2 + 2);
    return sizeObject;
}

uint16_t FFTColorMusic::getDeltaMinMaxSample(const int16_t *samples) {
    int16_t minValue = 0;
    int16_t maxValue = 0;
    for (int i = 0; i < SAMPLES_SIZE; i++) {
        if (samples[i] > maxValue) maxValue = samples[i];
        if (samples[i] < minValue) minValue = samples[i];
    }
    return maxValue + abs(minValue);
}



