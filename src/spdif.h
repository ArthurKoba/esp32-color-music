#ifndef ESP32_COLOR_MUSIC_SPDIF_H
#define ESP32_COLOR_MUSIC_SPDIF_H

#include <BluetoothA2DP.h>
#include <hal/gpio_types.h>

#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(4, 4, 0)
#error Unsupported ESP IDF Version. Need 4.4.0 and up.
#endif

#define BITS_PER_SUB_FRAME 64
#define FRAMES_PER_BLOCK 192
#define SPDIF_BUF_SIZE (BITS_PER_SUB_FRAME / 8 * 2 * FRAMES_PER_BLOCK)
#define SPDIF_BUF_ARRAY_SIZE (SPDIF_BUF_SIZE / sizeof(uint32_t))

struct SPDIFConfigs {
    gpio_num_t pin = GPIO_NUM_NC;
    i2s_port_t i2s_port = I2S_NUM_0;
    uint32_t task_stack_size = 4096;
    UBaseType_t task_priority = 10;
};

class SPDIFOutput final : public BluetoothA2DPOutput {
public:

    ~SPDIFOutput();

    void set_output_active(bool active) override;

    void set_configs(SPDIFConfigs cfg);

    bool begin() override;

    size_t write(const uint8_t *data, size_t len) override;

    void end() override;

    void set_sample_rate(int rate) override;

protected:
    SPDIFConfigs _cfg;
    uint32_t _spdif_buf[SPDIF_BUF_ARRAY_SIZE];
    uint32_t *_spdif_current_ptr;
    RingbufHandle_t _ring_buffer;
    TaskHandle_t *_spdif_task = nullptr;

    [[noreturn]] void task();
};


#endif //ESP32_COLOR_MUSIC_SPDIF_H
