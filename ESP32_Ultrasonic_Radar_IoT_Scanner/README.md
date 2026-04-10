# RadarScan360 — ESP32 Ultrasonic IoT Radar (HC-SR04 + Servo + ThingsBoard Cloud)

Hệ thống “Radar IoT” quét **180°** bằng **Servo** và đo vật cản bằng **cảm biến siêu âm HC-SR04**, sau đó **stream dữ liệu thời gian thực qua MQTT** lên **ThingsBoard Cloud** để **giám sát từ xa trên Dashboard**.

> Repo này đã kèm sẵn cấu hình PlatformIO, sơ đồ đi dây Wokwi và file export Dashboard/Widget để bạn tái tạo demo nhanh.

---

## 1) Tính năng nổi bật

- Quét radar **0 → 180 → 0** (bước góc mặc định **2°**).
- Gửi telemetry thời gian thực lên ThingsBoard:
  - `angle` (độ)
  - `distance` (cm, `-1` nếu ngoài ngưỡng/không đo được)
- Gửi attribute/metadata thiết bị:
  - `macAddress`, `localIp`, `ssid`, `device`
- Dashboard ThingsBoard hiển thị trực quan (Radar widget) + theo dõi từ xa qua web.
- Tương thích giả lập Wokwi (`diagram.json`, `wokwi.toml`).

---

## 2) Kiến trúc hệ thống

Luồng dữ liệu tổng quát:

1. **ESP32** điều khiển **Servo** quét 180°.
2. Mỗi góc quét, **HC-SR04** đo khoảng cách.
3. ESP32 kết nối WiFi và publish dữ liệu lên **ThingsBoard Cloud** qua **MQTT**.
4. **Dashboard** đọc timeseries và hiển thị radar theo thời gian thực.

```mermaid
flowchart LR
  HC[HC-SR04] -->|distance| ESP[ESP32]
  SERVO[Servo] <-->|PWM| ESP
  ESP -->|WiFi| AP[(Router/AP)]
  AP -->|MQTT :1883| TB[ThingsBoard Cloud\n(eu.thingsboard.cloud)]
  TB --> DASH[Dashboard\nRadar widget]
```

---

## 3) Phần cứng (Hardware)

**Bắt buộc**

- ESP32 DevKit (board: `esp32doit-devkit-v1`)
- HC-SR04 Ultrasonic Sensor
- Servo (SG90/MG90S hoặc tương đương)
- Nguồn 5V ổn định (khuyến nghị **nguồn rời cho servo**) + nối mass chung

**Lưu ý nguồn**

- Servo có thể gây sụt áp/giật nguồn nếu lấy trực tiếp từ cổng USB của ESP32. Khi chạy thật, nên cấp **5V riêng cho servo** và **GND chung** với ESP32.

---

## 4) Sơ đồ đi dây (Wiring)

Theo cấu hình trong `src/main.cpp`:

| Module  | Pin  | ESP32       |
| ------- | ---- | ----------- |
| Servo   | PWM  | GPIO18      |
| Servo   | V+   | 5V/VIN (5V) |
| Servo   | GND  | GND         |
| HC-SR04 | TRIG | GPIO5       |
| HC-SR04 | ECHO | GPIO4       |
| HC-SR04 | VCC  | 5V/VIN (5V) |
| HC-SR04 | GND  | GND         |

File sơ đồ Wokwi: `diagram.json`

---

## 5) Phần mềm (Firmware) & thư viện

### 5.1 PlatformIO

- Cấu hình môi trường: `platformio.ini`
- Board: `esp32doit-devkit-v1`
- Framework: Arduino

### 5.2 Libraries

Repo đang khai báo dependency trong `platformio.ini` (một số có thể phục vụ mở rộng):

- `thingsboard/ThingsBoard` (kết nối ThingsBoard/MQTT)
- `madhephaestus/ESP32Servo` (điều khiển servo)
- Ngoài ra còn có `PubSubClient`, `DHT`, `LiquidCrystal_I2C`, `ArduinoOTA` (chưa dùng trong `src/main.cpp` hiện tại)

