#ifndef ESP32_COLOR_MUSIC_SERIALPORTINTERACTION_H
#define ESP32_COLOR_MUSIC_SERIALPORTINTERACTION_H

#include <cstdint>
#include "Arduino.h"

enum PacketType : uint8_t {
    FFT_FLOAT = 21, FFT_BYTE = 22
};

struct Packet {

    Packet(PacketType type, uint16_t length, void *dataPointer) {
        this->type = type;
        this->length = length;
        this->data = dataPointer;
    }

    Packet() {
        type = FFT_BYTE;
        length = 0;
        data = nullptr;
    }

    PacketType type = FFT_BYTE;
    uint16_t length = 0;
    void *data = nullptr;
};

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
    [[noreturn]] void static sendTask(void *thisPointer);
    TaskHandle_t handleSendTask = nullptr;
    QueueHandle_t packetQueue = nullptr;
};

#endif //ESP32_COLOR_MUSIC_SERIALPORTINTERACTION_H
