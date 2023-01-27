#ifndef SAMPLES_SIZE
    #define SAMPLES_SIZE 2048
#endif

#define AMPLITUDES_SIZE SAMPLES_SIZE / 1


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

struct FFT_DATA {
    Samples samples;
    Amplitudes amplitudes;
    bool useWindow;
    bool logarithmResult;
    float buffer[SAMPLES_SIZE * 2] __attribute__((aligned(16)));
    float fftWindow[SAMPLES_SIZE] __attribute__((aligned(16)));
};

FFT_DATA fftData;

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
        if (fftData.logarithmResult) amplitudes[i] = 10 * log10f(temp/SAMPLES_SIZE);
        else amplitudes[i] = 2 * sqrtf(temp)/SAMPLES_SIZE;
        if (amplitudes[i] < 1 || isnan(amplitudes[i])) amplitudes[i] = 0;
    }
}

void colorMusic() {
    if (fftData.samples.fullness != SAMPLES_SIZE) return;
//    sendJsonArray(fftData.samples.left, 256, "samples");
    uint16_t calcTime = millis();
    calculateAmplitudes(fftData.samples.right, fftData.amplitudes.right);
    calculateAmplitudes(fftData.samples.left, fftData.amplitudes.left);
    Serial.println("Calc time (ms): " + String(millis() - calcTime));
    calcTime = millis();
    sendJsonArray(fftData.amplitudes.left, 128, "test");
    Serial.println("Send time (ms): " + String(millis() - calcTime));
    fftData.samples.fullness = 0;
}

void setupColorMusic() {
    fftData.samples.fullness = 0;
    fftData.useWindow = true;
    fftData.logarithmResult = false;
    dsps_wind_hann_f32(fftData.fftWindow, SAMPLES_SIZE);
    dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
}

void appendSamples(const uint8_t *data, uint32_t length) {
    uint16_t samplesLength = length/4;
    auto frame = (Frame*)data;
//    for (int i = 0; i < samplesLength && fftData.samples.fullness < SAMPLES_SIZE; i++) {
//        fftData.samples.left[fftData.samples.fullness] = frame[i].channel1;
//        fftData.samples.right[fftData.samples.fullness] = frame[i].channel2;
//        fftData.samples.fullness++;
//    }
    for (int i = 0; i < samplesLength; i++) {
        fftData.samples.left[fftData.samples.fullness] = frame[i].channel1;
        fftData.samples.right[fftData.samples.fullness] = frame[i].channel2;
        fftData.samples.fullness++;
        if (fftData.samples.fullness == SAMPLES_SIZE) colorMusic();
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