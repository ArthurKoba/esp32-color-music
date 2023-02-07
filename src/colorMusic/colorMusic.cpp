#include "colorMusic.h"

ColorMusic::ColorMusic(CustomLedStrip &targetStrip) : strip(targetStrip) {
    fftConfig.amplitudesType = BARK;
    fftConfig.windowType = NUTTALL;
}

ColorMusic::~ColorMusic() {
    if (a2dp == nullptr) return;
    a2dp->set_raw_stream_reader(nullptr, nullptr);
    a2dp->set_sample_rate_callback(nullptr, nullptr);
}

void ColorMusic::setupCallbacks(CustomBluetoothA2DPSink *a2dpPointer) {
    this->a2dp = a2dpPointer;
    a2dp->set_raw_stream_reader(ColorMusic::addSamples, this);
    a2dp->set_sample_rate_callback(ColorMusic::setSampleRate, this);
//    a2dp_sink.set_on_volumechange(change_volume);
//    a2dp_sink.set_on_audio_state_changed_post(callbackOnChangeAudioState);
//    a2dp_sink.set_on_connection_state_changed(callbackChangeConnectionState);
//    a2dp_sink.set_avrc_metadata_callback(callbackAVRCMetadata);
}

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

void ColorMusic::setSampleRate(uint16_t sampleRate, void *thisPointer) {
    ColorMusic &object = *(ColorMusic*)thisPointer;
    printf("[%lu] new sample rate: %u \n", millis(), sampleRate);
    object.fftConfig.frequencyStep = 1/((float) SAMPLES_SIZE/ (float) sampleRate);
    if (object.fft != nullptr) object.setConfigFFT(object.fftConfig);
}

void ColorMusic::addSamples(const uint8_t *data, uint32_t length, void *thisPointer) {
    ColorMusic &object = *(ColorMusic*)thisPointer;
    if (object.fft != nullptr) object.fft->addSamples(data, length);
}

FFTConfig ColorMusic::getConfigFFT() {
    return fftConfig;
}

void ColorMusic::setConfigFFT(FFTConfig &config) {
    if (fft != nullptr) fft->setConfigs(config);
    fftConfig = config;
}
