//
// Created by Koba on 31.01.2023.
//

#include "colorMusicCore.h"

ColorMusic *actualColorMusic = nullptr;

ColorMusic::ColorMusic(CRGB *leds) {
    actualColorMusic = this;
    this->leds = leds;
    samplesFullness = 0;
    samples = new Samples;
    amplitudes = new Amplitudes;
    fastAmplitudes = new uint8_t[AMPLITUDES_SIZE];

    dsps_fft2r_init_fc32(nullptr, SAMPLES_SIZE); // todo do free memory
    setAmplitudesType(amplitudesType);

//  todo find optimal stack depth and cores. configMINIMAL_STACK_SIZE - not enough, 2048 - there may be many
    printf("minimal stack depth: %u\n", configMINIMAL_STACK_SIZE);
    xTaskCreate(ColorMusic::fftExecutor, "colorMusicFFT", 2048, nullptr, 10, &fftTask);
    xTaskCreate(ColorMusic::colorsExecutor, "colorMusicColorsExecutor", 2048, nullptr, 8, &colorsTask);
}

ColorMusic::~ColorMusic() {
    actualColorMusic = nullptr;
    delete samples;
    delete amplitudes;
    delete fastAmplitudes;
    delete barkScale;
    vTaskDelete(&fftTask);
}


void ColorMusic::addSamples(const uint8_t *data, uint32_t length) {
    printf("[%lu] addSamples | new samples\n", millis());
    length = length/4;
    auto frame = (Frame*)data;

    if (length == SAMPLES_SIZE) samplesFullness = 0;

    if (samplesFullness + length > SAMPLES_SIZE && length < SAMPLES_SIZE) {
        uint16_t countOfOutOfBounds =  samplesFullness + length - SAMPLES_SIZE;
        // Offset of the samples array by countOfOutOfBounds
        for (uint16_t i = countOfOutOfBounds; i < SAMPLES_SIZE; i++) {
            samples->left[i - countOfOutOfBounds] = samples->left[i];
            samples->right[i - countOfOutOfBounds] = samples->right[i];
        }
        // The new full value of the samples array.
        samplesFullness -= countOfOutOfBounds;
    }

    // Filling the array with new samples
    if (SAMPLES_SIZE >= length) {
        for (uint32_t i = 0; i < length; ++i) {
            samples->left[samplesFullness] = frame[i].channel1;
            samples->right[samplesFullness++] = frame[i].channel2;
        }
    } else {
        for (samplesFullness = 0; samplesFullness < SAMPLES_SIZE; samplesFullness++) {
            samples->left[SAMPLES_SIZE - 1 - samplesFullness] = frame[length - 1 - samplesFullness].channel1;
            samples->right[SAMPLES_SIZE - 1 - samplesFullness] = frame[length - 1 - samplesFullness].channel2;
        }
    }
    if (samplesFullness == SAMPLES_SIZE) xTaskNotify(actualColorMusic->fftTask, 0, eNoAction); // todo execute result
}

void ColorMusic::calcFFT(const int16_t *targetSamples, float *targetAmplitudes) {
    for (int i = 0; i < SAMPLES_SIZE; i++) {
//        if (useWindow)
//            buffer[i * 2 + 0] = (float) targetSamples[i] * fftData.fftWindow[i];
//        else
        buffer[i * 2 + 0] = (float) targetSamples[i];
        buffer[i * 2 + 1] = 0;
    }

    dsps_fft2r_fc32(buffer, SAMPLES_SIZE);
    dsps_bit_rev_fc32(buffer, SAMPLES_SIZE);
    dsps_cplx2reC_fc32(buffer, SAMPLES_SIZE);

    float temp;
    for (int i = 0 ; i < AMPLITUDES_SIZE ; i++) {
        temp = buffer[i * 2 + 0] * buffer[i * 2 + 0] + buffer[i * 2 + 1] * buffer[i * 2 + 1];
        if (amplitudesType == LIN || amplitudesType == BARK)
            targetAmplitudes[i] = 2 * sqrtf(temp)/SAMPLES_SIZE;
        switch(amplitudesType) {
            case BARK:
                if (barkScale == nullptr) generateBarkScaleTable();
                targetAmplitudes[i] *= barkScale[i];
                break;
            case LOG:
                targetAmplitudes[i] = 10 * log10f(temp/SAMPLES_SIZE);
                break;
            default: break;
        }

        if (targetAmplitudes[i] > 32767) targetAmplitudes[i] = 32767;
        if (targetAmplitudes[i] < 0 || isinf(targetAmplitudes[i]) || isnan(targetAmplitudes[i]))
            targetAmplitudes[i] = 0;
    }
}

