# RadarScan  Slide outline & kịch bản thuyết trình (16 slides, 3 người)

## Phân vai

- **Người 1 (P1)**: Slides 1-5 — Mở bài, bài toán, kiến trúc, tổng quan
- **Người 2 (P2)**: Slides 6-11 — Phần cứng, đi dây, quét radar, HC-SR04, telemetry
- **Người 3 (P3)**: Slides 12-16 — Dashboard, sử dụng, hiệu năng, phát triển, Q&A

**Mỗi người thuyết trình 1 bộ slides liên tục (không xen kẽ)** — dễ nắm bắt hơn, flow tự nhiên.

---

# PHẦN 1: NGƯỜI 1 (Slides 1-5) — MỞ BÀI + KIẾN TRÚC

## Slide 1 — RadarScan360: Ultrasonic IoT Radar

**Tiêu đề slide**: RadarScan360 — Hệ Thống Radar Siêu Âm IoT

**Nội dung chính**

- ESP32 microcontroller + WiFi tích hợp
- Cảm biến siêu âm HC-SR04 (đo khoảng cách)
- Servo motor quay 180°
- Gửi dữ liệu real-time lên ThingsBoard Cloud qua MQTT
- Dashboard trực quan để giám sát từ xa

**Bullet points**

- ✓ ESP32 + HC-SR04 + Servo motor
- ✓ Quét toàn bộ 180° với đo khoảng cách mỗi góc
- ✓ Truyền dữ liệu real-time qua MQTT lên Cloud
- ✓ Giám sát và kiểm soát từ xa qua web dashboard

**Visual cần**: Logo/icon IoT, hình ảnh ESP32, HC-SR04, servo, icon cloud

**Lời thoại (Người 1)**

- "Chào thầy/cô và các bạn. Nhóm em xin giới thiệu RadarScan360 — một hệ thống radar siêu âm IoT sử dụng ESP32, cảm biến HC‑SR04 và servo motor. Hệ thống này có khả năng quét 180 độ để phát hiện vật cản, gửi dữ liệu thời gian thực lên ThingsBoard Cloud thông qua giao thức MQTT. Mục đích cuối cùng là cho phép người dùng giám sát và kiểm soát thiết bị từ xa thông qua một dashboard trực quan trên web."

---

## Slide 2 — Bài toán & mục tiêu

**Tiêu đề slide**: Bài Toán & Mục Tiêu Dự Án

**Nội dung chính**
Vấn đề: Các hệ thống đo khoảng cách thường chỉ đo 1 hướng, không thể phát hiện vật cản theo nhiều góc. Cần giải pháp chi phí thấp, dễ triển khai, có thể giám sát từ xa.

**Bullet points**

- ❌ Vấn đề: Đo cảm biến 1 hướng không đủ
- ✓ Giải pháp: Quét 360° dengan servo để tạo 'bản đồ' vật cản
- ✓ Hiển thị: Góc (°) + Khoảng cách (cm) theo thời gian thực
- ✓ Giám sát từ xa: Cloud-based dashboard, không cần thiết bị di động
- ✓ Mở rộng: Dễ thêm cảnh báo, lưu lịch sử, quản lý nhiều thiết bị

**Visual cần**: Sơ đồ so sánh (1 cảm biến vs quét multi-góc), icon "360", bảng so sánh ưu điểm

**Lời thoại (Người 1)**

- "Bài toán đặt ra là: làm sao phát hiện vật cản theo nhiều hướng, không chỉ 1 chiều? Và nó phải đơn giản, rẻ, có thể giám sát từ xa qua Internet."
- "Liên tưởng đến cảm biến thường chỉ đo 1 hướng. Nếu kết hợp servo quét góc, ta tạo dữ liệu dạng radar: mỗi điểm dữ liệu gồm góc quay và khoảng cách tương ứng."
- "Vì là IoT, ta cần nền tảng cloud. ThingsBoard cho phép nhận dữ liệu, lưu trữ, và dựng dashboard mà không phải tự viết backend từ đầu."

---

## Slide 3 — Demo nhanh: hệ thống làm gì?

