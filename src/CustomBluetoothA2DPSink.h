#ifndef ESP32_COLOR_MUSIC_CUSTOMBLUETOOTHA2DPSINK_H
#define ESP32_COLOR_MUSIC_CUSTOMBLUETOOTHA2DPSINK_H

#include "BluetoothA2DPSink.h"

class CustomBluetoothA2DPSink : public BluetoothA2DPSink {
public:
    void set_raw_stream_reader(void (*callBack)(const uint8_t *, uint32_t, void *), void *context);
    void set_sample_rate_callback(void (*callback)(uint16_t rate, void *), void *context);
private:
    void *raw_stream_reader_context = nullptr;
    void *sample_rate_callback_context = nullptr;
    void (*raw_stream_reader)(const uint8_t*, uint32_t, void *) = nullptr;
    void (*sample_rate_callback)(const uint16_t, void *) = nullptr;
    void handle_audio_cfg(uint16_t event, void *p_param) override;
    void audio_data_callback(const uint8_t *data, uint32_t len) override;
};

#endif //ESP32_COLOR_MUSIC_CUSTOMBLUETOOTHA2DPSINK_H
