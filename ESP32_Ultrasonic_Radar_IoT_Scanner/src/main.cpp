// ====== RADARSCAN360 - HỆ THỐNG RADAR SIÊU ÂM IoT ======
// ESP32 + Servo + HC-SR04 + ThingsBoard Cloud
// Wokwi Simulation Compatible

// ====== THƯ VIỆN CẦN THIẾT ======
#include <Arduino.h>             // Bắt buộc khi dùng .cpp thay cho .ino
#include <WiFi.h>                // Kết nối WiFi
#include <Arduino_MQTT_Client.h> // MQTT client
#include <ThingsBoard.h>         // ThingsBoard IoT
#include <ESP32Servo.h>          // Servo motor cho ESP32

// ====== CẤU HÌNH CHÂN PHẦN CỨNG ======
#define SERVO_PIN 18 // Servo tại GPIO 18
#define TRIG_PIN 5   // HC-SR04 TRIG tại GPIO 5
#define ECHO_PIN 4   // HC-SR04 ECHO tại GPIO 4

// ====== CẤU HÌNH WIFI & THINGSBOARD ======
constexpr char WIFI_SSID[] = "Wokwi-GUEST";
constexpr char WIFI_PASSWORD[] = "";
// constexpr char WIFI_SSID[] = "Xuan Truong";      // <<< Dán SSID WiFi của bạn vào đây
// constexpr char WIFI_PASSWORD[] = "0387229116";   // <<< Dán mật khẩu WiFi của bạn vào đây
constexpr char TOKEN[] = "s5u3YF80ul7uD5GG588u"; // <<< Dán token của bạn vào đây
constexpr char THINGSBOARD_SERVER[] = "eu.thingsboard.cloud";
constexpr uint16_t THINGSBOARD_PORT = 1883U;
constexpr uint32_t MAX_MESSAGE_SIZE = 256U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;
constexpr uint32_t HEARTBEAT_INTERVAL_MS = 1000U;

// ====== CÁC BIẾN TOÀN CỤC ======
WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

Servo radarServo;

int currentAngle = 0;
int angleStep = 1; // Quay mượt
bool sweepForward = true;

uint32_t previousDataSend = 0;
constexpr int16_t SEND_INTERVAL = 200U; // Tăng từ 50ms lên 200ms để giảm MQTT traffic
uint32_t previousHeartbeat = 0;

// Biến quản lý timing HC-SR04
uint32_t previousMeasure = 0;
constexpr uint16_t MEASURE_INTERVAL = 100U; // Đo khoảng cách 100ms 1 lần
float lastDistance = 0;
uint32_t lastMeasureTime = 0;

// ====== KHAI BÁO PROTOTYPE ======
// Bắt buộc trong .cpp vì không có Arduino IDE tự scan
float measureDistance();
void InitWiFi();
bool reconnect();

// ====== HÀM ĐO KHOẢNG CÁCH HC-SR04 ======
float measureDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0)
        return -1;

    float distance = duration * 0.034 / 2.0;

    // HC-SR04 mặc định có dead zone ~2cm, nhưng giảm xuống 0.5cm để detect gần hơn
    if (distance < 0.5 || distance > 400)
        return -1;

    return distance;
}

// ====== HÀM KHỞI TẠO WIFI ======
void InitWiFi()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

// ====== HÀM KẾT NỐI LẠI WIFI ======
bool reconnect()
{
    if (WiFi.status() == WL_CONNECTED)
        return true;
    InitWiFi();
    return true;
}

// ====== SETUP ======
void setup()
{
    Serial.begin(SERIAL_DEBUG_BAUD);
    Serial.println("=== RadarScan360 Starting ===");
    Serial.println("Tip: Open Serial Monitor @ 115200 baud to see logs.");

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // PWM Range: 1000-2000 phù hợp với hầu hết servo SG90
    // Nếu servo còn jitter, dùng: 1040-1920 (range chính xác SG90)
    radarServo.attach(SERVO_PIN, 1000, 2000);
    radarServo.write(0);
    delay(500);

    InitWiFi();

    Serial.println("=== Radar Ready! Starting scan... ===");
}

// ====== LOOP ======
void loop()
{
    uint32_t loopStart = millis();

    if (!reconnect())
        return;

    if (!tb.connected())
    {
        Serial.print("Connecting to ThingsBoard: ");
        Serial.println(THINGSBOARD_SERVER);
        if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT))
        {
            Serial.println("Failed to connect ThingsBoard!");
            delay(100); // Giảm từ 2000 xuống 100
            return;
        }
        Serial.println("ThingsBoard Connected!");

        tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());
        tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
        tb.sendAttributeData("ssid", WiFi.SSID().c_str());
        tb.sendAttributeData("device", "RadarScan360");
    }

    // ===== CẬP NHẬT SERVO (QUY TRÌNH NHANH) =====
    radarServo.write(180 - currentAngle); // Đảo chiều để phù hợp với web interface

    // ===== ĐO KHOẢNG CÁCH (QUY TRÌNH CHẬM, CÓ THỂBLOCK) =====
    uint32_t now = millis();
    if (now - previousMeasure >= MEASURE_INTERVAL)
    {
        previousMeasure = now;
        lastDistance = measureDistance();
        lastMeasureTime = now;
    }
    float distance = lastDistance;

    if (now - previousDataSend >= SEND_INTERVAL)
    {
        previousDataSend = now;

        Serial.print("Angle: ");
        Serial.print(currentAngle);
        Serial.print("° | Distance: ");
        if (distance < 0)
            Serial.println("OUT OF RANGE");
        else
        {
            Serial.print(distance);
            Serial.println(" cm");
        }

        tb.sendTelemetryData("angle", currentAngle);
        tb.sendTelemetryData("distance", distance < 0 ? -1 : distance);
        // tb.sendAttributeData("rssi", WiFi.RSSI());  // Comment để tránh blocking
    }

    if (now - previousHeartbeat >= HEARTBEAT_INTERVAL_MS)
    {
        previousHeartbeat = now;
        Serial.print("[Heartbeat] uptime_ms=");
        Serial.print(now);
        Serial.print(" wifi=");
        Serial.print(WiFi.status() == WL_CONNECTED ? "OK" : "DOWN");
        Serial.print(" tb=");
        Serial.print(tb.connected() ? "OK" : "DOWN");
        Serial.print(" rssi=");
        Serial.println(WiFi.RSSI());
    }

    // ===== CẬP NHẬT GÓC SERVO =====
    if (sweepForward)
    {
        currentAngle += angleStep;
        if (currentAngle >= 180)
        {
            currentAngle = 180;
            sweepForward = false;
        }
    }
    else
    {
        currentAngle -= angleStep;
        if (currentAngle <= 0)
        {
            currentAngle = 0;
            sweepForward = true;
        }
    }

    tb.loop();

    // ===== ĐIỀU CHỈNH TIMING CHO MỤC TIÊU LOOP RATE 50Hz (20ms) =====
    uint32_t elapsed = millis() - loopStart;
    uint16_t targetLoopTime = 20; // 50 Hz
    if (elapsed < targetLoopTime)
    {
        delayMicroseconds((targetLoopTime - elapsed) * 1000);
    }
}
