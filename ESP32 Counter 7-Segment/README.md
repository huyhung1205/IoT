# ESP32 Counter 7-Segment

## Tổng quan
Đây là dự án đếm số đơn giản trên ESP32 sử dụng LED 7 đoạn common cathode và 3 nút nhấn để điều khiển:

- `Increment`: tăng giá trị đếm lên 1.
- `Decrement`: giảm giá trị đếm đi 1.
- `Reset`: đặt giá trị đếm về 0.

Dự án được mô phỏng trên Wokwi, phù hợp cho minh họa cách đọc nút nhấn, điều khiển GPIO và hiển thị số trên LED 7 đoạn.

## Mục tiêu

- Làm quen với ESP32 và MicroPython.
- Hiểu cách điều khiển LED 7 đoạn common cathode.
- Xử lý nút nhấn với pull-up nội.
- Xây dựng bộ đếm có quay vòng trong khoảng 0 đến 9.

## Tính năng chính

- Đếm tăng/giảm bằng 2 nút riêng biệt.
- Nhấn reset để đưa giá trị về 0.
- Hiển thị 1 chữ số từ 0 đến 9 trên LED 7 đoạn.
- Có cơ chế quay vòng:
  - Tăng từ 9 sẽ về 0.
  - Giảm từ 0 sẽ về 9.
- Có debounce đơn giản bằng cách kiểm tra cạnh nút nhấn và trễ 100 ms.

## Phần cứng và sơ đồ kết nối

### Linh kiện

- 1 ESP32 DevKitC V4.
- 1 LED 7 đoạn common cathode.
- 3 nút nhấn.
- Dây nối.

### Bảng chân kết nối

| Chức năng | Chân ESP32 | Mô tả |
|---|---:|---|
| Nút tăng | GPIO14 | Nút nhấn `Increment`, pull-up nội |
| Nút giảm | GPIO12 | Nút nhấn `Decrement`, pull-up nội |
| Nút reset | GPIO5 | Nút nhấn `Reset`, pull-up nội |
| Đoạn a | GPIO23 | Điều khiển segment a |
| Đoạn b | GPIO22 | Điều khiển segment b |
| Đoạn c | GPIO21 | Điều khiển segment c |
| Đoạn d | GPIO19 | Điều khiển segment d |
| Đoạn e | GPIO18 | Điều khiển segment e |
| Đoạn f | GPIO4 | Điều khiển segment f |
| Đoạn g | GPIO2 | Điều khiển segment g |
| COM | GND | Chân chung của LED 7 đoạn |

## Nguyên lý hoạt động

Chương trình khởi tạo 3 nút nhấn ở chế độ input với `PULL_UP`. Khi nút không nhấn, giá trị đọc được là `1`; khi nhấn, giá trị về `0`.

Hàm `show_digit(n)` dựa vào bảng mã `digits` để bật/tắt từng segment tương ứng với số cần hiển thị. Vòng lặp chính đọc trạng thái các nút nhấn, phát hiện sự kiện cạnh xuống, cập nhật biến `counter`, sau đó gọi lại `show_digit(counter)`.

Có ba trường hợp xử lý:

- Nhả nút tăng: `counter = counter + 1`, nếu lớn hơn 9 thì quay về 0.
- Nhả nút giảm: `counter = counter - 1`, nếu nhỏ hơn 0 thì quay về 9.
- Nhả nút reset: `counter = 0`.

## Cách chạy dự án

### Trên Wokwi

1. Mở dự án trong Wokwi.
2. Đảm bảo file `wokwi-project.txt` đang trỏ đến dự án này.
3. Chạy mô phỏng.
4. Bấm các nút `Increment`, `Decrement`, `Reset` để kiểm tra kết quả.

### Trên ESP32 thực

1. Nạp MicroPython lên ESP32 nếu chưa có.
2. Chép `main.py` lên bộ nhớ của ESP32 với tên `main.py`.
3. Nối dây đúng theo bảng chân ở trên.
4. Cấp nguồn cho board.
5. Dùng 3 nút nhấn để điều khiển bộ đếm.

## Cấu trúc file

- `main.py`: mã nguồn chính của bộ đếm.
- `diagram.json`: sơ đồ mô phỏng Wokwi.
- `wokwi-project.txt`: liên kết đến dự án Wokwi.

## Giải thích mã nguồn

### Phần khai báo

- `machine.Pin(...)` được dùng để khởi tạo nút nhấn và các segment LED.
- `btn_inc`, `btn_dec`, `btn_rst` là 3 nút điều khiển.
- `segments` là danh sách 7 chân điều khiển đoạn a đến g.

### Bảng mã hiển thị

Biến `digits` lưu mẫu bit cho từng chữ số từ 0 đến 9. Mỗi số có 7 giá trị tương ứng với trạng thái của các segment a, b, c, d, e, f, g.

### Vòng lặp chính

Chương trình:

- Đọc trạng thái nút nhấn mỗi 100 ms.
- So sánh với trạng thái trước đó để phát hiện cạnh nhấn.
- Cập nhật bộ đếm và hiển thị lại ngay lập tức.

## Có thể mở rộng

- Thêm nút đếm tự động.
- Hiển thị 2 chữ số hoặc đếm lên tới 99.
- Thêm buzzer khi chuyển số.
- Thêm debounce dùng phương pháp thời gian chính xác hơn.
- Viết lại giao diện hiển thị cho 4 digit 7-segment.

## Ghi chú

- Dự án hiện tại dùng logic common cathode.
- Nếu đổi sang common anode, cần đảo ngược logic bật/tắt segment.