**Tiêu đề slide**: Cách Hệ Thống Hoạt Động (Demo Flow)

**Nội dung chính**
Hệ thống tự động quét góc từ 0° đến 180° rồi quay ngược lại. Mỗi góc, cảm biến HC-SR04 đo khoảng cách và gửi kết quả lên cloud real-time.

**Bullet points**

- 1. Servo quay từ 0° → 180° → 0° (vòng lặp)
- 2. Mỗi góc: HC-SR04 đo khoảng cách (cm)
- 3. Dữ liệu: (angle, distance) gửi MQTT → Cloud
- 4. Dashboard: Vẽ radar trực quan cập nhật real-time

**Visual cần**: Sơ đồ flow 4 bước, animation servo quay, icon MQTT cloud, screenshot dashboard radar

**Lời thoại (Người 1)**

- "Nếu mô tả ngắn gọn: thiết bị tự động quét 180 độ, đo khoảng cách từng góc, gửi liên tục lên đám mây qua WiFi."
- "Trên cloud, ThingsBoard nhận 2 thông số chính: angle (góc hiện tại) và distance (khoảng cách đo được). Dashboard dùng đúng 2 trường này để vẽ hiển thị radar."
- "Khi người xem mở web dashboard, họ thấy radar quét theo thời gian thực, như một thiết bị radar thực tế — chỉ khác là nằm trên cloud."

---

## Slide 4 — Kiến trúc tổng thể

**Tiêu đề slide**: Kiến Trúc Hệ Thống (4 Lớp)

**Nội dung chính**
Hệ thống chia 4 lớp: tầng thiết bị (ESP32), tầng mạng (WiFi), tầng truyền (MQTT), tầng cloud (ThingsBoard).

**Bullet points**

- **Tầng 1 - Thiết bị (Edge)**: ESP32 đọc cảm biến + điều khiển servo
- **Tầng 2 - Mạng**: WiFi kết nối Internet (802.11b/g/n)
- **Tầng 3 - Truyền**: MQTT publish telemetry real-time (lightweight, low-latency)
- **Tầng 4 - Cloud**: ThingsBoard nhận, lưu trữ, dựng dashboard

**Visual cần**: Sơ đồ kiến trúc 4 lớp chồng lên nhau, mũi tên dữ liệu từ thiết bị → cloud

**Lời thoại (Người 1)**

- "Kiến trúc của dự án có 4 lớp chính. Lớp 1: Thiết bị ESP32 là 'tim' của hệ thống, nó đọc cảm biến và điều khiển servo. Lớp 2: WiFi là kênh truyền kết nối ra Internet. Lớp 3: MQTT là giao thức truyền dữ liệu nhẹ, độ trễ thấp, ideal cho IoT. Lớp 4: ThingsBoard Cloud là nơi nhận, lưu trữ, và cung cấp dashboard."
- "Điểm quan trọng: dữ liệu được sinh ra tại edge (thiết bị) theo từng chu kỳ quét. Mọi thứ phía sau cloud chỉ cần 'nhận và hiển thị' — thiết kế đơn giản, dễ scale."

---

# PHẦN 2: NGƯỜI 2 (Slides 5-10) — PHẦN CỨNG + FIRMWARE LOWER-LEVEL

## Slide 5 — Module phần cứng

**Tiêu đề slide**: Các Module Phần Cứng Chính

**Nội dung chính**
Dự án sử dụng 3 module chính: ESP32 (MCU), HC-SR04 (cảm biến), Servo (motor). Tất cả là chip phổ biến, rẻ, dễ mua.

**Bullet points - Bảng thông số**

| Module               | Chức năng          | Đặc điểm                        | Giá  |
| -------------------- | -------------------- | ----------------------------------- | ----- |
| **ESP32**      | MCU + WiFi           | 2 lõi, 520KB RAM, WiFi 802.11b/g/n | ~$5-8 |
| **HC-SR04**    | Cảm biến siêu âm | Đo 2-400cm, ±3% sai số           | ~$2-3 |
| **Servo SG90** | Motor servo          | Quay 0-180°, torque ~1.5kg·cm     | ~$3-4 |
| **Power 5V**   | Nguồn               | 2A+ (riêng servo)                  | ~$2-5 |

