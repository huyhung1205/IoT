import machine # Thư viện để điều khiển phần cứng
import time # Thư viện để tạo độ trễ

# Khai báo nút nhấn
btn_inc = machine.Pin(14, machine.Pin.IN, machine.Pin.PULL_UP) # GPIO14
btn_dec = machine.Pin(12, machine.Pin.IN, machine.Pin.PULL_UP) # GPIO12
btn_rst = machine.Pin(5, machine.Pin.IN, machine.Pin.PULL_UP)  # GPIO5

# Khai báo chân cho LED 7 đoạn (a, b, c, d, e, f, g)
segments = [
    machine.Pin(23, machine.Pin.OUT), # a
    machine.Pin(22, machine.Pin.OUT), # b
    machine.Pin(21, machine.Pin.OUT), # c
    machine.Pin(19, machine.Pin.OUT), # d
    machine.Pin(18, machine.Pin.OUT), # e
    machine.Pin(4, machine.Pin.OUT),  # f
    machine.Pin(2, machine.Pin.OUT)   # g
]

# Bảng mã số cho LED 7 đoạn (0–9)
digits = {
    0: [1,1,1,1,1,1,0],
    1: [0,1,1,0,0,0,0],
    2: [1,1,0,1,1,0,1],
    3: [1,1,1,1,0,0,1],
    4: [0,1,1,0,0,1,1],
    5: [1,0,1,1,0,1,1],
    6: [1,0,1,1,1,1,1],
    7: [1,1,1,0,0,0,0],
    8: [1,1,1,1,1,1,1],
    9: [1,1,1,1,0,1,1]
}

# Hàm hiển thị số trên LED 7 đoạn
def show_digit(n):
    if n in digits:
        for i, seg in enumerate(segments):
            seg.value(digits[n][i])

counter = 0 # Biến đếm
last_inc = last_dec = last_rst = 1 # Lưu trạng thái trước đó của các nút (1: không nhấn, 0: nhấn)

print("=== ESP32 Counter with 7-Segment ===")
show_digit(counter) # Hiển thị số ban đầu (0)

while True:
    inc = btn_inc.value() # Đọc trạng thái nút Increment
    dec = btn_dec.value() # Đọc trạng thái nút Decrement
    rst = btn_rst.value() # Đọc trạng thái nút Reset

    # Kiểm tra nếu nút Increment được nhấn (trạng thái chuyển từ 1 → 0)
    if inc == 0 and last_inc == 1:
        counter += 1
        if counter > 9: counter = 0   # quay vòng về 0
        print("Increment →", counter)
        show_digit(counter)

    # Kiểm tra nếu nút Decrement được nhấn (trạng thái chuyển từ 1 → 0)
    if dec == 0 and last_dec == 1:
        counter -= 1
        if counter < 0: counter = 9   # quay vòng về 9
        print("Decrement →", counter)
        show_digit(counter)

    # Kiểm tra nếu nút Reset được nhấn (trạng thái chuyển từ 1 → 0)
    if rst == 0 and last_rst == 1:
        counter = 0
        print("Reset →", counter)
        show_digit(counter)

    last_inc, last_dec, last_rst = inc, dec, rst
    time.sleep_ms(100)