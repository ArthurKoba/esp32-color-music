#if not defined(TEST_ENTRY)
#include "app.h"

App app;

void setup() {
    app.setup();
}

void loop() {
//    vTaskDelete(nullptr);
    app.tick();
}
#else
#include "configs.h"
#include "BluetoothA2DPSink.h"
#include "AudioTools/AudioLibs/SPDIFOutput.h"


SPDIFOutput stream;
BluetoothA2DPSink a2dp_sink;

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.println("\nStart Test Entry!");

    auto cfg = stream.defaultConfig();
    cfg.buffer_size = 384;
    cfg.buffer_count = 30;
    cfg.pin_data = 23;
    stream.begin(cfg);

    a2dp_sink.set_output(stream);
    a2dp_sink.set_on_volumechange(volume_cb);
    a2dp_sink.set_volume(64);
    a2dp_sink.start("MyMusic");
}

void loop() {
    vTaskDelete(nullptr); // Fixed bugs in issue (close arduino loop task without error)
//    delay(1000); // bugs in issue
}
#endif

