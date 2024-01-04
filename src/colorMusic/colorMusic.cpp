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

void ColorMusic::setSerialPortInteraction(SerialPortInteraction *serialPortInteractionPointer) {
    this->serialPortInteraction = serialPortInteractionPointer;
}

void ColorMusic::setStrip(CustomLedStrip *strip_ptr) {
    strip = strip_ptr;
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
    object.fftConfig.frequencyStep = 1/((float) SAMPLES_SIZE/ (float) sampleRate);
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

void ColorMusic::show() {
    if (!fft || !strip) return;

    ChannelBright leftChannelBright = calculateBrightFromChannel(fft->amplitudes.left);
    ChannelBright rightChannelBright = calculateBrightFromChannel(fft->amplitudes.right);

    uint32_t time = millis();
    bool needUpdateRed = false;
    bool needUpdateGreen = false;
    bool needUpdateBlue = false;

    if (time - this->lastTimeRed > 14) {
        needUpdateRed = true;
        this->lastTimeRed = time;
    }

    if (time - this->lastTimeGreen > 11) {
        needUpdateGreen = true;
        this->lastTimeGreen = time;
    }

    if (time - this->lastTimeBlue > 5) {
        needUpdateBlue = true;
        this->lastTimeBlue = time;
    }

    strip->flowDown(needUpdateRed, needUpdateGreen, needUpdateBlue);

    strip->leds[114].setRGB(leftChannelBright.low, leftChannelBright.middle, leftChannelBright.high);
    strip->leds[115].setRGB(rightChannelBright.low, rightChannelBright.middle, rightChannelBright.high);

    BottomEQData data;
    data.red_left=leftChannelBright.low;
    data.green_left=leftChannelBright.middle;
    data.blue_left=leftChannelBright.high;
    data.red_right=rightChannelBright.low;
    data.green_right=rightChannelBright.middle;
    data.blue_right=rightChannelBright.high;
    strip->setBottomEQ(data);

    strip->show();

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

    bright.low = (uint16_t) channel[6] >> 6;

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
    bright.low = bright.low > 0 ? (1 + (uint16_t)(bright.low * bright.low + 255)) >> 8 : 0;

    // test crt correction for dynamic green leds
    bright.middle = bright.middle > 0 ? (1 + (uint16_t)(bright.middle * bright.middle + 255)) >> 8 : 0;
    bright.high = ((uint16_t)bright.high) * 2;

    if (bright.low > bright.middle && bright.low > bright.high) {
        bright.high *= 0.3;
        bright.middle *= 0.3;
    }

    if (bright.middle > bright.low && bright.middle > bright.high) {
        bright.low *= 0.3;
        bright.high *= 0.3;
    }

    if (bright.high > bright.low && bright.high > bright.middle) {
        bright.low *= 0.3;
        bright.middle *= 0.3;
    }

    return bright;
}
