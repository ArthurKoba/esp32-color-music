#include "app.h"

App app;

void setup() {
    app.setup();
}

void loop() {
//    vTaskDelete(nullptr);
    app.tick();
}


