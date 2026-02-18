# ESP32-WROOM-32E pins 1 to 39, GSM4 circuit functions

Notes:
- “Pin No.” refers to the **ESP32-WROOM-32E module pad number** (top view) as per the Espressif pin definition table.
- GPIO strap pins in use on this board: **GPIO0, GPIO2, GPIO5, GPIO12**. Treat carefully at reset.
- Input only pins: **GPIO34, GPIO35, GPIO36, GPIO39** (no internal pull-ups or pull-downs available on these).

| Pin No. | Module pad name | ESP32 signal | Direction | Net, function in this circuit | Notes |
|---:|---|---|---|---|---|
| 1 | GND | – | Power | Ground | |
| 2 | 3V3 | – | Power | 3.3 V supply | |
| 3 | EN | CHIP_PU | Input | Module enable | Driven by reset circuit, not a GPIO |
| 4 | SENSOR_VP | GPIO36 | Input only | **TFT_INT** | Active high, externally pulled up (10 kΩ) |
| 5 | SENSOR_VN | GPIO39 | Input only | **GPIO_INT** from KTS1622 expanders | Active low, externally pulled up |
| 6 | IO34 | GPIO34 | Input only | **GLOBAL_INT** (AND chain output) | Active high output of logic chain |
| 7 | IO35 | GPIO35 | Input only | **VEXT_ADC** | External voltage sense ADC input |
| 8 | IO32 | GPIO32 | I/O | **TFT_DC** | TFT data/command select |
| 9 | IO33 | GPIO33 | I/O | **I2S_DIN** | Audio data into ESP32 |
| 10 | IO25 | GPIO25 | I/O | **I2S_LRCLK** | Audio word select |
| 11 | IO26 | GPIO26 | I/O | **I2C_SCL / PI2C_SCL** | Shared I²C bus SCL |
| 12 | IO27 | GPIO27 | I/O | **I2C_SDA / PI2C_SDA** | Shared I²C bus SDA |
| 13 | IO14 | GPIO14 | I/O | **SPI_CLK** | Expansion SPI clock |
| 14 | IO12 | GPIO12 (MTDI) | I/O | **SYS_EN** | Active high enables, external 10 kΩ pull-down |
| 15 | GND | – | Power | Ground | |
| 16 | IO13 | GPIO13 | I/O | **SPI_MISO (SPI_DIN)** | Expansion SPI MISO |
| 17 | SD2 | GPIO9 (flash) | – | Internal flash signal | Do not use |
| 18 | SD3 | GPIO10 (flash) | – | Internal flash signal | Do not use |
| 19 | CMD | GPIO11 (flash) | – | Internal flash signal | Do not use |
| 20 | CLK | GPIO6 (flash) | – | Internal flash signal | Do not use |
| 21 | SD0 | GPIO7 (flash) | – | Internal flash signal | Do not use |
| 22 | SD1 | GPIO8 (flash) | – | Internal flash signal | Do not use |
| 23 | IO15 | GPIO15 | I/O | **MDM_UART_DTR** | Modem UART DTR control |
| 24 | IO2 | GPIO2 | I/O | **MDM_PWRKEY** | Strap pin, avoid forcing illegal strap state at reset |
| 25 | IO0 | GPIO0 | I/O | **BOOT_BUT** | Button shorts to GND, use internal pull-up |
| 26 | IO4 | GPIO4 | I/O | **SPI_CS1** | Expansion SPI chip select 1 |
| 27 | IO16 | GPIO16 | I/O | **LED_DATA** | WS2812 style LED data line |
| 28 | IO17 | GPIO17 | I/O | **SYS_UART_RXD** | Modem UART RX into ESP32 |
| 29 | IO5 | GPIO5 | I/O | **SPI_CS2** | Expansion SPI chip select 2, strap pin |
| 30 | IO18 | GPIO18 | I/O | **SPI_MOSI (SPI_DOUT)** | Expansion SPI MOSI |
| 31 | IO19 | GPIO19 | I/O | **SYS_UART_TXD** | Modem UART TX from ESP32 |
| 32 | NC | – | – | Not connected | Module pad is NC |
| 33 | IO21 | GPIO21 | I/O | **DOOR_LUM** | Output, active low |
| 34 | RXD0 | GPIO3 (U0RXD) | I/O | **Programming UART0 RX** | Used for flashing and console |
| 35 | TXD0 | GPIO1 (U0TXD) | I/O | **Programming UART0 TX** | Used for flashing and console |
| 36 | IO22 | GPIO22 | I/O | **I2S_DOUT** | Audio data out of ESP32 |
| 37 | IO23 | GPIO23 | I/O | **I2S_BCLK (I2S_SCLK)** | Audio bit clock |
| 38 | GND | – | Power | Ground | |
| 39 | GND (exposed pad) | – | Power | Ground, thermal pad | Solder to GND plane |

## Shared I²C bus device list (7-bit addresses)
- ES8388 codec: 0x10
- DS4420 PGA: 0x50
- DS4420 PGA: 0x52
- LIS3DH accelerometer: 0x18
- BQ25622 charger: 0x6B
- KTS1622 IO expander (ADDR low): 0x20
- KTS1622 IO expander (ADDR high): 0x21
