#include "audio_analyzer.h"

AudioAnalyzer::AudioAnalyzer() {
    _fft_config.amplitudes_type = BARK;
    _fft_config.window_type = NUTTALL;
    _fft = new FFTCore(_fft_config);
    _samples_queue = xQueueCreate(10, sizeof(SamplesBuffer));
    xTaskCreate(
            [] (void *context) {static_cast<AudioAnalyzer*>(context)->_analyzer_task();},
            "ColorMusicTask",
            COLOR_MUSIC_TASK_STACK_SIZE,
            this,
            COLOR_MUSIC_TASK_PRIORITY,
            &_handle_audio_analyzer
    );
}

AudioAnalyzer::~AudioAnalyzer() {
    vTaskDelete(_handle_audio_analyzer);
    delete _fft;
}

ChannelsAudioInformation AudioAnalyzer::get_channels_info() {
    return _channels_info;
}

void AudioAnalyzer::add_samples(const uint8_t *data, uint32_t length) {
//    if (not _fft or not uxQueueSpacesAvailable(_samples_queue)) return;
    SamplesBuffer buffer {.data = new uint8_t[length], .length = length};
    for (int i = 0; i < length; ++i) buffer.data[i] = data[i];
    xQueueSend(_samples_queue, &buffer, 0);
}

void AudioAnalyzer::_analyzer_task() {
    SamplesBuffer buffer {};
    while (true) {
        if (xQueuePeek(_samples_queue, &buffer, portMAX_DELAY) != pdTRUE) continue;
        uint32_t length = buffer.length >> 1;
        _fft->add_samples(buffer.data, length);
        _fft->calculate();
        _calculate_audio_information();
//        Packet packet(1, 256, nullptr);
//        packet.create_buffer();
//        auto samples = reinterpret_cast<uint8_t*>(_fft->samples.left);
//        for (int i = 0; i < 256; i+=2) packet.data_ptr[i] = samples[i];
//        _bdsp_transmitter->send_packet(packet);

        _fft->add_samples(buffer.data + length, length);
        _fft->calculate();
        _calculate_audio_information();
        xQueueReceive(_samples_queue, &buffer, 0); // Get first buffer from queue
        delete[] buffer.data; // free memory buffer.data
    }
}

void AudioAnalyzer::_calculate_audio_information() {
    _channels_info.left = _calculate_audio_information_for_channel(_fft->amplitudes.left);
    _channels_info.right = _calculate_audio_information_for_channel(_fft->amplitudes.right);
}

AudioInformation AudioAnalyzer::_calculate_audio_information_for_channel(float *amplitudes) {
    AudioInformation info;
    uint32_t sum;

    auto start_low_index = static_cast<uint16_t>(340.0 / _fft_config.frequency_step);
    auto end_low_index = static_cast<uint16_t>(400 / _fft_config.frequency_step);
////    for (auto i = startLowIndex; i < endLowIndex; ++i) {
//        uint8_t targetLow = (uint16_t) channel[i] >> 6;
//        if (targetLow > bright.low) bright.low = targetLow;
//    }
    info.low = static_cast<uint16_t>(amplitudes[6]) >> 6;

    auto start_middle_index = static_cast<uint16_t>(600 / _fft_config.frequency_step);
    auto end_middle_index = static_cast<uint16_t>(1000 / _fft_config.frequency_step);
    sum = 0;
    for (auto i = start_middle_index; i < end_middle_index; ++i) {
        sum += static_cast<uint16_t>(amplitudes[i]) >> 6;
    }
    info.middle = sum / (end_middle_index - start_middle_index);


    auto start_high_index = static_cast<uint16_t>(10000 / _fft_config.frequency_step);
    auto end_high_index = static_cast<uint16_t>(20000 / _fft_config.frequency_step);
    sum = 0;
    for (auto i = start_high_index; i < end_high_index; ++i) {
        sum += (uint16_t) static_cast<uint16_t>(amplitudes[i]) >> 6;
    }
    info.high = sum / (end_high_index - start_high_index);

    return info;
}

void AudioAnalyzer::setup_callbacks(CustomBluetoothA2DPSink *a2dp) {
//void AudioAnalyzer::setup_callbacks(CustomBluetoothA2DPSink *a2dp, BDSPTransmitter *bdsp_transmitter_) {
    _a2dp = a2dp;
//    _bdsp_transmitter = bdsp_transmitter_;
    a2dp->set_raw_stream_reader_ctx([] (const uint8_t *data, uint32_t length, void *context) {
        static_cast<AudioAnalyzer*>(context)->add_samples(data, length);
    }, this);
    a2dp->set_sample_rate_callback_ctx([] (uint16_t sampleRate, void *context) {
        auto &self = *static_cast<AudioAnalyzer*>(context);
        self._fft_config.frequency_step = float(sampleRate) / float(SAMPLES_SIZE);
        if (self._fft not_eq nullptr) self._fft->set_configs(self._fft_config);
    }, this);
}