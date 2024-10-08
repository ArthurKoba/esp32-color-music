//#include "remoteControl.h"
//
//RemoteControl::RemoteControl() {
////    this->colorModes = nullptr;
////    this->colorMusic = nullptr;
//}
//
//void RemoteControl::setup() {
//
//}
//
//void RemoteControl::handleCommands() {
//    handleIR();
//}
//
//
//void RemoteControl::handleTestIRCommand(IRButton command) {
//    FFTConfig fftConfig;
//    if (colorMusic == nullptr && command > 3) return;
//    else fftConfig = colorMusic->getConfigFFT();
//
//    switch (command) {
//        case BRIGHT_UP_BUTTON:
////            if (strip != nullptr) strip->changeBrightness(+5);
//            break;
//        case BRIGHT_DOWN_BUTTON:
////            if (strip != nullptr) strip->changeBrightness(-5);
//            break;
//        case OFF_BUTTON:
//            if (colorModes != nullptr) colorModes->setMode(OFF_MODE);
//            break;
//        case ON_BUTTON:
//            if (colorModes != nullptr) colorModes->setMode(COLOR_MUSIC_MODE);
//            break;
//        case RED_BUTTON:
//                fftConfig.amplitudesType = LIN;
//            break;
//        case GREEN_BUTTON:
//            fftConfig.amplitudesType = LOG;
//            break;
//        case BLUE_BUTTON:
//            fftConfig.amplitudesType = BARK;
//            break;
//        case WHITE_BUTTON:
//            fftConfig.amplitudesType = CUSTOM_BARK;
//            break;
//        case ORANGERED_BUTTON:
//            fftConfig.windowType = NO_WINDOW;
//            break;
//        case LIME_BUTTON:
//            fftConfig.windowType = BLACKMAN;
//            break;
//        case VIOLET_BUTTON:
//            fftConfig.windowType = BLACKMAN_HARRIS;
//            break;
//        case FLASH_BUTTON:
//            fftConfig.windowType = BLACKMAN_NUTTALL;
//            break;
//        case ORANGE_BUTTON:
//            fftConfig.windowType = FLAT_TOP;
//            break;
//        case CYAN_BUTTON:
//            fftConfig.windowType = HANN;
//            break;
//        case PURPLE_BUTTON:
//            fftConfig.windowType = NUTTALL;
//            break;
//        case STROBE_BUTTON:
//            fftConfig.frequencyStep += 1.0;
//            break;
//        case GOLD_BUTTON:
//            break;
//        case GREENCYAN_BUTTON:
//            break;
//        case ORCHID_BUTTON:
//            break;
//        case FADE_BUTTON:
//            break;
//        case YELLOW_BUTTON:
//
//            break;
//        case NAVY_BUTTON:
//
//            break;
//        case PINK_BUTTON:
//            break;
//        case SMOOTH_BUTTON:
//            break;
//    }
//    if (command > 3) colorMusic->setConfigFFT(fftConfig);
//}
//
//void RemoteControl::handleIRCommand(IRButton command) {
//    printf("Exec command: %i\n", command);
//    handleTestIRCommand(command);
//    return;
//    switch (command) {
//        case BRIGHT_UP_BUTTON:
////            if (strip != nullptr) strip->changeBrightness(+5);
//            break;
//        case BRIGHT_DOWN_BUTTON:
////            if (strip != nullptr) strip->changeBrightness(-5);
//            break;
//        case OFF_BUTTON:
//            if (colorModes != nullptr) colorModes->setMode(OFF_MODE);
//            break;
//        case ON_BUTTON:
//            if (colorModes != nullptr) colorModes->setMode(COLOR_MUSIC_MODE);
//            break;
//        case RED_BUTTON:
//            break;
//        case GREEN_BUTTON:
//            break;
//        case BLUE_BUTTON:
//            break;
//        case WHITE_BUTTON:
//            break;
//        case ORANGERED_BUTTON:
//            break;
//        case LIME_BUTTON:
//            break;
//        case VIOLET_BUTTON:
//            break;
//        case FLASH_BUTTON:
//            break;
//        case ORANGE_BUTTON:
//            break;
//        case CYAN_BUTTON:
//            break;
//        case PURPLE_BUTTON:
//            break;
//        case STROBE_BUTTON:
//            break;
//        case GOLD_BUTTON:
//            break;
//        case GREENCYAN_BUTTON:
//            break;
//        case ORCHID_BUTTON:
//            break;
//        case FADE_BUTTON:
//            break;
//        case YELLOW_BUTTON:
//            break;
//        case NAVY_BUTTON:
//            break;
//        case PINK_BUTTON:
//            break;
//        case SMOOTH_BUTTON:
//            if (colorModes != nullptr) colorModes->setMode(RAINBOW_MODE);
//            break;
//    }
//}
//
//void RemoteControl::handleIR() {
//
//}
//
//void RemoteControl::setColorModes(ColorMode *colorModesPointer) {
//    this->colorModes = colorModesPointer;
//}
//
//void RemoteControl::setColorMusic(ColorMusic *colorMusicPointer) {
//    this->colorMusic = colorMusicPointer;
//}
