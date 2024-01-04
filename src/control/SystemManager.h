#ifndef ESP32_COLOR_MUSIC_SYSTEMMANAGER_H
#define ESP32_COLOR_MUSIC_SYSTEMMANAGER_H

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Esp.h>
#include <esp_freertos_hooks.h>
#include <sdkconfig.h>

#ifndef SYSTEM_MANAGER_STACK_SIZE
#define SYSTEM_MANAGER_STACK_SIZE 2048
#endif

#ifndef SYSTEM_MANAGER_TASK_PRIORITY
#define SYSTEM_MANAGER_TASK_PRIORITY 20
#endif

namespace SystemManagerNameSpace {
    static uint64_t idle0Calls_ = 0;
    static uint64_t idle1Calls_ = 0;
}

struct CPULoad {
    uint8_t idle0 = 0;
    uint8_t idle1 = 0;
};

class SystemManager {
public:
    SystemManager() = default;

    void init();
    void deInit();
    uint8_t getLoadCpu0() const;
    uint8_t getLoadCpu1() const;
    uint8_t getMemoryUsage() const;

private:
    static bool idle_task_0();
    static bool idle_task_1();
    [[noreturn]] static void update(void *pvParam);
    TaskHandle_t taskHandle = nullptr;
    float heapSize = 0;
    uint16_t cpuFreq = 0;
    float cpuLoad0 = -1.0;
    float cpuLoad1 = -1.0;
    float freeHeap = -1.0;
};


#endif //ESP32_COLOR_MUSIC_SYSTEMMANAGER_H