**Bullet points - Ưu/Nhược**

- ✓ ESP32: WiFi tích hợp, cộng đồng lớn, hỗ trợ tốt
- ✓ HC-SR04: Siêu âm phổ biến, dễ triển khai, chính xác reasonably
- ✓ SG90: Giá rẻ, consumen ít dòng, quay nhanh tương đối
- ✗ HC-SR04: Dead zone ~2cm, bị ảnh hưởng nhiễu môi trường
- ✗ Servo: Jitter khi PWM không ổn, cần nguồn ổn định

**Visual cần**: Hình ảnh 3 module chính (ESP32, HC-SR04, SG90), bảng thông số, icon $

**Lời thoại (Người 2)**

- "Nhóm chọn ESP32 vì có WiFi tích hợp, cộng đồng cực lớn, ngôn ngữ Arduino quen thuộc. So với Arduino cũ, ESP32 mạnh hơn nhiều: 2 lõi CPU, 520KB RAM, còn hỗ trợ Bluetooth."
- "HC‑SR04 là cảm biến siêu âm phổ biến nhất, dễ triển khai, rẻ (~$2-3). Nó đo khoảng cách 2-400cm với độ chính xác tạm được."
- "Servo SG90 cho phép quay 0-180°, đúng nhu cầu của chúng ta. Khi triển khai thật, nguồn rất quan trọng: servo kéo dòng lớn, có thể gây voltage drop làm reset ESP32. Nên tách nguồn servo riêng và nối chung GND."

---

## Slide 6 — Kết nối dây (Wiring)

**Tiêu đề slide**: Sơ Đồ Đi Dây & Kết Nối Chân

**Nội dung chính**
Servo, HC-SR04, ESP32 kết nối qua GPIO và power. Tất cả chia sẻ GND (ground) chung.

**Bảng Wiring**

| Component              | Pin ESP32     | Giải thích                           |
| ---------------------- | ------------- | -------------------------------------- |
| **Servo**        | GPIO 18       | PWM signal cho servo (quay)            |
| **HC-SR04 TRIG** | GPIO 5        | Output trigger 10µs pulse             |
| **HC-SR04 ECHO** | GPIO 4        | Input: nhận echo pulse từ cảm biến |
| **VCC (5V)**     | VIN hoặc USB | Cấp điện 5V cho servo + HC-SR04     |
| **GND (Ground)** | GND           | Chung mass cho tất cả                |

**Bullet points - Ghi chú quan trọng**

- Servo: GPIO 18, PWM 1000-2000µs (50Hz standard SG90)
- HC-SR04: TRIG (GPIO 5) gửi pulse 10µs, ECHO (GPIO 4) nhận phản hồi
- Power: VCC 5V từ USB hoặc adapter, GND chung (rất quan trọng!)
- **IMPORTANT**: Servo cần 5V ≥2A. Nếu chỉ USB, có thể reset. **Dùng adapter 5V riêng!**
- File Wokwi: `diagram.json` — import để test giả lập trước lắp thực tế

**Visual cần**: Sơ đồ wiring rõ ràng (Fritzing diagram hoặc tương tự), icon GPIO, chủ đề màu cho mỗi kết nối

**Lời thoại (Người 2)**

- "Đây là sơ đồ đi dây chuẩn theo cấu hình firmware: servo vào GPIO18 (PWM), HC-SR04 TRIG vào GPIO5, ECHO vào GPIO4."
- "Điểm rất quan trọng: tất cả module phải chia sẻ GND (ground) chung. Nếu không, tín hiệu sẽ không ổn định, servo jitter hoặc HC-SR04 không đo được."
- "Về power: servo cần 5V ≥2A. Nếu cấp từ USB computer, có thể không đủ dòng → servo chập chờn hoặc reset ESP32. Khuyến cáo dùng adapter 5V power riêng hoặc power bank."
- "Nhóm cũng chuẩn bị sơ đồ Wokwi trong `diagram.json`. Có thể import vào Wokwi online để test/demo nhanh trước khi lắp mạch thật. Đây là cách học IoT rất tiện!"

