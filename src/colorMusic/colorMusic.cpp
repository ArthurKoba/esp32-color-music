#include "colorMusic.h"

ColorMusic::ColorMusic() {

}

//void ColorMusic::setBluetoothA2DPSink(BluetoothA2DPSink *a2dpPointer) {
//    a2dp = a2dpPointer;
//}

void ColorMusic::enable() {
    printf("ColorMusic enable.\n");
    this->fft = new FFTColorMusic(fftConfig);
//    printf("free memory after create: %i\n", ESP.getFreeHeap());
}

void ColorMusic::disable() {
    printf("ColorMusic disable.\n");
    delete this->fft;
    fft = nullptr;
//    printf("free memory after delete: %i\n", ESP.getFreeHeap());
}

void ColorMusic::show() {
//    vTaskDelay(5000);
}

void ColorMusic::setSampleRate(uint16_t sampleRate, ColorMusic *thisPointer) {
    printf("sample rate new: %u \n", sampleRate);
    thisPointer->fftConfig.frequencyStep = 1/((float) SAMPLES_SIZE/ (float) sampleRate);
    printf("set frequencyStep: %f \n", thisPointer->fftConfig.frequencyStep);
    if (thisPointer->fft != nullptr) thisPointer->setConfigFFT(thisPointer->fftConfig);
}

void ColorMusic::addSamples(const uint8_t *data, uint32_t length, ColorMusic *thisPointer) {
    if (thisPointer->fft != nullptr) thisPointer->fft->addSamples(data, length);
}

FFTConfig ColorMusic::getConfigFFT() {
    return fftConfig;
}

void ColorMusic::setConfigFFT(FFTConfig &config) {
    if (fft != nullptr) fft->setConfigs(config);
    fftConfig = config;
}
