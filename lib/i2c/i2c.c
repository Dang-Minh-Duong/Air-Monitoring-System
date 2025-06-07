#include "i2c.h"
#define DPORT_PERIP_CLK_EN_REG (*(volatile uint32_t *)0x3FF000C0)
#define I2C_BASE_ADDR        0x3FF53000

#define I2C_FIFO_REG         (I2C_BASE_ADDR + 0x100)
#define I2C_SCL_LOW_PERIOD   (I2C_BASE_ADDR + 0x0)
#define I2C_SCL_HIGH_PERIOD  (I2C_BASE_ADDR + 0x38)
#define I2C_CMD_REG(n)       (I2C_BASE_ADDR + 0x58 + ((n) * 4))
#define I2C_CTRL_REG         (I2C_BASE_ADDR + 0x04)
#define I2C_FIFO_CONF_REG    (I2C_BASE_ADDR + 0x18)
#define I2C_INT_CLR_REG      (I2C_BASE_ADDR + 0x24)
#define I2C_INT_RAW_REG      (I2C_BASE_ADDR + 0x20)
#define I2C_STATUS_REG       (I2C_BASE_ADDR + 0x2C)

#define I2C_ADDR             0x27
#define WRITE_BIT            0x0
#define BIT(x)               (1 << (x))

#define REG_WRITE(addr, val) (*((volatile unsigned int *)(addr)) = (val))
#define REG_READ(addr)       (*((volatile unsigned int *)(addr)))

/////////////////////////////////////////////////////////////////////////
#define GPIO_BASE_ADDR            0x3FF44000
#define IO_MUX_BASE_ADDR          0x3FF49000

// Thanh ghi chọn tín hiệu output cho một chân GPIO
// (Bảng 4-5, Trang 21 & Thanh ghi 4.32, Trang 33)
#define GPIO_FUNC_OUT_SEL_CFG_REG(n)  (GPIO_BASE_ADDR + 0x0530 + ((n) * 4))
#define GPIO_ENABLE_W1TS_REG      (GPIO_BASE_ADDR + 0x0024)

// Thanh ghi chọn chân GPIO input cho một tín hiệu ngoại vi
// (Bảng 4-5, Trang 21 & Thanh ghi 4.31, Trang 32)
#define GPIO_FUNC_IN_SEL_CFG_REG(y)   (GPIO_BASE_ADDR + 0x0130 + ((y) * 4))

// Thanh ghi cấu hình PAD của một chân GPIO
// (Bảng 4-5, Trang 21 & Thanh ghi 4.30, Trang 32)
#define GPIO_PIN_REG(n)               (GPIO_BASE_ADDR + 0x0088 + ((n) * 4))

// Thanh ghi cấu hình IO_MUX của một chân GPIO
// (Bảng 4-6, Trang 22 & Thanh ghi 4.34, Trang 35)
// Lưu ý: Địa chỉ này thay đổi tùy theo chân, ví dụ cho GPIO21, GPIO22
#define IO_MUX_GPIO21_REG             (IO_MUX_BASE_ADDR + 0x007C)
#define IO_MUX_GPIO22_REG             (IO_MUX_BASE_ADDR + 0x0080)


// --- Index của tín hiệu I2C (Từ Bảng 4-2, Trang 11) ---
#define I2CEXT0_SCL_OUT_IDX       29  // 
#define I2CEXT0_SDA_OUT_IDX       30  // 
#define I2CEXT0_SCL_IN_IDX        29  // 
#define I2CEXT0_SDA_IN_IDX        30  // 
static void delay(int count) {
    while (count--) {
        asm volatile ("nop");
    }
}

