
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

void RemoteControl::handleIRCommand(uint16_t command) {
    printf("Exec command: %i\n", command);
}

void RemoteControl::handleIR() {
    if (!IrReceiver.decode()) return;
    if (IrReceiver.decodedIRData.command != 0 || IrReceiver.decodedIRData.decodedRawData == -16716032) {
        handleIRCommand(IrReceiver.decodedIRData.command);
    }
    IrReceiver.resume();
}
