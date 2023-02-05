
#include "remoteControl.h"
#include <IRRemote.hpp>

RemoteControl::RemoteControl() {
    this->colorModes = nullptr;
}

void RemoteControl::setup() {
    IrReceiver.begin(IR_RECEIVE_PIN);
}

void RemoteControl::handleCommands() {
    handleIR();
}

void RemoteControl::handleIRCommand(IRButton command) {
    printf("Exec command: %i\n", command);
}

void RemoteControl::handleIR() {
    if (!IrReceiver.decode()) return;
    if (IrReceiver.decodedIRData.command != 0 || IrReceiver.decodedIRData.decodedRawData == -16716032) {
        if (IrReceiver.decodedIRData.command > 23) return;

        handleIRCommand((IRButton)IrReceiver.decodedIRData.command);
    }
    IrReceiver.resume();
}

void RemoteControl::setColorModes(ColorModes *colorModesObject) {
    this->colorModes = colorModesObject;
}
