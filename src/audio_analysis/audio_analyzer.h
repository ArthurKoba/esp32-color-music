#ifndef ESP32_COLOR_MUSIC_AUDIO_ANALYZER_H
#define ESP32_COLOR_MUSIC_AUDIO_ANALYZER_H


#include <cstdint>
#include <BDSP.h>
#include "fft.h"
#include "CustomBluetoothA2DPSink.h"


struct AudioInformation {
    uint8_t low = 0;
    uint8_t middle = 0;
    uint8_t high = 0;
};

struct ChannelsAudioInformation {
    AudioInformation left;
    AudioInformation right;
};

struct SamplesBuffer{
    uint8_t *data;
    size_t length;
};

class AudioAnalyzer {
    ChannelsAudioInformation _channels_info;
    FFTConfig _fft_config;
    FFTCore *_fft{};
    QueueHandle_t _samples_queue = nullptr;
    TaskHandle_t _handle_audio_analyzer = nullptr;
    CustomBluetoothA2DPSink *_a2dp{};
    BDSPTransmitter *_bdsp_transmitter{};
public:
    AudioAnalyzer();
    ~AudioAnalyzer();
    void add_samples(const uint8_t *data, uint32_t length);
    void setup_callbacks(CustomBluetoothA2DPSink *a2dp, BDSPTransmitter *bdsp_transmitter);
    ChannelsAudioInformation get_channels_info();
private:
    [[noreturn]] void _analyzer_task();
    void _calculate_audio_information();
    AudioInformation _calculate_audio_information_for_channel(float *amplitudes);
};

#endif //ESP32_COLOR_MUSIC_AUDIO_ANALYZER_H
