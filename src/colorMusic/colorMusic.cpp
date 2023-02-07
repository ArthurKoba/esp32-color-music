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

void ColorMusic::setSerialPortInteraction(SerialPortInteraction *serialPortInteractionPointer) {
    this->serialPortInteraction = serialPortInteractionPointer;
}


void ColorMusic::enable() {
    if (this->fft != nullptr) return;
    printf("[%lu] ColorMusic enable.\n", millis());
//    printf("free memory after create: %i\n", ESP.getFreeHeap());
    xTaskCreate(
            ColorMusic::showTask,
            "ColorMusicTask",
            2048,
            this,
            5,
            &handleColorMusic);
    this->fft = new FFTColorMusic(fftConfig, handleColorMusic);
}

void ColorMusic::disable() {
    printf("[%lu] ColorMusic disable.\n", millis());
    delete this->fft;
    fft = nullptr;
    vTaskDelete(handleColorMusic);
//    printf("free memory after delete: %i\n", ESP.getFreeHeap());
}

void ColorMusic::showTask(void *context) {
    ColorMusic &object = *(ColorMusic*)context;
    while (true) {
        if (xTaskNotifyWait(0, 0, 0, portMAX_DELAY) != pdPASS) continue;
        if (object.serialPortInteraction != nullptr)
            object.serialPortInteraction->sendAmplitudes(object.fft->amplitudes.left, AMPLITUDES_SIZE);
        for (int i = 0; i < AMPLITUDES_SIZE; ++i) {
            object.amplitudesLeft[i] = (uint16_t) object.fft->amplitudes.left[i] >> 6;
            object.amplitudesRight[i] = (uint16_t) object.fft->amplitudes.right[i] >> 6;
        }
        object.show();
    }
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

void ColorMusic::show() {

    uint8_t low = 0;
    uint8_t middle;
    uint8_t high;
    auto startLowIndex = (uint16_t) (150/fftConfig.frequencyStep);
    auto endLowIndex = (uint16_t) (150/fftConfig.frequencyStep);
    auto startMiddleIndex = (uint16_t) (600/fftConfig.frequencyStep);
    auto endMiddleIndex = (uint16_t) (1500/fftConfig.frequencyStep);
    auto startHighIndex = (uint16_t) (10000/fftConfig.frequencyStep);
    auto endHighIndex = (uint16_t) (20000/fftConfig.frequencyStep);

    low = amplitudesLeft[7];
//    for (auto i = startLowIndex; i < endLowIndex; ++i) {
//        if (amplitudes[i] > low) low = amplitudes[i];
//    }
//
    uint32_t sum = 0;
    for (auto i = startMiddleIndex; i < endMiddleIndex; ++i) {
        sum += amplitudesLeft[i];
    }
    middle = sum/(endMiddleIndex - startMiddleIndex);
    sum = 0;
    for (auto i = startHighIndex; i < endHighIndex; ++i) {
        sum += amplitudesLeft[i];
    }
    high = sum/(endHighIndex - startHighIndex);
//
    int index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            switch (i) {
                case 0:
                    strip.leds[index].setRGB(low, 0, 0);
                    strip.leds[229-index].setRGB(low, 0, 0);
                    break;
                case 1:
                    strip.leds[index].setRGB(0, middle, 0);
                    strip.leds[229-index].setRGB(0, middle, 0);
                    break;
                case 2:
                    strip.leds[index].setRGB(0, 0, high);
                    strip.leds[229-index].setRGB(0, 0, high);
                    break;
            }
            index += 1;
        }
    }
//
////    if ((millis() - timeLastShow) > 25) {
    for (int i = 28; i < 115; i++) strip.leds[i-1] = strip.leds[i];
    for (int i = 202; i > 114; i--) strip.leds[i] = strip.leds[i-1];
    strip.leds[114].setRGB(low, middle, high);
    strip.leds[115].setRGB(low, middle, high);
////        timeLastShow = millis();
////    }
    strip.show();
}
