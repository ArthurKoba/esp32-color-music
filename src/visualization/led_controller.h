#ifndef ESP32_COLOR_MUSIC_LED_CONTROLLER_H
#define ESP32_COLOR_MUSIC_LED_CONTROLLER_H

#include "configs.h"

#include <vector>
#include "stdint.h"
#include "RTOS.h"
#include "FastLED.h"

struct LedSegment final {
    uint16_t start;
    uint16_t end;
};


class LedController final {
    CRGB leds[NUM_LEDS]{};
    std::vector<LedSegment> segments;
    uint8_t bright = 255;

    CRGB color_need_show{};
    TaskHandle_t handle_show_task = nullptr;
    [[noreturn]] void show_task();

    enum NotificationShowType {SET_COLOR, SET_LEDS};

public:
    LedController();
    ~LedController();

    void start();
    void show_color(CRGB color);
    void show();

    uint8_t get_brightness() const;
    void set_brightness(uint8_t bright);
    void change_brightness(float changeValue);

    void add_segment(LedSegment segment);
    size_t number_of_segments();
    const LedSegment* get_segment(size_t index);

    void set_color_to_led(size_t index, CRGB color);
    size_t number_of_leds();
    void fill_leds(CRGB color, size_t start_led = 0, size_t end_led = NUM_LEDS);
    void set_color_to_segment(uint8_t segment_id, CRGB color);
    void move_colors(size_t start_led, size_t end_led);

    static uint8_t crt(uint8_t value);
};


#endif //ESP32_COLOR_MUSIC_LED_CONTROLLER_H
