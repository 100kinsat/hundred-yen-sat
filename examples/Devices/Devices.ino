#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Leds leds;
LightSensor light;
Motors motors;
Speaker speaker;
Imu imu;
Gnss gnss;
Storage sd;
Recorder recorder(sd);

void setup() {
    Serial.begin(115200);

    leds.begin();
    light.begin();
    motors.begin();
    // 左右は独立したMotorとしても操作できる。
    motors.right().forward(100);
    motors.left().forward(80);
    delay(200);
    motors.stop();
    speaker.begin();

    leds.setPattern(0x01);
    speaker.beep();

    if (!imu.begin()) Serial.println("BNO055 initialization failed");
    if (!gnss.begin()) Serial.println("GNSS initialization failed");

    if (sd.begin(3000)) {
        recorder.begin("sensor.csv", "millis,light");
    } else {
        Serial.println("SD is not mounted at /mnt/sd0");
    }
}

void loop() {
    const uint16_t lightValue = light.read();
    char line[48];
    snprintf(line, sizeof(line), "%llu,%u",
             static_cast<unsigned long long>(millis()), lightValue);
    recorder.writeLine(line);

    gnss.update();
    delay(200);
}
