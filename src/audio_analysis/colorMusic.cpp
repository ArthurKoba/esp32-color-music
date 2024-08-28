#include "colorMusic.h"

ColorMusic::ColorMusic() {
    fftConfig.amplitudesType = BARK;
    fftConfig.windowType = NUTTALL;
}

ColorMusic::~ColorMusic() {
    if (!a2dp) return;
    a2dp->set_raw_stream_reader(nullptr, nullptr);
    a2dp->set_sample_rate_callback(nullptr, nullptr);
    disable();
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
    if (this->fft) return;
    printf("[%lu] ColorMusic enable.\n", millis());
//    printf("free memory after create: %i\n", ESP.getFreeHeap());
    samplesQueue = xQueueCreate(10, sizeof(SamplesBuffer));
    xTaskCreate(
            ColorMusic::showTask,
            "ColorMusicTask",
            COLOR_MUSIC_TASK_STACK_SIZE,
            this,
            COLOR_MUSIC_TASK_PRIORITY,
            &handleColorMusic);
    this->fft = new FFTColorMusic(fftConfig);
}

void ColorMusic::disable() {
    printf("[%lu] ColorMusic disable.\n", millis());
    vTaskDelete(handleColorMusic);
    delete fft;
    fft = nullptr;
    SamplesBuffer buffer {};
    while (uxQueueMessagesWaiting(samplesQueue)) {
        xQueueReceive(samplesQueue, &buffer, 0);
        delete [] buffer.data;
    }
    vQueueDelete(samplesQueue);

//    printf("free memory after delete: %i\n", ESP.getFreeHeap());
}

void ColorMusic::showTask(void *pvParam) {
    ColorMusic &t = *reinterpret_cast<ColorMusic *>(pvParam);
    SamplesBuffer buffer {};
    while (true) {
        if (xQueuePeek(t.samplesQueue, &buffer, portMAX_DELAY) != pdTRUE) continue;
        uint32_t length = buffer.length >> 1;
        t.fft->addSamples(buffer.data, length);
        t.fft->calculate();
        for (int i = 0; i < AMPLITUDES_SIZE >> 2; ++i) {
            t.amplitudes[i] = t.fft->amplitudes.left[i];
        }
        t.show();
        t.fft->addSamples(buffer.data + length, length);
        t.fft->calculate();
        for (int i = 0; i < AMPLITUDES_SIZE; ++i) {
            t.amplitudes[i] = t.fft->amplitudes.left[i];
        }
        t.show();
        xQueueReceive(t.samplesQueue, &buffer, 0); // Get first buffer from queue
        delete[] buffer.data; // free memory buffer.data
    }
}

void ColorMusic::setSampleRate(uint16_t sampleRate, void *thisPointer) {
    ColorMusic &object = *(ColorMusic*)thisPointer;
    printf("[%lu] new sample rate: %u \n", millis(), sampleRate);
    object.fftConfig.frequencyStep = float(sampleRate) / float(SAMPLES_SIZE);
    if (object.fft != nullptr) object.setConfigFFT(object.fftConfig);
}

void ColorMusic::addSamples(const uint8_t *data, uint32_t length, void *thisPointer) {
    ColorMusic &self = *(ColorMusic*)thisPointer;
    if (not self.fft or not uxQueueSpacesAvailable(self.samplesQueue)) return;
    SamplesBuffer buffer {.data = new uint8_t[length], .length = length};
    for (int i = 0; i < length; ++i) buffer.data[i] = data[i];
    xQueueSend(self.samplesQueue, &buffer, 0);
}

FFTConfig ColorMusic::getConfigFFT() {
    return fftConfig;
}

void ColorMusic::setConfigFFT(FFTConfig &config) {
    if (fft != nullptr) fft->setConfigs(config);
    fftConfig = config;
}