---

## Slide 7 — Radar quét 180° hoạt động thế nào?

**Tiêu đề slide**: Nguyên Lý Quét & Thuật Toán

**Nội dung chính**
Servo quay từng bước từ 0° đến 180° rồi quay ngược. Mỗi góc, HC-SR04 đo khoảng cách 1 lần. Cứ lặp lại.

**Pseudocode - Thuật Toán Quét**

```
currentAngle = 0
angleStep = 1°       // Hoặc 2-3° nếu muốn nhanh hơn
sweepForward = true

while (true) {
    radarServo.write(currentAngle)          // Quay servo
    distance = measureDistance()             // Đo khoảng cách
    send_to_cloud(angle, distance)          // Gửi lên cloud
  
    if (sweepForward) {
        currentAngle += angleStep
        if (currentAngle >= 180) {
            currentAngle = 180
            sweepForward = false               // Đổi chiều
        }
    } else {
        currentAngle -= angleStep
        if (currentAngle <= 0) {
            currentAngle = 0
            sweepForward = true                // Đổi chiều
        }
    }
    delay(20ms)  //  50Hz loop rate
}
```

**Bullet points**

- **Biến trạng thái**: currentAngle (góc hiện tại), angleStep (bước quay), sweepForward (chiều quay)
- **Bước quay**: 1° = mượt máy, 2-3° = nhanh hơn. Tuỳ chỉnh ở `angleStep`
- **Đổi chiều**: Khi đạt 0° hoặc 180°, đổi biến `sweepForward` từ true ↔ false
- **Output**: Mỗi góc tạo 1 cặp data (angle, distance)
- **Tốc độ**: 50Hz loop (~20ms) tạo quét mượt mà

**Quét 360° (nâng cao - không trong dự án này)**

- Có thể thêm servo thứ 2 (vertical tilt) để quét 360° toàn bộ không gian
- Hoặc dùng LiDAR quay 2D

**Visual cần**: Diagram góc quay (0° → 90° → 180° → 90° → 0°), pseudocode, animation servo quay

**Lời thoại (Người 2)**

- "Phần 'radar' là kết hợp chuyển động quét (servo) + đo khoảng cách (HC-SR04) lặp lại liên tục."
- "Thuật toán rất rõ: tăng góc từ 0 đến 180, rồi giảm về 0. Bước angleStep (1-3 độ) cân bằng giữa độ mịn chuyển động và tốc độ quét. Mỗi góc tạo 1 cặp data (angle, distance) rồi gửi lên cloud."
- "Vì mỗi cặp được gửi liên tục, dashboard chỉ cần nhận là có thể dựng hiệu ứng radar quét theo thời gian thực. Quét càng nhanh, grid points càng ít nhưng tốc độ cao. Quét chậm, grid dense hơn nhưng mất thời gian."

---

## Slide 8 — Đo khoảng cách với HC-SR04

**Tiêu đề slide**: Nguyên Lý Cảm Biến Siêu Âm HC-SR04

**Nội dung chính**
HC-SR04 sử dụng "time-of-flight": phát xung siêu âm, đo thời gian phản hồi, tính khoảng cách từ v = s/2t.

**Diagram - Cách Hoạt Động**

```
ESP32:
  Bước 1: TRIG ↑ (gửi pulse 10µs)
  Bước 2: Chờ ECHO ↑ (siêu âm phát đi)
  Bước 3: Đo thời gian từ ECHO ↑ đến ECHO ↓ (phản hồi trở về)
  Bước 4: Tính distance = duration × 0.034 / 2   (cm)

HC-SR04:
  ├─ TRIG = input từ ESP32 (pulse 10µs trigger)
  ├─ ECHO = output lại ESP32 (pulse width phản ánh khoảng cách)
  └─ Đến 4m cách xa thì ECHO rộng tối đa ~29ms
```

**Công thức & Tính toán**

