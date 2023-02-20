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
            SERIAL_PORT_INTERACTION_TASK_STACK_SIZE,
            this,
            SERIAL_PORT_INTERACTION_TASK_PRIORITY,
            &handleSendTask);
}

void SerialPortInteraction::stop() {
    if (handleSendTask == nullptr) return;
    vTaskDelete(handleSendTask);
    handleSendTask = nullptr;
}

[[noreturn]] void SerialPortInteraction::sendTask(void *thisPointer) {
    SerialPortInteraction &object = *(SerialPortInteraction*)thisPointer;
    while (true) {
        if (xTaskNotifyWait(0, 0, 0, portMAX_DELAY) != pdPASS) continue;
        Packet packet;
        xQueueReceive(object.packetQueue, &packet, 0);
        object.packetCobsSender(packet);
    }
}


void SerialPortInteraction::send(Packet &packet) {
    if (handleSendTask == nullptr) return;
    xQueueOverwrite(packetQueue, &packet);
    xTaskNotify(handleSendTask, 0, eSetValueWithOverwrite);
}

void SerialPortInteraction::sendAmplitudes(float *values, uint16_t length) {
    Packet packet(21, FLOAT32_T, length * sizeof(float), values);
    send(packet);
}

void SerialPortInteraction::sendAmplitudes(uint8_t *values, uint16_t length) {
    Packet packet(22, UINT8_T,  length, values);
    send(packet);
}

void SerialPortInteraction::packetCobsSender(Packet &packet) {
    uint8_t delimiter = 10;
    uint8_t debt = 255;
    auto *buffer = new uint8_t[debt];

    uint8_t code = 1;
    uint8_t *dst = buffer;
    uint8_t *code_ptr = dst++;

    auto *ptr = (uint8_t*)&packet;
    for (int i = 0; i < 4; ++i) {
        uint8_t character = *ptr++;
        if (character != delimiter) {
            *dst++ = character;
            code++;
        } else {
            if (delimiter != 0 && code == delimiter) code = 0;
            *code_ptr = code;
            code = 1;
            code_ptr = dst++;
        }
    }

    ptr = (uint8_t*)packet.dataPointer;
    for (uint16_t i = 0; i < packet.dataLength; ++i) {
        uint8_t character = *ptr++;
        if (character != delimiter && dst - buffer < debt) {
            *dst++ = character;
            code++;
        } else {
            if (dst - buffer == debt) { *ptr--; i--;};
            if (delimiter != 0 && code == delimiter) code = 0;
            *code_ptr = code;
            Serial.write(buffer, dst - buffer);
            code = 1;
            dst = buffer;
            code_ptr = dst++;
        }
    }
    *code_ptr = dst - code_ptr;
    Serial.write(buffer, dst - buffer);
    Serial.write(delimiter);
    delete[] buffer;
}
