void execIrCommandTest() {
    if (!IrReceiver.decode()) return;
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.println(String(command));
    switch (command) {
        case 0:
            Serial.println("USE WINDOW TRUE");
            fftData.useWindow = true;
            break;
        case 1:
            Serial.println("USE WINDOW FALSE");
            fftData.useWindow = false;
            break;
        case OFF_BUTTON:    colorModes.mode = COLOR_MODE;           break;
        case ON_BUTTON:     colorModes.mode = COLOR_MUSIC_MODE;     break;
        case RED_BUTTON:
            Serial.println("LIN EXIT");
            fftData.sendType = LIN;
            break;
        case GREEN_BUTTON:
            Serial.println("BARK EXIT");
            fftData.sendType = BARK;
            break;
        case BLUE_BUTTON:
            Serial.println("LOG EXIT");
            fftData.sendType = LOG;
            break;
        case WHITE_BUTTON:
            Serial.println("WINDOW EXIT");
            fftData.sendType = WINDOW;
            break;
        case 8:
            Serial.println("AREA 1-256");
            fftArea = 0;
            break;
        case 9:
            Serial.println("AREA 256-512");
            fftArea = 1;
            break;
        case 10:
            Serial.println("AREA 512-768");
            fftArea = 2;
            break;
        case FLASH_BUTTON:
            Serial.println("AREA 768-1024");
            fftArea = 3;
            break;
        case 12:
            Serial.println("WINDOW dsps_wind_blackman_f32");
            dsps_wind_blackman_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 13:
            Serial.println("WINDOW dsps_wind_blackman_harris_f32");
            dsps_wind_blackman_harris_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 14:
            Serial.println("WINDOW dsps_wind_blackman_nuttall_f32");
            dsps_wind_blackman_nuttall_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case STROBE_BUTTON:
            Serial.println("WINDOW dsps_wind_flat_top_f32");
            dsps_wind_flat_top_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 16:
            Serial.println("WINDOW dsps_wind_hann_f32");
            dsps_wind_hann_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 17:
            Serial.println("WINDOW dsps_wind_nuttall_f32");
            dsps_wind_nuttall_f32(fftData.fftWindow, SAMPLES_SIZE);
            break;
        case 18:
            Serial.println("BARK EXIT");
            fftData.sendType = BARK_SCALE;
            break;
        case FADE_BUTTON:
            Serial.println("OFF SHOW");
            fftData.sendType = OFF_SHOW;
            break;
        case 20:
            Serial.println("SEND FULL AMPL TRUE");
            sendFullAmplitudes = true;
            break;
        case 21:
            Serial.println("SEND FULL AMPL FALSE");
            sendFullAmplitudes = false;
            break;
        case 22:
            Serial.println("USE DIVISION");
            useDivision = true;
            break;
        case SMOOTH_BUTTON:
            Serial.println("UN USE DIVISION");
            useDivision = false;
            break;

    }
    IrReceiver.resume();
}