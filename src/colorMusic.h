

void calculateColors(const uint8_t *amplitudes) {
    uint8_t low = 0;
    uint8_t middle;
    uint8_t high;

//    auto startLowIndex = (uint16_t) (150/fftData.frequencyStep);
//    auto endLowIndex = (uint16_t) (150/fftData.frequencyStep);
    auto startMiddleIndex = (uint16_t) (600/fftData.frequencyStep);
    auto endMiddleIndex = (uint16_t) (1500/fftData.frequencyStep);
    auto startHighIndex = (uint16_t) (10000/fftData.frequencyStep);
    auto endHighIndex = (uint16_t) (20000/fftData.frequencyStep);

    low = amplitudes[5];
//    for (auto i = startLowIndex; i < endLowIndex; ++i) {
//        if (amplitudes[i] > low) low = amplitudes[i];
//    }

    uint32_t sum = 0;
    for (auto i = startMiddleIndex; i < endMiddleIndex; ++i) {
        sum += amplitudes[i];
    }
    middle = sum/(endMiddleIndex - startMiddleIndex);
    sum = 0;
    for (auto i = startHighIndex; i < endHighIndex; ++i) {
        sum += amplitudes[i];
    }
    high = sum/(endHighIndex - startHighIndex);

    int index = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 9; j++) {
            switch (i) {
                case 0:
                    leds[index].setRGB(low, 0, 0);
                    leds[229-index].setRGB(low, 0, 0);
                    break;
                case 1:
                    leds[index].setRGB(0, middle, 0);
                    leds[229-index].setRGB(0, middle, 0);
                    break;
                case 2:
                    leds[index].setRGB(0, 0, high);
                    leds[229-index].setRGB(0, 0, high);
                    break;
            }
            index += 1;
        }
    }

//    if ((millis() - timeLastShow) > 25) {
        for (int i = 28; i < 115; i++) leds[i-1] = leds[i];
        for (int i = 202; i > 114; i--) leds[i] = leds[i-1];
        leds[114].setRGB(low, middle, high);
        leds[115].setRGB(low, middle, high);
//        timeLastShow = millis();
//    }
    FastLED.show();
}