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


// void sendSamples(SamplingConfigs &sampling) {
//   sendJsonArray(sampling.samples, sampling.samplesLength, "samplesData");
// }

// void sendAmplitudes(FHTData &fhtData) {
//   sendJsonArray(fhtData.amplitudes, fhtData.amplitudesLength, "fhtData");
// }
