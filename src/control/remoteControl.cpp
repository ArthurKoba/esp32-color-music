#include "remoteControl.h"
#include <IRRemote.hpp>

RemoteControl::RemoteControl() {
    this->colorModes = nullptr;
    this->strip = nullptr;
}

void RemoteControl::setup() {
    IrReceiver.begin(IR_RECEIVE_PIN);
}

void RemoteControl::handleCommands() {
    handleIR();
}

void RemoteControl::handleIRCommand(IRButton command) {
    printf("Exec command: %i\n", command);
    switch (command) {
        case BRIGHT_UP_BUTTON:
            if (strip != nullptr) strip->changeBrightness(+5);
            break;
        case BRIGHT_DOWN_BUTTON:
            if (strip != nullptr) strip->changeBrightness(-5);
            break;
        case OFF_BUTTON:
            if (colorModes != nullptr) colorModes->setMode(OFF_MODE);
            break;
        case ON_BUTTON:
            if (colorModes != nullptr) colorModes->setMode(COLOR_MUSIC_MODE);
            break;
        case RED_BUTTON:
            break;
        case GREEN_BUTTON:
            break;
        case BLUE_BUTTON:
            break;
        case WHITE_BUTTON:
            break;
        case ORANGERED_BUTTON:
            break;
        case LIME_BUTTON:
            break;
        case VIOLET_BUTTON:
            break;
        case FLASH_BUTTON:
            break;
        case ORANGE_BUTTON:
            break;
        case CYAN_BUTTON:
            break;
        case PURPLE_BUTTON:
            break;
        case STROBE_BUTTON:
            break;
        case GOLD_BUTTON:
            break;
        case GREENCYAN_BUTTON:
            break;
        case ORCHID_BUTTON:
            break;
        case FADE_BUTTON:
            break;
        case YELLOW_BUTTON:
            break;
        case NAVY_BUTTON:
            break;
        case PINK_BUTTON:
            break;
        case SMOOTH_BUTTON:
            if (colorModes != nullptr) colorModes->setMode(RAINBOW_MODE);
            break;
    }
}

void RemoteControl::handleIR() {
    if (!IrReceiver.decode()) return;
    if (IrReceiver.decodedIRData.command != 0 || IrReceiver.decodedIRData.decodedRawData == -16716032) {
        if (IrReceiver.decodedIRData.command > 23) return;

        handleIRCommand((IRButton)IrReceiver.decodedIRData.command);
    }
    IrReceiver.resume();
}

void RemoteControl::setColorModes(ColorModes *colorModesPointer) {
    this->colorModes = colorModesPointer;
}

void RemoteControl::setLedStrip(LedStrip *stripPointer) {
    this->strip = stripPointer;
}

void RemoteControl::setColorMusic(ColorMusic *colorMusicPointer) {
    this->colorMusic = colorMusicPointer;
}

