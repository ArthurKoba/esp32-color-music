void sendDataType(String &dataType) {
  Serial.print(F("{\"dataType\":\""));
  Serial.print(dataType);
  Serial.print(F("\",\"data\":"));
}

void sendJsonArray(uint8_t *arr, const uint16_t &arrSize, String dataType = "null") {
  sendDataType(dataType);
  Serial.print("[" + String(arr[0]));
  for (uint16_t i = 1; i < arrSize;) Serial.print("," + String(arr[i++]));
  Serial.println("]}");
}

void sendJsonArray(int16_t *arr, const uint16_t arrSize, String dataType = "null") {
  sendDataType(dataType);
  Serial.print("[" + String(arr[0]));
  for (uint16_t i = 1; i < arrSize;) Serial.print("," + String(arr[i++]));
  Serial.println("]}");
}

void sendJsonArray(float *arr, const uint16_t arrSize, String dataType = "null") {
  sendDataType(dataType);
  Serial.print("[" + String(arr[0]));
  for (uint16_t i = 1; i < arrSize;) Serial.print("," + String(arr[i++]));
  Serial.println("]}");
}

void sendAmplitudesArea(uint8_t *arr, const uint16_t arrSize, String dataType = "null", uint8_t area = 0) {
    sendDataType(dataType);
    uint16_t startIndex, stopIndex;
    switch (area) {
        case 1:
            startIndex = arrSize/4;
            stopIndex = arrSize/2;
            break;
        case 2:
            startIndex = arrSize/2;
            stopIndex = arrSize*3/4;
            break;
        case 3:
            startIndex = arrSize*3/4;
            stopIndex = arrSize;
            break;
        case 0:
        default:
            startIndex = 0;
            stopIndex = arrSize/4;
    }

    Serial.print("[" + String(arr[startIndex]));
    for (uint16_t i = startIndex + 1; i < stopIndex;) Serial.print("," + String(arr[i++]));
    Serial.println("]}");
}


// void sendSamples(SamplingConfigs &sampling) {
//   sendJsonArray(sampling.samples, sampling.samplesLength, "samplesData");
// }

// void sendAmplitudes(FHTData &fhtData) {
//   sendJsonArray(fhtData.amplitudes, fhtData.amplitudesLength, "fhtData");
// }