- Tốc độ âm thanh: ~340 m/s = 0.034 cm/µs
- Echo duration (µs) = thời gian âm thanh bay 2 chiều (đi + về)
- **Distance (cm) = duration × 0.034 / 2**
- **Ví dụ**: Nếu duration = 580µs → distance = 580 × 0.034 / 2 ≈ 9.86cm
- **Valid range**: 0.5cm - 400cm (firmware tối ưu, dead zone giảm)

**Bullet points**

- **Phát**: TRIG gửi pulse 10µs → siêu âm phát đi
- **Phản hồi**: Vật cản phản xạ siêu âm trở lại
- **Đo thời gian**: ECHO pulse width = khoảng cách
- **Range hợp lệ**: 2-400cm (phụ thuộc môi trường)
- **Timeout**: Nếu không nhận ECHO trong 30ms → return -1 (OUT_OF_RANGE)
- **Accuracy**: ±3% sai số (ngành hàng: được chấp nhận)

**Nhược điểm HC-SR04**

- Dead zone: ~2cm quá gần không đo được (hoặc sai)
- Sensitive to enviroment: Bị ảnh hưởng vô số, vật mềm hấp thụ
- Tốc độ: Slow (~60ms per measurement), không thích hợp high-frequency

**Visual cần**: Diagram time-of-flight, sơ đồ TRIG/ECHO pulse, công thức khoảng cách, chart accuracy

**Lời thoại (Người 2)**

- "HC‑SR04 hoạt động theo kỹ thuật time-of-flight: phát xung siêu âm 40kHz, đo thời gian phản hồi từ chướng ngại vật, từ đó tính khoảng cách."
- "Công thức rất đơn giản: distance = duration × 0.034 / 2 (cm). Tốc độ âm thanh ~340m/s = 0.034cm/µs. Chia 2 vì âm thanh bay đi + về."
- "Firmware dùng `pulseIn()` có timeout (30ms) tránh treo khi không có echo — chẳng hạn vật quá xa hoặc vật hấp thụ âm. Nếu timeout, trả về -1. Khoảng cách hợp lệ: 2-400cm."
- "Độ chính xác ±3% khá tốt. Nhược điểm: dead zone ~2-3cm (quá gần không đo được). Nhưng firmware tối ưu để nhạy hơn, giảm xuống 0.5cm."

---

## Slide 9 — Truyền dữ liệu thời gian thực

**Tiêu đề slide**: Telemetry & Timing: Cân Bằng Để Real-Time

**Nội dung chính**
Hệ thống phải cân bằng 3 điểm: servo update mượt (20ms), HC-SR04 đo (100-200ms), gửi MQTT (50-200ms).

**Bảng Timing Optimization**

| Thöngsố                 | Giá trị    | Ghi chí                     |
| ------------------------- | ------------ | ---------------------------- |
| **Loop Rate**       | 50 Hz (20ms) | Servo update frequency       |
| **HC-SR04 Measure** | Mỗi 100ms   | Tránh pulseIn() block servo |
| **MQTT Send**       | Mỗi 200ms   | 5 telemetry/sec = real-time  |
| **Servo Update**    | Mỗi 20ms    | Smooth quay, không jitter   |
| **angleStep**       | 1°          | Quét mượt mà             |

**Bullet points - Data Sent**

- **Telemetry 1**: `angle` (độ) — góc servo hiện tại
- **Telemetry 2**: `distance` (cm) — khoảng cách đo được (-1 nếu invalid)
- **Attribute**: `rssi` (dBm) — WiFi signal strength (mỗi 1000ms)

**Visual cần**: Timeline diagram (20ms, 100ms, 200ms), bảng thông số, phổ tần số gửi

**Lời thoại (Người 2)**

- "Để tạo cảm giác real-time, phải gửi telemetry theo chu kỳ ngắn. Nhưng nếu gửi quá nhanh (mỗi 10ms), WiFi/MQTT resources chết. Nếu quá chậm (mỗi 500ms), radar trông giật lag."
- "Nhóm cân bằng: servo update mỗi 20ms (50Hz) để mượt, HC-SR04 đo mỗi 100ms (tránh blocking), MQTT gửi mỗi 200ms (5 samples/sec = real-time đủ)."
- "Ngoài telemetry chính (angle, distance), gửi thêm `rssi` (WiFi signal strength). Khi demo từ xa, rssi thấp giải thích được độ trễ hoặc gián đoạn. Rất hữu ích khi troubleshoot!"
- "Công thức tối ưu: tách HC-SR04 measurement chạy độc lập (không block servo loop) — điều này tạo servo mượt mà trên dashboard, không bị giật."

