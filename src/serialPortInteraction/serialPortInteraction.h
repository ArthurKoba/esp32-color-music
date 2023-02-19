#ifndef ESP32_COLOR_MUSIC_SERIALPORTINTERACTION_H
#define ESP32_COLOR_MUSIC_SERIALPORTINTERACTION_H

#include <cstdint>
#include "Arduino.h"
#include "configs.h"

enum PacketDataType : uint8_t {
    NONE_T = 0, STRUCT_T = 1,
    INT8_T = 2, UINT8_T = 3,
    INT16_T = 4, UINT16_T = 5,
    INT32_T = 6, UINT32_T = 7,
    FLOAT32_T = 8, FLOAT64_T = 9, TEST = 10
};

struct Packet {
    Packet(uint8_t number, PacketDataType dataType, uint16_t dataLength, void *dataPointer) {
        this->number = number;
        this->dataType = dataType;
        this->dataLength = dataLength;
        this->dataPointer = dataPointer;
    }

    Packet() = default;

    uint8_t number = 0;
    PacketDataType dataType = NONE_T;
    uint16_t dataLength = 0;
    void *dataPointer = nullptr;
} __attribute__((packed));


class SerialPortInteraction {
public:
    SerialPortInteraction();
    ~SerialPortInteraction();
    void send(Packet &);
    void sendAmplitudes(float *values, uint16_t length);
    void sendAmplitudes(uint8_t *values, uint16_t length);
    void start();
    void stop();
private:
    void packetCobsSender(Packet &packet);
    [[noreturn]] void static sendTask(void *thisPointer);
    TaskHandle_t handleSendTask = nullptr;
    QueueHandle_t packetQueue = nullptr;
};

#endif //ESP32_COLOR_MUSIC_SERIALPORTINTERACTION_H
