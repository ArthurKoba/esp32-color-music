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
            COLOR_MUSIC_TASK_STACK_SIZE,
            this,
            COLOR_MUSIC_TASK_PRIORITY,
            &handleColorMusic);
    this->fft = new FFTColorMusic(fftConfig);
}

void ColorMusic::disable() {
    printf("[%lu] ColorMusic disable.\n", millis());
    delete this->fft;
    fft = nullptr;
    vTaskDelete(handleColorMusic);
//    printf("free memory after delete: %i\n", ESP.getFreeHeap());
}

void ColorMusic::showTask(void *thisPointer) {
    ColorMusic &self = *(ColorMusic*)thisPointer;
    while (true) {
        if (xTaskNotifyWait(0, 0, 0, portMAX_DELAY) != pdPASS || self.fft->samples.fullness != SAMPLES_SIZE) continue;
        self.fft->calculate();
        self.show();
//        if (self.serialPortInteraction != nullptr) {
//            self.serialPortInteraction->sendAmplitudes(self.fft->amplitudes.left, AMPLITUDES_SIZE);
//        }
    }
}

void ColorMusic::setSampleRate(uint16_t sampleRate, void *thisPointer) {
    ColorMusic &object = *(ColorMusic*)thisPointer;
    printf("[%lu] new sample rate: %u \n", millis(), sampleRate);
    object.fftConfig.frequencyStep = 1/((float) SAMPLES_SIZE/ (float) sampleRate);
    if (object.fft != nullptr) object.setConfigFFT(object.fftConfig);
}

void ColorMusic::addSamples(const uint8_t *data, uint32_t length, void *thisPointer) {
    ColorMusic &self = *(ColorMusic*)thisPointer;
    if (self.fft == nullptr) return;
    self.fft->addSamples(data, length);
    xTaskNotify(self.handleColorMusic, 0, eNoAction);
}

FFTConfig ColorMusic::getConfigFFT() {
    return fftConfig;
}

void ColorMusic::setConfigFFT(FFTConfig &config) {
    if (fft != nullptr) fft->setConfigs(config);
    fftConfig = config;
}

void ColorMusic::show() {

    ChannelBright leftChannelBright = calculateBrightFromChannel(fft->amplitudes.left);
    ChannelBright rightChannelBright = calculateBrightFromChannel(fft->amplitudes.right);

    int index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            switch (i) {
                case 0:
                    strip.leds[index].setRGB(leftChannelBright.low, 0, 0);
                    strip.leds[229-index].setRGB(rightChannelBright.low, 0, 0);
                    break;
                case 1:
                    strip.leds[index].setRGB(0, leftChannelBright.middle, 0);
                    strip.leds[229-index].setRGB(0, rightChannelBright.middle, 0);
                    break;
                case 2:
                    strip.leds[index].setRGB(0, 0, leftChannelBright.high);
                    strip.leds[229-index].setRGB(0, 0, rightChannelBright.high);
                    break;
            }
            index += 1;
        }
    }
    for (int i = 28; i < 115; i++) strip.leds[i-1] = strip.leds[i];
    for (int i = 202; i > 114; i--) strip.leds[i] = strip.leds[i-1];
    strip.leds[114].setRGB(leftChannelBright.low, leftChannelBright.middle, leftChannelBright.high);
    strip.leds[115].setRGB(rightChannelBright.low, rightChannelBright.middle, rightChannelBright.high);
    strip.show();
}

ChannelBright ColorMusic::calculateBrightFromChannel(const float *channel) const {
    ChannelBright bright;
    auto startLowIndex = (uint16_t) (340/fftConfig.frequencyStep);
    auto endLowIndex = (uint16_t) (400/fftConfig.frequencyStep);
    auto startMiddleIndex = (uint16_t) (600/fftConfig.frequencyStep);
    auto endMiddleIndex = (uint16_t) (1000/fftConfig.frequencyStep);
    auto startHighIndex = (uint16_t) (10000/fftConfig.frequencyStep);
    auto endHighIndex = (uint16_t) (20000/fftConfig.frequencyStep);


    uint32_t sum = 0;

//    for (auto i = startLowIndex; i < endLowIndex; ++i) {
//        uint8_t targetLow = (uint16_t) channel[i] >> 6;
//        if (targetLow > bright.low) bright.low = targetLow;
//    }

    bright.low = (uint16_t) channel[6] >> 7;

    sum = 0;
    for (auto i = startMiddleIndex; i < endMiddleIndex; ++i) {
        sum += (uint16_t) channel[i] >> 6;
    }
    bright.middle = sum/(endMiddleIndex - startMiddleIndex);

    sum = 0;
    for (auto i = startHighIndex; i < endHighIndex; ++i) {
        sum += (uint16_t) channel[i] >> 6;
    }
    bright.high = sum/(endHighIndex - startHighIndex);


//    bright.low = getBrightCRT(bright.low);

    // test crt correction for dynamic green leds
    bright.middle = bright.middle > 0 ? (1 + (uint16_t)(bright.middle * bright.middle + 255)) >> 8 : 0;;
    bright.high = ((uint16_t)bright.high) * 2;

    return bright;
}
