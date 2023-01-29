#ifndef SAMPLES_SIZE
    #define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE SAMPLES_SIZE / 2

#include "esp_dsp.h"
#include "cmath"
#include "BluetoothA2DPSink.h"

struct Samples {
    float left[SAMPLES_SIZE];
    float right[SAMPLES_SIZE];
    uint16_t fullness;
};

struct Amplitudes {
    float left[AMPLITUDES_SIZE];
    float right[AMPLITUDES_SIZE];
};

enum SEND_TYPE {
    LIN, BARK, LOG, WINDOW
};

struct FFTData {
    Samples samples;
    Amplitudes amplitudes;
    bool useWindow;
    uint8_t sendType;
    float frequencyStep;
    float barkScale[AMPLITUDES_SIZE];
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16)));
    float fftWindow[SAMPLES_SIZE] __attribute__((aligned(16)));
};


FFTData fftData;


void calculateAmplitudes(const float *samples, float *amplitudes) {
    float *buffer = fftData.buffer;
    for (int i = 0; i < SAMPLES_SIZE; i++) {
        buffer[i * 2 + 0] = fftData.useWindow ? samples[i] * fftData.fftWindow[i] : samples[i];
        buffer[i * 2 + 1] = 0;
    }

    dsps_fft2r_fc32(buffer, SAMPLES_SIZE);
    dsps_bit_rev_fc32(buffer, SAMPLES_SIZE);
    dsps_cplx2reC_fc32(buffer, SAMPLES_SIZE);

    float temp;
    for (int i = 0 ; i < AMPLITUDES_SIZE ; i++) {
        temp = buffer[i * 2 + 0] * buffer[i * 2 + 0] + buffer[i * 2 + 1] * buffer[i * 2 + 1];
        if (fftData.sendType == LIN || fftData.sendType == BARK)
            amplitudes[i] = 2 * sqrtf(temp)/SAMPLES_SIZE;
        switch (fftData.sendType) {
            case BARK: amplitudes[i] *= fftData.barkScale[i]; break;
            case LOG: amplitudes[i] = 10 * log10f(temp/SAMPLES_SIZE); break;
        }
        if (isinf(amplitudes[i])) amplitudes[i] = 70000000000.0;
        if (isnan(amplitudes[i])) amplitudes[i] = -70000000000.0;
    }
}


void generateBarkScaleTable(FFTData &fft) {
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (fft.frequencyStep/650);
        fft.barkScale[i] = 7 * logf(base + sqrtf(1 + base * base));
    }
    for (int i = 1; i < AMPLITUDES_SIZE; i++) fft.barkScale[i] /= fft.barkScale[AMPLITUDES_SIZE-1];
}


void setupColorMusic(FFTData &fft) {
    fft.samples.fullness = 0;
    fft.useWindow = true;
    fft.sendType = LIN;
    fft.frequencyStep = 1/((float)SAMPLES_SIZE/44100);
    dsps_wind_hann_f32(fft.fftWindow, SAMPLES_SIZE);
    dsps_fft2r_init_fc32(nullptr, SAMPLES_SIZE);
    generateBarkScaleTable(fft);
}


void appendSamples(const uint8_t *data, uint32_t length) {
    uint16_t samplesLength = length/4;
    auto frame = (Frame*)data;
    for (int i = 0; i < samplesLength && fftData.samples.fullness < SAMPLES_SIZE; i++) {
        fftData.samples.left[fftData.samples.fullness] = frame[i].channel1;
        fftData.samples.right[fftData.samples.fullness] = frame[i].channel2;
        fftData.samples.fullness++;
    }
}


float getAmplitudeSignal(const float *samples) {
    float minValue = 0.0;
    float maxValue = 0.0;
    for (int i = 0; i < SAMPLES_SIZE; i++) {
        if (samples[i] > maxValue) maxValue = samples[i];
        if (samples[i] < minValue) minValue = samples[i];
    }
    return maxValue + abs(minValue);
}