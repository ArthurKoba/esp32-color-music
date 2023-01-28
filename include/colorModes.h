#include <FastLED.h>

#ifndef FLASH_MODE_DELAY_MS
    #define FLASH_MODE_DELAY_MS 200
#endif

#ifndef FADE_MODE_DELAY_MS
    #define FADE_MODE_DELAY_MS 5
#endif
#ifndef FADE_MODE_LOW_LEVEL
    #define FADE_MODE_LOW_LEVEL 5
#endif
#ifndef FADE_MODE_MAX_LEVEL
    #define FADE_MODE_MAX_LEVEL 255
#endif

#ifndef STROBE_MODE_DELAY_MS
    #define STROBE_MODE_DELAY_MS 50
#endif

#ifndef RAINBOW_MODE_DELAY_MS
    #define RAINBOW_MODE_DELAY_MS 2
#endif

#ifndef SMOOTH_MODE_DELAY_MS
    #define SMOOTH_MODE_DELAY_MS 10
#endif
#ifndef SMOOTH_MODE_LOW_LEVEL
    #define SMOOTH_MODE_LOW_LEVEL 10
#endif
#ifndef SMOOTH_MODE_MAX_LEVEL
    #define SMOOTH_MODE_MAX_LEVEL 255
#endif

#ifndef DEFAULT_MODE
    #define DEFAULT_MODE SMOOTH_MODE
#endif

#ifndef DEFAULT_BRIGHT
    #define DEFAULT_BRIGHT 255
#endif

enum ColorMode {
    OFF_MODE, COLOR_MUSIC_MODE, COLOR_MODE,
    FLASH_MODE, STROBE_MODE, FADE_MODE, SMOOTH_MODE
};

struct FlashModeData    {uint8_t delayMs; uint8_t index; bool dir;};
struct StrobeModeData   {uint8_t delayMs; bool activeState;};
struct FadeModeData     {uint8_t delayMs; uint8_t index; bool dir;};
struct SmoothModeData   {uint8_t delayMs; uint8_t index;};
struct RainbowModeData  {uint8_t delayMs; uint8_t index;};

struct ColorModes{
    uint8_t mode;
    uint8_t colorID;
    int16_t bright;
    struct FlashModeData flash;
    struct StrobeModeData strobe;
    struct FadeModeData fade;
    struct SmoothModeData smooth;
    struct RainbowModeData rainbow;
};


void setupColorModes(ColorModes &data)
// Функция инициализации структуры световых режимов.
{
    data.mode = DEFAULT_MODE; // Стандартный режим при запуске микроконтроллера.
    data.bright = DEFAULT_BRIGHT;
    data.flash.delayMs = FLASH_MODE_DELAY_MS;
    data.flash.index = 5;
    data.flash.dir = true;
    data.strobe.delayMs = STROBE_MODE_DELAY_MS;
    data.fade.delayMs = FADE_MODE_DELAY_MS;
    data.fade.index = (FADE_MODE_MAX_LEVEL - FADE_MODE_LOW_LEVEL) >> 1;
    data.smooth.delayMs = SMOOTH_MODE_DELAY_MS;
    data.rainbow.delayMs = RAINBOW_MODE_DELAY_MS;
}


void rainbowColorMode(CRGB *leds, RainbowModeData &data) {
    for (int i = 0; i < NUM_LEDS; i++) leds[i] = static_cast<CRGB>(CHSV(data.index - i, 255, 255));
    FastLED.show();
    data.index++;
    delay(data.delayMs);
}

void flashColorMode(FlashModeData &data)
// Режим двойных вспышек отдельных цветов.
// Порядок цветов красный -> зеленый -> синий -> циановый -> желтый -> фиолетовый и в обратном направлении.
// Между вспышками также появляются затемнения.
{
    switch (data.index) {
        case 1: case 3: case 5: case 7: case 9: case 11:
        case 13: case 15: case 17: case 19: case 21:
                          FastLED.showColor(CRGB::Black ); break;
        case  0: case  2: FastLED.showColor(CRGB::Red   ); break;
        case  4: case  6: FastLED.showColor(CRGB::Green ); break;
        case  8: case 10: FastLED.showColor(CRGB::Blue  ); break;
        case 12: case 14: FastLED.showColor(CRGB::Aqua  ); break;
        case 16: case 18: FastLED.showColor(CRGB::Yellow); break;
        case 20:          FastLED.showColor(CRGB::Purple); break;
    }

    if (data.dir) data.index++;
    else          data.index--;

    if (data.index == 20 || data.index == 0) data.dir = !data.dir;
    if (data.index > 20) data.index = 0;

    delay(data.delayMs);
}


void strobeColorMode(StrobeModeData &data) {
    if (data.activeState) FastLED.showColor(CRGB(255, 255, 255));
    else                  FastLED.showColor(CRGB(  0,   0,   0));
    data.activeState = !data.activeState;
    delay(data.delayMs);
}


void fadeColorMode(FadeModeData &data) {
    if (data.dir) data.index += 1;
    else          data.index -= 1;
    FastLED.showColor(CRGB(data.index, data.index, data.index));

    if (data.index >= FADE_MODE_MAX_LEVEL || data.index <= FADE_MODE_LOW_LEVEL) {
        data.dir = !data.dir; // Разворот направления.
    }
    delay(data.delayMs);
}


void smoothColorMode(SmoothModeData &data)
// Режим плавного (бесшовного) переливания цветов за счет индекса, который храниться в структуре режима.
// Так как индекс принимает значения от 0 до 255, а цвета три, диапазон индекса поделен на три и равен 85.
{
    if (data.index <= 85) { // Нарастание яркости красного канала и понижение яркости синего канала.
        FastLED.showColor(CRGB(
                map(data.index, 0, 85, SMOOTH_MODE_LOW_LEVEL, SMOOTH_MODE_MAX_LEVEL),
                SMOOTH_MODE_LOW_LEVEL,
                map(data.index, 0, 85, SMOOTH_MODE_MAX_LEVEL, SMOOTH_MODE_LOW_LEVEL)
        ));
    } else if (data.index <= 170) { // Нарастание яркости зеленого канала и понижение яркости красного канала.
        FastLED.showColor(CRGB(
                map(data.index, 86, 170, SMOOTH_MODE_MAX_LEVEL, SMOOTH_MODE_LOW_LEVEL),
                map(data.index, 86, 170, SMOOTH_MODE_LOW_LEVEL, SMOOTH_MODE_MAX_LEVEL),
                SMOOTH_MODE_LOW_LEVEL
        ));
    } else { // Нарастание яркости синего канала и понижение яркости зеленого канала.
        FastLED.showColor(CRGB(
                SMOOTH_MODE_LOW_LEVEL,
                map(data.index, 171, 255, SMOOTH_MODE_MAX_LEVEL, SMOOTH_MODE_LOW_LEVEL),
                map(data.index, 171, 255, SMOOTH_MODE_LOW_LEVEL, SMOOTH_MODE_MAX_LEVEL)
        ));
    }
    data.index++;
    delay(data.delayMs);
}