void ColorMusic::generateBarkScaleTable() {
    printf("generate bark scale\n");
    if (barkScale == nullptr) barkScale = new float[AMPLITUDES_SIZE];
    float base;
    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
        base = (float) i * (frequencyStep/650);
        barkScale[i] = 7 * logf(base + sqrtf(1 + base * base));
    }
}

void ColorMusic::setAmplitudesType(AmplitudesType value) {
    // todo block repeat delete barkScale
    if (amplitudesType != value && value == BARK) generateBarkScaleTable();
    if (value != BARK && barkScale != nullptr) {
        printf("delete bark scale!\n");
        delete barkScale;
    }
    amplitudesType = value;
}

void ColorMusic::setSampleRate(uint16_t newSampleRate) {
    printf("sample rate new: %u \n", newSampleRate);
    float newFrequencyStep = 1/((float) SAMPLES_SIZE/ (float) newSampleRate);
    if (newFrequencyStep != frequencyStep && amplitudesType == BARK) generateBarkScaleTable();
    frequencyStep = newFrequencyStep;
    printf("set frequencyStep: %f \n", frequencyStep);
}

[[noreturn]] void ColorMusic::fftExecutor(void *pvParam) {
    while (true) {
        if (xTaskNotifyWait(0, 0, 0, portMAX_DELAY) != pdPASS) continue;
        if (actualColorMusic == nullptr) continue;
        printf("[%lu] fftExecutor | start calculating fft\n", millis());
        actualColorMusic->calcFFT(actualColorMusic->samples->left, actualColorMusic->amplitudes->left);
        actualColorMusic->calcFFT(actualColorMusic->samples->right, actualColorMusic->amplitudes->right);
        printf("[%lu] fftExecutor | end calculating fft\n", millis());
        xTaskNotify(actualColorMusic->colorsTask, 0, eNoAction); // todo execute result
    }
}

void calculateColors(const uint8_t *amplitudes, CRGB *leds, float freqStep) {
    uint8_t low = 0;
    uint8_t middle;
    uint8_t high;

//    auto startLowIndex = (uint16_t) (150/fftData.frequencyStep);
//    auto endLowIndex = (uint16_t) (150/fftData.frequencyStep);
    auto startMiddleIndex = (uint16_t) (600/freqStep);
    auto endMiddleIndex = (uint16_t) (1500/freqStep);
    auto startHighIndex = (uint16_t) (10000/freqStep);
    auto endHighIndex = (uint16_t) (20000/freqStep);

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

[[noreturn]] void ColorMusic::colorsExecutor(void *) {
    while (true) {
        if (xTaskNotifyWait(0, 0, 0, portMAX_DELAY) != pdPASS) continue;
        if (actualColorMusic == nullptr) continue;
        printf("[%lu] colorsExecutor | get notification fft\n", millis());
        for (int i = 0; i < AMPLITUDES_SIZE; ++i) {
            actualColorMusic->fastAmplitudes[i] = ((int16_t) actualColorMusic->amplitudes->left[i]) >> 6;
        }
        printf("[%lu] colorsExecutor | start calc colors and show\n", millis());
        calculateColors(actualColorMusic->fastAmplitudes, actualColorMusic->leds, actualColorMusic->frequencyStep);
        printf("[%lu] colorsExecutor | end calc colors and show\n", millis());
    }
}

void ColorMusic::callbackAddSamples(const uint8_t *data, uint32_t length) {
    if (actualColorMusic != nullptr) actualColorMusic->addSamples(data, length);
}

void ColorMusic::callbackUpdateSampleRate(uint16_t newSampleRate) {
    if (actualColorMusic != nullptr) actualColorMusic->setSampleRate(newSampleRate);
}





//void generateCustomBarkScaleTable(FFTData &fft) {
//    float base;
//    for (int i = 0; i < AMPLITUDES_SIZE; i++) {
//        base = (float) i * (fft.frequencyStep/3000);
//        fft.barkScale[i] = logf(base + sqrtf(1 + base * base));
//    }
//}
//

//int32_t getAmplitudeSignal(const int16_t *samples) {
//    int16_t minValue = 0;
//    int16_t maxValue = 0;
//    for (int i = 0; i < SAMPLES_SIZE; i++) {
//        if (samples[i] > maxValue) maxValue = samples[i];
//        if (samples[i] < minValue) minValue = samples[i];
//    }
//    return maxValue + abs(minValue);
//}


