#include "SystemManager.h"


void SystemManager::update(void *pvParam) {
    SystemManager &t = *reinterpret_cast<SystemManager *>(pvParam);
    float maxIdleCalls = 132000.0;
    while (true) {
        auto idle0 = float(SystemManagerNameSpace::idle0Calls_);
        auto idle1 = float(SystemManagerNameSpace::idle1Calls_);
        if (idle0 > maxIdleCalls) maxIdleCalls = idle0;
        if (idle1 > maxIdleCalls) maxIdleCalls = idle1;
        SystemManagerNameSpace::idle0Calls_ = SystemManagerNameSpace::idle1Calls_ = 0;
        t.cpuLoad0 = (maxIdleCalls - idle0)/ maxIdleCalls;
        t.cpuLoad1 = (maxIdleCalls - idle1)/ maxIdleCalls;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void SystemManager::init() {
    heapSize = float(ESP.getHeapSize());
    if (taskHandle) return;
    cpuFreq = ESP.getCpuFreqMHz();
    esp_register_freertos_idle_hook_for_cpu(SystemManager::idle_task_0, 0);
    esp_register_freertos_idle_hook_for_cpu(SystemManager::idle_task_1, 1);
    xTaskCreatePinnedToCore(
            SystemManager::update,
            "SystemManager",
            SYSTEM_MANAGER_STACK_SIZE,
            this,
            SYSTEM_MANAGER_TASK_PRIORITY,
            &taskHandle,
            0
    );
}

void SystemManager::deInit() {
    esp_deregister_freertos_idle_hook_for_cpu(SystemManager::idle_task_0, 0);
    esp_deregister_freertos_idle_hook_for_cpu(SystemManager::idle_task_1, 1);
    vTaskDelete(&taskHandle);
    taskHandle = nullptr;
}

bool SystemManager::idle_task_0() {
    SystemManagerNameSpace::idle0Calls_ += 1;
    return false;
}

bool SystemManager::idle_task_1() {
    SystemManagerNameSpace::idle1Calls_ += 1;
    return false;
}

uint8_t SystemManager::getLoadCpu0() const {
    return uint8_t(round(cpuLoad0 * 100.f));
}

uint8_t SystemManager::getLoadCpu1() const {
    return uint8_t(round(cpuLoad1 * 100.f));
}

uint8_t SystemManager::getMemoryUsage() const {
    return uint8_t(round((heapSize - float(ESP.getFreeHeap())) * 100.f / heapSize));
}


