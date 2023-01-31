#ifndef SAMPLES_SIZE
    #define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE SAMPLES_SIZE / 2

#include "esp_dsp.h"
#include "cmath"
#include "BluetoothA2DPSink.h"


enum AmplitudesType : uint8_t {
    LIN, BARK, LOG
};

struct Samples {
    int16_t left[SAMPLES_SIZE];
    int16_t right[SAMPLES_SIZE];
    uint16_t fullness;
};

struct Amplitudes {
    float left[AMPLITUDES_SIZE];
    float right[AMPLITUDES_SIZE];
};

struct FFTData {
    Samples samples;
    Amplitudes amplitudes;
    float barkScale[AMPLITUDES_SIZE];
    float fftWindow[SAMPLES_SIZE] __attribute__((aligned(16)));
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16)));
    bool useWindow;
    AmplitudesType amplitudesType;
    float frequencyStep;
};


FFTData fftData;


void calculateAmplitudes(const int16_t *samples, float *amplitudes) {
    auto *buffer = fftData.buffer;
    for (int i = 0; i < SAMPLES_SIZE; i++) {
        if (fftData.useWindow)
            buffer[i * 2 + 0] = (float) samples[i] * fftData.fftWindow[i];
        else
            buffer[i * 2 + 0] = (float) samples[i];
        buffer[i * 2 + 1] = 0;
    }

    dsps_fft2r_fc32(buffer, SAMPLES_SIZE);
    dsps_bit_rev_fc32(buffer, SAMPLES_SIZE);
    dsps_cplx2reC_fc32(buffer, SAMPLES_SIZE);

    float temp;
    for (int i = 0 ; i < AMPLITUDES_SIZE ; i++) {
        temp = buffer[i * 2 + 0] * buffer[i * 2 + 0] + buffer[i * 2 + 1] * buffer[i * 2 + 1];
        if (fftData.amplitudesType == LIN || fftData.amplitudesType == BARK)
            amplitudes[i] = 2 * sqrtf(temp)/SAMPLES_SIZE;
        switch (fftData.amplitudesType) {
            case LOG:   amplitudes[i] = 10 * log10f(temp/SAMPLES_SIZE); break;
            case BARK:  amplitudes[i] *= fftData.barkScale[i];          break;
            default:                                                    break;
        }
        if (amplitudes[i] > 32767) amplitudes[i] = 32767;
        if (amplitudes[i] < 0 || isinf(amplitudes[i]) || isnan(amplitudes[i])) amplitudes[i] = 0;
    }
}


void generateBarkScaleTable(FFTData &fft) {
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (fft.frequencyStep/650);
        fft.barkScale[i] = 7 * logf(base + sqrtf(1 + base * base));
    }
//    for (int i = 1; i < AMPLITUDES_SIZE; i++) fft.barkScale[i] /= fft.barkScale[AMPLITUDES_SIZE-1];
}


void generateCustomBarkScaleTable(FFTData &fft) {
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (fft.frequencyStep/3000);
        fft.barkScale[i] = logf(base + sqrtf(1 + base * base));
    }
}

void callbackSampleRate(uint16_t sampleRate) {
    Serial.println("Sample rate: " + String(sampleRate));
    float frequencyStep = 1/((float) SAMPLES_SIZE/ (float) sampleRate);
    Serial.println("frequencyStep: " + String(frequencyStep));
    if (frequencyStep != fftData.frequencyStep) {
        fftData.frequencyStep = frequencyStep;
//        generateCustomBarkScaleTable(fftData);
        generateBarkScaleTable(fftData);
    }
}

void setupColorMusic(FFTData &fft) {
    fft.samples.fullness = 0;
    fft.useWindow = true;
    fft.amplitudesType = BARK;
    dsps_wind_flat_top_f32(fft.fftWindow, SAMPLES_SIZE);
//    dsps_wind_hann_f32(fft.fftWindow, SAMPLES_SIZE);
    dsps_fft2r_init_fc32(nullptr, SAMPLES_SIZE);
}


void callbackAppendSamples(const uint8_t *data, uint32_t length) {
    length = length/4;
    auto frame = (Frame*)data;
    Samples &samples = fftData.samples;

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
}



int32_t getAmplitudeSignal(const int16_t *samples) {
    int16_t minValue = 0;
    int16_t maxValue = 0;
    for (int i = 0; i < SAMPLES_SIZE; i++) {
        if (samples[i] > maxValue) maxValue = samples[i];
        if (samples[i] < minValue) minValue = samples[i];
    }
    return maxValue + abs(minValue);
}