#ifndef ESP32_COLOR_MUSIC_DISPLAY_H
#define ESP32_COLOR_MUSIC_DISPLAY_H

#ifndef TFT_CS
#define TFT_CS          5
#endif
#ifndef TFT_DC
#define TFT_DC          4
#endif
#ifndef TFT_RST
#define TFT_RST         21
#endif
#ifndef TFT_LED
#define TFT_LED         2
#endif
#ifndef DISPLAY_ROTATE
#define DISPLAY_ROTATE  3
#endif

#ifndef TFT_SPI_FREQ
#define TFT_SPI_FREQ  80000000UL
#endif

#ifndef DISPLAY_TASK_STACK_SIZE
#define DISPLAY_TASK_STACK_SIZE 2048
#endif

#ifndef DISPLAY_TASK_PRIORITY
#define DISPLAY_TASK_PRIORITY 4
#endif

#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <control/SystemManager.h>

class Display {
public:
    explicit Display(SPIClass *spiPointer = nullptr);

    void initAndStart();
    void update();
    void setSystemManager(SystemManager *);

private:
    void initUI();
    void writeFPS();
    void writeSystemStats();

    [[noreturn]] static void displayTask(void *pvParam);
    TaskHandle_t displayTaskHandle = nullptr;
    SystemManager *sysManager = nullptr;
    SPIClass *spi = nullptr;
    Adafruit_ILI9341 *tft;
    uint16_t lastFPS = 0;
    uint8_t statsOffset = 25;


    struct {
        uint16_t bg = ILI9341_BLUE;
        uint16_t txt = ILI9341_WHITE;
        uint16_t line = ILI9341_RED;
    } c;
};

#endif //ESP32_COLOR_MUSIC_DISPLAY_H