---

## Slide 10 — MQTT & Kết nối ThingsBoard

**Tiêu đề slide**: MQTT Protocol & Cloud Connection

**Nội dung chính**
MQTT là lightweight pub/sub protocol. Thay vì tự dựng broker, dùng ThingsBoard cloud broker public.

**Connection Parameters**

- **Host**: eu.thingsboard.cloud
- **Port**: 1883 (MQTT standard)
- **Auth**: Device Access Token (từ ThingsBoard)
- **Topic**: v1/devices/me/telemetry

**Firmware Code Snippet**

```
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
    Serial.println("Failed to connect");
}

tb.sendTelemetryData("angle", currentAngle);
tb.sendTelemetryData("distance", distance);
tb.loop();
```

**Bullet points**

- Library: Arduino_MQTT_Client + ThingsBoard SDK
- tb.connect(host, TOKEN, port) — kết nối broker
- tb.sendTelemetryData() — gửi telemetry
- tb.loop() — duy trì kết nối
- Auto-reconnect khi down

**Visual cần**: Diagram client-broker, code flow, MQTT logo

**Lời thoại (Người 2)**

- "MQTT là 'đường ống' truyền dữ liệu real-time từ ESP32 lên cloud. Nhẹ hơn HTTP, độ trễ thấp (50-100ms), ideal cho IoT."
- "ThingsBoard cung cấp broker MQTT công khai. Thay vì tự dựng (phức tạp), chỉ dùng sẵn. Chỉ cần token là xong."
- "Firmware dùng thư viện ThingsBoard để gửi telemetry. Quan trọng nhất: gọi tb.loop() đều đặn để giữ kết nối.",
- "Auto-reconnect giúp khi demo từ xa WiFi gián đoạn — tự kết nối lại mà không cần reset."

---

# PHẦN 3: NGƯỜI 3 (Slides 11-13) — CLOUD + DASHBOARD + KẾT THÚC

## Slide 11 — Dashboard giám sát từ xa

**Tiêu đề slide**: Dashboard ThingsBoard — Giám Sát Real-Time

**Nội dung chính**
ThingsBoard dashboard là giao diện người dùng. Export sẵn dashboard.json trong repo — import trực tiếp, không cần cấu hình phức tạp.

**Dashboard Components**

1. **Widget Radar** (chính)

   - Datasource: angle + distance từ device
   - Hiển thị: Bản đồ radar 180°, cập nhật real-time
2. **Gauge Widgets** (phụ)

   - Angle gauge (0-180°)
   - Distance gauge (0-400cm)
3. **Chart Widget** (lịch sử)

   - Time-series distance vs time
   - Tuỳ chỉnh timewindow (1h, 6h, 1d, etc.)
4. **Status Indicators**

   - WiFi signal strength (rssi)
   - Device connection status
   - Last update time

**Bullet points**

- **Radar Widget**: Map angle + distance (2 datasource = radar visual)
- **Timeseries Chart**: Lưu lịch sử distance để phân tích
- **Status Panel**: Theo dõi kết nối (green = OK, red = down)
- **Tuỳ chỉnh**: Thay đổi color, layout, range dễ dàng
- **Export**: dashboard.json — backup & share

**Visual cần**: Screenshot dashboard ThingsBoard (radar + chart + status), dashboard layout diagram

**Lời thoại (Người 3)**

- "Dashboard là nơi người dùng cuối tương tác — là cửa sổ để nhìn vào thiết bị từ xa. Trên ThingsBoard, không cần code web — chỉ drag-drop widgets là xong."
- "Repo có dashboard.json (export sẵn) để import trực tiếp từ file. Không cần cấu hình từ đầu."
- "Vì dữ liệu đã là 'góc + khoảng cách', widget radar chỉ cần map đúng 2 key là dựng được hiển thị trực quan. Không cần code custom phức tạp."
- "Ngoài radar chính, có chart lưu lịch sử để phân tích trend, gauge để xem giá trị tức thời, status panel để biết thiết bị có online hay không."

