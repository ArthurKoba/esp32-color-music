//void execIrCommandTest(FFTColorMusic &colorMusicObj) {
//    if (!IrReceiver.decode()) return;
//    uint16_t command = IrReceiver.decodedIRData.command;
//    printf("IR command: %u\n", command);
//    switch (command) {
//        case BRIGHT_UP_BUTTON:
////            Serial.println("USE WINDOW TRUE");
////            fftData.useWindow = true;
//            break;
//        case BRIGHT_DOWN_BUTTON:
////            Serial.println("USE WINDOW FALSE");
////            fftData.useWindow = false;
//            break;
//        case OFF_BUTTON:
////            colorModes.mode = COLOR_MODE;
//            break;
//        case ON_BUTTON:
////            colorModes.mode = COLOR_MUSIC_MODE;
//            break;
//        case RED_BUTTON:
//            Serial.println("LIN EXIT");
//            colorMusicObj.setAmplitudesType(LIN);
//            break;
//        case GREEN_BUTTON:
//            Serial.println("BARK EXIT");
//            colorMusicObj.setAmplitudesType(BARK);
//            break;
//        case BLUE_BUTTON:
//            Serial.println("LOG EXIT");
//            colorMusicObj.setAmplitudesType(LOG);
//            break;
////        case WHITE_BUTTON:
////            Serial.println("WINDOW EXIT");
////            sendType = WINDOW;
////            break;
////        case 8:
////            Serial.println("AREA 1-256");
////            fftArea = 0;
////            break;
////        case 9:
////            Serial.println("AREA 256-512");
////            fftArea = 1;
////            break;
////        case 10:
////            Serial.println("AREA 512-768");
////            fftArea = 2;
////            break;
////        case FLASH_BUTTON:
////            Serial.println("AREA 768-1024");
////            fftArea = 3;
////            break;
////        case 12:
////            break;
////        case 13:
////            break;
////        case 14:
////            break;
////        case STROBE_BUTTON:
////
////            break;
////        case 16:
////
////            break;
//        case 17:
//            printf("NO_WINDOW\n");
//            colorMusicObj.setWindowType(NO_WINDOW);
//            break;
//        case 18:
//            printf("WINDOW dsps_wind_blackman_f32\n");
//            colorMusicObj.setWindowType(BLACKMAN);
//            break;
//        case FADE_BUTTON:
//            printf("WINDOW dsps_wind_blackman_harris_f32\n");
//            colorMusicObj.setWindowType(BLACKMAN_HARRIS);
//            break;
//        case 20:
//            printf("WINDOW dsps_wind_blackman_nuttall_f32\n");
//            colorMusicObj.setWindowType(BLACKMAN_NUTTALL);
//            break;
//        case 21:
//            printf("WINDOW dsps_wind_flat_top_f32\n");
//            colorMusicObj.setWindowType(FLAT_TOP);
//            break;
//        case 22:
//            printf("WINDOW dsps_wind_hann_f32\n");
//            colorMusicObj.setWindowType(HANN);
//            break;
//        case SMOOTH_BUTTON:
//            printf("WINDOW dsps_wind_nuttall_f32\n");
//            colorMusicObj.setWindowType(NUTTALL);
//            break;
//    }
//}