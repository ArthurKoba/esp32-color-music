#include "serialPortInteraction.h"

SerialPortInteraction::SerialPortInteraction() {
    packetQueue = xQueueCreate(1, sizeof(Packet));
};

SerialPortInteraction::~SerialPortInteraction() {
    stop();
    vQueueDelete(packetQueue);
};

void SerialPortInteraction::start() {
    if (handleSendTask != nullptr) return;
    xTaskCreate(
            SerialPortInteraction::sendTask,
            "SerialPortInteraction",
            2048,
            this,
            4,
            &handleSendTask);
}

void SerialPortInteraction::stop() {
    if (handleSendTask == nullptr) return;
    vTaskDelete(handleSendTask);
    handleSendTask = nullptr;
}

[[noreturn]] void SerialPortInteraction::sendTask(void *thisPointer) {
    SerialPortInteraction &object = *(SerialPortInteraction*)thisPointer;
    uint32_t deltaTimeMs;
    while (true) {
        if (xTaskNotifyWait(0, 0, 0, portMAX_DELAY) != pdPASS) continue;
        Packet packet;
        xQueueReceive(object.packetQueue, &packet, 0);
        deltaTimeMs = millis();
        Serial.write((char*)&packet.type, 1);
        Serial.write((char*)&packet.length, 2);
        Serial.write((char*)packet.data, packet.length);
        deltaTimeMs = millis() - deltaTimeMs;
        auto lastTimeSend = (uint8_t) deltaTimeMs;
        Serial.write((char*)&lastTimeSend, 1);
        Serial.write('\n');
        Serial.write((char)170);
        Serial.write('\n');
    }
}


void SerialPortInteraction::send(Packet &packet) {
    xQueueOverwrite(packetQueue, &packet);
    xTaskNotify(handleSendTask, 0, eSetValueWithOverwrite);
}

void SerialPortInteraction::sendAmplitudes(float *values, uint16_t length) {
    Packet packet(FFT_FLOAT, length * sizeof(float), values);
    send(packet);
}

void SerialPortInteraction::sendAmplitudes(uint8_t *values, uint16_t length) {
    Packet packet(FFT_BYTE, length, values);
    send(packet);
}
