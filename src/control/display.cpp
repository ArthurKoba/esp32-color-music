#include "display.h"

Display::Display(SPIClass *spiPointer) {
    spi = spiPointer;
    if (not spi) spi = new SPIClass(VSPI);
    tft = new Adafruit_ILI9341(spi, TFT_DC, TFT_CS, TFT_RST);
}

void Display::initAndStart() {
    tft->begin(TFT_SPI_FREQ);
    tft->setRotation(DISPLAY_ROTATE);
    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, 1);
    xTaskCreatePinnedToCore(
            Display::displayTask,
            "DisplayUpdater",
            DISPLAY_TASK_STACK_SIZE,
            this,
            DISPLAY_TASK_PRIORITY,
            &displayTaskHandle,
            0
    );
}

void Display::displayTask(void *pvParam) {
    Display &t = *reinterpret_cast<Display *>(pvParam);
    t.initUI();
    while (true) {
        t.update();
    }
}

void Display::initUI() {
    tft->fillScreen(c.bg);
    tft->setTextColor(c.txt, c.bg);
    tft->setCursor(0, 0);

    tft->println("FPS: ");
    tft->println("C 0: ");
    tft->println("C 1: ");
    tft->println("MEM: ");
}

void Display::update() {
    uint32_t timer = micros();
    writeFPS();
    writeSystemStats();
    vTaskDelay(1);
    lastFPS = 1000000 / (micros() - timer);
}

void Display::writeFPS() {
    tft->setCursor(statsOffset, 0);
    tft->println(lastFPS);
}

void Display::writeSystemStats() {
    if (not sysManager) return;
    tft->setCursor(0, 0);
    tft->println();
    tft->setCursor(statsOffset, tft->getCursorY());
    tft->println(sysManager->getLoadCpu0());
    tft->setCursor(statsOffset, tft->getCursorY());
    tft->println(sysManager->getLoadCpu1());
    tft->setCursor(statsOffset, tft->getCursorY());
    tft->println(sysManager->getMemoryUsage());
}

void Display::setSystemManager(SystemManager *manager_) {
    sysManager = manager_;
}









