#include "CustomBluetoothA2DPSink.h"

void
CustomBluetoothA2DPSink::set_raw_stream_reader_ctx(void (*callBack)(const uint8_t *, uint32_t, void *), void *context) {
    raw_stream_reader_ctx = callBack;
    raw_stream_reader_context = context;
}

void CustomBluetoothA2DPSink::set_sample_rate_callback_ctx(void (*callback)(uint16_t, void *), void *context) {
    sample_rate_callback_ctx = callback;
    sample_rate_callback_context = context;
}

void CustomBluetoothA2DPSink::audio_data_callback(const uint8_t *data, uint32_t len) {
    ESP_LOGD(BT_AV_TAG, "%s", __func__);

    // swap left and right channels
    if (swap_left_right) {
        Frame *frame = (Frame *) data;
        for (int i = 0; i < len / 4; i++) {
            int16_t temp = frame[i].channel1;
            frame[i].channel1 = frame[i].channel2;
            frame[i].channel2 = temp;
        }
    }

    // make data available via callback, before volume control
    if (raw_stream_reader != nullptr) {
        ESP_LOGD(BT_AV_TAG, "raw_stream_reader");
        (*raw_stream_reader)(data, len);
    }

    // adjust the volume
    volume_control()->update_audio_data((Frame *) data, len / 4);

    // make data available via callback
    if (stream_reader != nullptr) {
        ESP_LOGD(BT_AV_TAG, "stream_reader");
        (*stream_reader)(data, len);
    }

    if (raw_stream_reader_ctx) {
        ESP_LOGD(BT_AV_TAG, "stream_reader_ctx");
        (*raw_stream_reader_ctx)(data, len, raw_stream_reader_context);
    }

    // put data into ringbuffer
    if (is_output) {
        write_audio(data, len);
    }

    // data_received callback
    if (data_received != nullptr) {
        ESP_LOGD(BT_AV_TAG, "data_received");
        (*data_received)();
    }
}

void CustomBluetoothA2DPSink::handle_audio_cfg(uint16_t event, void *p_param) {
    ESP_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    esp_a2d_cb_param_t *a2d = (esp_a2d_cb_param_t *) (p_param);
    audio_type = a2d->audio_cfg.mcc.type;
    ESP_LOGI(BT_AV_TAG, "a2dp audio_cfg_cb , codec type %d",
             a2d->audio_cfg.mcc.type);

    // determine sample rate
    m_sample_rate = 16000;
    char oct0 = a2d->audio_cfg.mcc.cie.sbc[0];
    if (oct0 & (0x01 << 6)) {
        m_sample_rate = 32000;
    } else if (oct0 & (0x01 << 5)) {
        m_sample_rate = 44100;
    } else if (oct0 & (0x01 << 4)) {
        m_sample_rate = 48000;
    }
    ESP_LOGI(BT_AV_TAG, "a2dp audio_cfg_cb , sample_rate %d", m_sample_rate);
    if (sample_rate_callback != nullptr) {
        sample_rate_callback(m_sample_rate);
    }

    if (sample_rate_callback_ctx) {
        sample_rate_callback_ctx(m_sample_rate, sample_rate_callback_context);
    }

    // for now only SBC stream is supported
    if (a2d->audio_cfg.mcc.type == ESP_A2D_MCT_SBC) {
        ESP_LOGI(BT_AV_TAG, "configure audio player %x-%x-%x-%x\n",
                 a2d->audio_cfg.mcc.cie.sbc[0], a2d->audio_cfg.mcc.cie.sbc[1],
                 a2d->audio_cfg.mcc.cie.sbc[2], a2d->audio_cfg.mcc.cie.sbc[3]);

        out->set_sample_rate(m_sample_rate);
    }
}