---

## Slide 12 — Hướng dẫn sử dụng (end-to-end)

**Tiêu đề slide**: Bước-By-Step Triển Khai (5 Bước)

**Step-by-Step Guide**

**Step 1: Tạo Device trên ThingsBoard (2 phút)**

```
1. Login: https://eu.thingsboard.cloud
2. Devices → "+" → New device
3. Name: "RadarScan360"
4. Copy Access Token (lưu cẩn thận!)
```

**Step 2: Cấu hình Firmware (3 phút)**

```cpp
// src/main.cpp - Dòng ~20-24
constexpr char WIFI_SSID[] = "Wokwi-GUEST";
constexpr char WIFI_PASSWORD[] = "";
constexpr char TOKEN[] = "Dán Device Access Token";
constexpr char THINGSBOARD_SERVER[] = "eu.thingsboard.cloud";
```

**Step 3: Build & Upload (3 phút)**

```
1. PlatformIO: Click "Build" 
2. Kết nối ESP32 USB → computer
3. Click "Upload"
4. Mở Serial Monitor (115200 baud)
```

**Step 4: Xác nhận Kết nối Serial Monitor (2 phút)**

```
Kỳ vọng thấy dòng:
=== RadarScan360 Starting ===
Connecting to WiFi...
.....
WiFi Connected!
IP: 192.168.x.x
Connecting to ThingsBoard: eu.thingsboard.cloud
ThingsBoard Connected!
[Heartbeat] uptime_ms=5000 wifi=OK tb=OK rssi=-50
```

**Step 5: Import Dashboard & Quan Sát (2 phút)**

```
1. Vào Dashboards → "+" → Import dashboard
2. Chọn file: dashboard.json (từ repo)
3. Click "Import"
4. Quan sát radar quét real-time!
```

**Bullet points**

- Bước 1: Device token là chìa khoá (lưu cẩn thận!)
- Bước 2: WiFi + token phải đúng
- Bước 3: Serial Monitor để debug
- Bước 4: "ThingsBoard Connected!" = success
- Bước 5: dashboard.json ready-to-use

**Visual cần**: 5 screenshots thứ tự (ThingsBoard create device, code snippet, PlatformIO upload, Serial monitor success, dashboard import)

**Lời thoại (Người 3)**

- "Quy trình triển khai end-to-end rất đơn giản, chỉ 5 bước."
- "**Bước 1-2**: Tạo Device trên ThingsBoard, copy token, dán vào firmware. **Bước 3**: Build/upload bằng PlatformIO — quá trình quen thuộc."
- "**Bước 4**: Mở Serial Monitor (115200 baud) để xác nhận: WiFi kết nối OK, ThingsBoard kết nối OK. Nếu thấy dòng 'ThingsBoard Connected!' → success!"
- "**Bước 5**: Trở lại ThingsBoard, import dashboard.json (export sẵn). Khi firmware chạy, dashboard cập nhật liên tục — radar quét ngay trên web! Xong! Bây giờ có thể mở từ bất cứ browser nào miễn kết nối Internet."

---

## Slide 13 — Chạy giả lập trên Wokwi

**Tiêu đề slide**: Giả Lập Trên Wokwi

**Nội dung chính**

Demo Live Options

## Slide 14 — Kết thúc

****Tiêu đề slide**: Cảm Ơn

**Nội dung chính**

Ảnh kết thúc dự án (ví dụ: radar quét trên dashboard, team photo, hoặc logo IoT)

**(Người 1)**: "Nhóm em xin cảm ơn thầy/cô, các thầy/cô trong hội đồng, và các bạn đã lắng nghe. Dự án RadarScan360 là sự kết hợp giữa phần cứng IoT (servo, sensor) và cloud platform (ThingsBoard). Chúng em hy vọng dự án này giúp các bạn hiểu rõ hơn về công nghệ IoT hiện đại."