---

## 6) Cấu hình ThingsBoard Cloud

Server dùng trong firmware:

- Host: `eu.thingsboard.cloud`
- MQTT port: `1883`

Các bước khuyến nghị:

1. Tạo **Device** trên ThingsBoard Cloud.
2. Copy **Access Token** của device.
3. Dán token vào `src/main.cpp` tại hằng `TOKEN`.
4. (Tuỳ chọn) Import dashboard export:
   - `dashboard.json` (Dashboard export)
   - `wingetcustom.json` (thực chất là cấu hình widget export trong repo này)

Telemetry keys mà Dashboard cần:

- `angle`
- `distance`

---

## 7) Cách chạy dự án

### 7.1 Chạy trên phần cứng thật

1. Cập nhật WiFi SSID/PASSWORD và `TOKEN` trong `src/main.cpp`.
2. Build & upload bằng PlatformIO (VS Code) hoặc CLI:
   - Build: `pio run`
   - Upload: `pio run -t upload`
3. Mở Serial Monitor (115200) để xem log góc quét và khoảng cách.
   - Nếu bạn chạy trong **Terminal**: `pio device monitor` (hoặc `pio device monitor -b 115200`).
   - Nếu bạn muốn upload xong tự mở monitor: `pio run -t upload -t monitor`.
   - Lưu ý: Terminal build (`pio run`) **không tự hiển thị log Serial** nếu bạn chưa mở Serial Monitor.
4. Mở ThingsBoard Dashboard để quan sát thời gian thực.

### 7.2 Chạy giả lập Wokwi

- Wokwi dùng:
  - `diagram.json` (mạch giả lập)
  - `wokwi.toml` (trỏ tới firmware build bởi PlatformIO)
- Luồng chạy gợi ý:
  1. `pio run` để tạo `.pio/build/.../firmware.bin`
  2. Mở dự án Wokwi (VS Code extension hoặc Wokwi project) và chạy.
  3. Xem log Serial trong Wokwi (không phải `pio device monitor`):
     - Trong VS Code: `Ctrl+Shift+P` → chạy **Wokwi: Start Simulation**
     - Sau khi simulation chạy, mở **View → Output** và chọn channel **Wokwi** (hoặc tab **Wokwi Console** nếu VS Code của bạn có).
     - Firmware sẽ in `Angle`, `Distance` và thêm dòng `[Heartbeat]` mỗi 1 giây.

---

## 8) Giải thích code (cốt lõi)

File chính: `src/main.cpp`

### 8.0 Cấu hình nhanh (pins, server, chu kỳ gửi)

Đây là nhóm tham số quyết định pin phần cứng, endpoint cloud và “độ mượt” realtime:

```cpp
// Hardware pins
#define SERVO_PIN 18
#define TRIG_PIN  5
#define ECHO_PIN  4

// ThingsBoard Cloud (MQTT)
constexpr char TOKEN[] = "...";
constexpr char THINGSBOARD_SERVER[] = "eu.thingsboard.cloud";
constexpr uint16_t THINGSBOARD_PORT = 1883U;

// Scan & send settings
int angleStep = 2;
constexpr int16_t SEND_INTERVAL = 50U; // ms
```

### 8.1 Quét 180°

- Biến `currentAngle` chạy từ `0` đến `180`.
- `angleStep = 2` tạo độ mịn quét.
- `sweepForward` đổi chiều khi chạm biên 0/180.

Trích đoạn code (cốt lõi) điều khiển servo và đổi chiều:

```cpp
radarServo.write(currentAngle);

if (sweepForward) {
  currentAngle += angleStep;
  if (currentAngle >= 180) { currentAngle = 180; sweepForward = false; }
} else {
  currentAngle -= angleStep;
  if (currentAngle <= 0)   { currentAngle = 0;   sweepForward = true; }
}
```

