#ifndef ESP32_COLOR_MUSIC_COLORMUSIC_H
#define ESP32_COLOR_MUSIC_COLORMUSIC_H

#include "stdio.h"
//#include "BluetoothA2DPSink.h"
//#include "fft.h"

class ColorMusic {
public:
    ColorMusic();
//    void setBluetoothA2DPSink(BluetoothA2DPSink *a2dp);
    void enable();
    void disable();
    void show();
private:
//    BluetoothA2DPSink *a2dp;
};

#endif //ESP32_COLOR_MUSIC_COLORMUSIC_H
