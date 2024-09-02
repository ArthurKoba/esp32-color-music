#include "led_controller.h"
#include "FastLED.h"

LedController::LedController() {
    CFastLED::addLeds<WS2812B, WS2812B_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
}

LedController::~LedController() {
    if (handle_show_task) vTaskDelete(handle_show_task);
}

void LedController::start() {
    if (handle_show_task) return;
    xTaskCreate(
            [] (void *context) {static_cast<LedController*>(context)->show_task();},
            "LedController_show_task",
            LED_CONTROLLER_TASK_STACK_SIZE,
            this,
            LED_CONTROLLER_TASK_PRIORITY,
            &handle_show_task
    );
}

void LedController::show_color(CRGB color) {
    color_need_show = color;
    xTaskNotify(handle_show_task, SET_COLOR, eSetValueWithOverwrite);
}

void LedController::show() {
    xTaskNotify(handle_show_task, SET_LEDS, eSetValueWithOverwrite);
}

void LedController::add_segment(LedSegment segment) {
    segments.push_back(segment);
}

size_t LedController::number_of_segments() {
    return segments.size();
}

const LedSegment *LedController::get_segment(size_t index) {
    if (index >= segments.size()) return nullptr;
    return &segments[index];
}

void LedController::set_color_to_led(size_t index, CRGB color) {
    if (index >= NUM_LEDS) return;
    leds[index] = color;
}

void LedController::fill_leds(CRGB color, size_t start_led, size_t end_led) {
    if (end_led > NUM_LEDS) end_led = NUM_LEDS;
    for (size_t i = start_led; i < end_led; ++i) leds[i] = color;
}

void LedController::set_color_to_segment(uint8_t segment_id, CRGB color) {
    if (segment_id >= segments.size()) return;
    fill_leds(color, segments[segment_id].start, segments[segment_id].end + 1);
}

void LedController::move_colors(size_t start_led, size_t end_led) {
    if (start_led > NUM_LEDS or end_led > NUM_LEDS) return;
    for (size_t i = end_led; i > start_led; --i) leds[i] = leds[i - 1];
    for (size_t i = end_led; i < start_led + 1; ++i) leds[i] = leds[i + 1];
}

uint8_t LedController::get_brightness() const {
    return bright;
}

void LedController::set_brightness(uint8_t bright_) {
    bright = bright_;
    FastLED.setBrightness(bright);
}

void LedController::change_brightness(float value) {
    value += static_cast<float>(bright);
    value = value > 255 ? 255 : value < 0 ? 0 : value;
    set_brightness(static_cast<uint8_t>(value));
}

void LedController::show_task() {
    static NotificationShowType notificationType;
    while (true) {
        if (xTaskNotifyWait(0, 0, reinterpret_cast<uint32_t*>(&notificationType), portMAX_DELAY) != pdPASS) continue;
        switch (notificationType) {
            case SET_COLOR: FastLED.showColor(color_need_show);    break;
            case SET_LEDS:  FastLED.show();                        break;
        }
    }
}

size_t LedController::number_of_leds() {
    return NUM_LEDS;
}

uint8_t LedController::crt(uint8_t value) {
    return value > 0 ? (1 + static_cast<uint16_t>(value * value + 255)) >> 8 : 0;
}