> Tốc độ quét phụ thuộc `delay(15)` trong `loop()`, `angleStep` và `SEND_INTERVAL`. Nếu muốn dashboard “mượt” hơn, giảm `SEND_INTERVAL` hoặc chỉ tăng/giảm góc đúng vào thời điểm gửi telemetry.

### 8.2 Đo khoảng cách HC-SR04

- Kích xung TRIG 10µs, đo độ rộng xung ECHO bằng `pulseIn(..., 30000)`.
- Quy đổi cm: `distance = duration * 0.034 / 2`.
- Nếu timeout hoặc ngoài ngưỡng (2–400cm) thì trả `-1`.

Trích đoạn code (rút gọn) của `measureDistance()`:

```cpp
digitalWrite(TRIG_PIN, LOW);  delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);

long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
if (duration == 0) return -1;

float distance = duration * 0.034 / 2.0; // cm
if (distance < 2 || distance > 400) return -1;
return distance;
```

Giải thích nhanh:

- `0.034` xấp xỉ **0.034 cm/µs** (tốc độ âm thanh ~340 m/s).
- Chia `2.0` vì sóng siêu âm đi-về (round-trip).

### 8.3 Gửi dữ liệu thời gian thực qua MQTT (ThingsBoard)

- Kết nối ThingsBoard qua `tb.connect(host, TOKEN, port)`.
- Chu kỳ gửi: `SEND_INTERVAL = 50ms`.
- Gửi:
  - `tb.sendTelemetryData("angle", currentAngle)`
  - `tb.sendTelemetryData("distance", ...)`
  - `tb.sendAttributeData("rssi", WiFi.RSSI())`
- `tb.loop()` duy trì vòng lặp MQTT.

Trình tự trong `loop()` (đã lược bớt log Serial):

```cpp
if (!tb.connected()) {
  if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) return;
  tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());
  tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
  tb.sendAttributeData("ssid", WiFi.SSID().c_str());
  tb.sendAttributeData("device", "RadarScan360");
}

radarServo.write(currentAngle);
float distance = measureDistance();

if (millis() - previousDataSend >= SEND_INTERVAL) {
  previousDataSend = millis();
  tb.sendTelemetryData("angle", currentAngle);
  tb.sendTelemetryData("distance", distance < 0 ? -1 : distance);
  tb.sendAttributeData("rssi", WiFi.RSSI());
}

tb.loop();
```

> Dashboard ThingsBoard đang map trực tiếp 2 key timeseries chính là `angle` và `distance` (xem `dashboard.json`).

### 8.4 Kết nối WiFi & tự phục hồi

Firmware tách riêng hàm khởi tạo WiFi và hàm “reconnect”:

```cpp
void InitWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

bool reconnect() {
  if (WiFi.status() == WL_CONNECTED) return true;
  InitWiFi();
  return true;
}
```

Lưu ý:

- `InitWiFi()` đang **block** cho đến khi kết nối thành công (phù hợp demo), nên nếu WiFi sai mật khẩu/không có mạng thì chương trình sẽ chờ mãi.
- Khi WiFi rớt, vòng lặp sẽ gọi `InitWiFi()` để nối lại, sau đó ThingsBoard sẽ được reconnect ở nhánh `if (!tb.connected())`.

### 8.5 Gợi ý khi public repo

- Không nên commit `TOKEN` thật lên GitHub. Có thể dùng `build_flags` trong `platformio.ini` hoặc tách `secrets.h` (gitignore) để an toàn hơn.

---

## 9) Vì sao chọn ThingsBoard?

- **MQTT-native**: phù hợp luồng telemetry thời gian thực từ IoT device.
- **Dashboard có sẵn**: kéo-thả, tuỳ biến widget, hiển thị timeseries nhanh.
- **Quản lý thiết bị**: device/token, attributes, telemetry rõ ràng.
- **Mở rộng dễ**: rule engine/cảnh báo/lưu trữ/đa thiết bị khi nâng cấp dự án.
- **Cloud tiện demo**: truy cập mọi nơi, phù hợp bài thuyết trình/triển khai nhanh.