void i2c_init() {
    REG_WRITE(DPORT_PERIP_CLK_EN_REG, BIT(7));
    const int scl_pin = 22;
    const int sda_pin = 21;

    /* * BƯỚC 1 & 2: CẤU HÌNH GPIO MATRIX CHO CẢ ĐƯỜNG RA VÀ VÀO
     */

    // --- Cấu hình cho SCL (GPIO 22) ---
    // 1.1. Đường ra: Nối tín hiệu I2CEXT0_SCL_out (index 29) ra chân GPIO 22.
    // Ghi vào thanh ghi GPIO_FUNC22_OUT_SEL_CFG, trường GPIO_FUNCn_OUT_SEL (bits 0-8).
    REG_WRITE(GPIO_FUNC_OUT_SEL_CFG_REG(scl_pin), I2CEXT0_SCL_OUT_IDX); // 

    // 1.2. Đường vào: Nối tín hiệu từ chân GPIO 22 vào ngoại vi I2CEXT0_SCL_in (index 29).
    // Ghi vào thanh ghi GPIO_FUNC29_IN_SEL_CFG.
    // Đặt GPIO_FUNCy_IN_SEL = 22 (chân input) và GPIO_SIGy_IN_SEL = 1 (dùng GPIO matrix).
    REG_WRITE(GPIO_FUNC_IN_SEL_CFG_REG(I2CEXT0_SCL_IN_IDX), (scl_pin) | (1 << 7)); // 

    // --- Cấu hình cho SDA (GPIO 21) ---
    // 1.3. Đường ra: Nối tín hiệu I2CEXT0_SDA_out (index 30) ra chân GPIO 21.
    REG_WRITE(GPIO_FUNC_OUT_SEL_CFG_REG(sda_pin), I2CEXT0_SDA_OUT_IDX); // 

    // 1.4. Đường vào: Nối tín hiệu từ chân GPIO 21 vào ngoại vi I2CEXT0_SDA_in (index 30).
    REG_WRITE(GPIO_FUNC_IN_SEL_CFG_REG(I2CEXT0_SDA_IN_IDX), (sda_pin) | (1 << 7)); // 

    REG_WRITE(GPIO_ENABLE_W1TS_REG, BIT(scl_pin));
    REG_WRITE(GPIO_ENABLE_W1TS_REG, BIT(sda_pin));


    /* * BƯỚC 3: CẤU HÌNH PAD (Thuộc tính vật lý của chân)
     */
    
    // --- Cấu hình cho SCL (GPIO 22) ---
    // 3.1. Chuyển chân sang chế độ open-drain.
    // Ghi vào thanh ghi GPIO_PIN22_REG, set bit GPIO_PINn_PAD_DRIVER (bit 2).
    REG_WRITE(GPIO_PIN_REG(scl_pin), BIT(2)); // 
    
    // 3.2. Cấu hình IO MUX: Bật input, bật pull-up, và chọn chức năng là GPIO.
    // Ghi vào thanh ghi IO_MUX_GPIO22_REG.
    // FUN_IE (bit 9) = 1 (Input Enable)
    // FUN_WPU (bit 8) = 1 (Weak Pull-up Enable)
    // MCU_SEL (bits 12-14) = 2 (Function 2 là GPIO cho chân này)
    REG_WRITE(IO_MUX_GPIO22_REG, (BIT(9)) | (BIT(8)) | (2 << 12)); // 

    // --- Cấu hình cho SDA (GPIO 21) ---
    // 3.3. Chuyển chân sang chế độ open-drain.
    REG_WRITE(GPIO_PIN_REG(sda_pin), BIT(2)); // 

    // 3.4. Cấu hình IO MUX: Bật input, bật pull-up, và chọn chức năng là GPIO.
    REG_WRITE(IO_MUX_GPIO21_REG, (BIT(9)) | (BIT(8)) | (2 << 12)); // 

    ///////////////////////////////////////////////////
    REG_WRITE(I2C_SCL_LOW_PERIOD, 40);
    REG_WRITE(I2C_SCL_HIGH_PERIOD, 40);
    REG_WRITE(I2C_FIFO_CONF_REG, BIT(10));
    REG_WRITE(I2C_INT_CLR_REG, 0xFFFFFFFF);
    REG_WRITE(I2C_CTRL_REG, BIT(4));
}

void i2c_master_write_byte_new(uint8_t *data, uint8_t length) {
    REG_WRITE(I2C_INT_CLR_REG, 0xFFFFFFFF);
    REG_WRITE(I2C_FIFO_REG, (I2C_ADDR << 1) | WRITE_BIT);

    for (uint8_t i = 0; i < length; i++) {
        REG_WRITE(I2C_FIFO_REG, data[i]);
    }

    REG_WRITE(I2C_CMD_REG(0), (0x0 << 11)); // Start
    REG_WRITE(I2C_CMD_REG(1), 
        (0x1 << 11) |     // OPCODE = 1: WRITE
        (1 << 10)   |     // ACK_CHECK_EN
        (0 << 8)    |     // ACK_EXP
        (0 << 7)    |     // ACK_VAL
        (length & 0xFF)); // Number of bytes
    REG_WRITE(I2C_CMD_REG(2), (0x3 << 11)); // Stop

    REG_WRITE(I2C_CTRL_REG, BIT(5));

    while (REG_READ(I2C_STATUS_REG) & BIT(9)) {
        delay(1000);
    }
}
