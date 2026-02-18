#pragma once

// =============================================================================
// GSM4 (DA017 Issue 4) Board Pin Mapping
// ESP32-WROOM-32E + KTS1622 expanders + shared I2C/PI2C bus
//
// Purpose:
// - Single source of truth for GPIO assignment and I2C inventory.
// - Keeps the firmware rewrite modular, with clear ownership boundaries.
//
// Conventions:
// - ESP32 pins are named PIN_*.
// - IO expander pins are named IOEXP<addr>_P0_* or IOEXP<addr>_P1_*
//   where P0/P1 correspond to the KTS1622 Port 0 and Port 1.
// - Active levels are explicitly defined where known.
//
// Strap pin cautions (ESP32):
// - GPIO0  : boot mode strap (BOOT button)
// - GPIO2  : strap pin (also used as MDM_PWRKEY here, be careful at boot)
// - GPIO5  : strap pin (SPI_CS2, expansion only)
// - GPIO12 : strap pin (SYS_EN, keep low at boot; has external pulldown)
// =============================================================================

#include <stdint.h>

// =============================================================================
// ESP32 UARTs
// =============================================================================
#define PIN_UART0_TXD                 1   // TXD0 (programming / console)
#define PIN_UART0_RXD                 3   // RXD0 (programming / console)

#define PIN_SYS_UART_TXD             19   // SYS_UART_TXD (to modem)
#define PIN_SYS_UART_RXD             17   // SYS_UART_RXD (from modem)

// Programming interface explicitly via UART0
#define PIN_PROG_UART_TXD     PIN_UART0_TXD
#define PIN_PROG_UART_RXD     PIN_UART0_RXD

// =============================================================================
// Modem control (Quectel EG21 etc.)
// =============================================================================
#define PIN_MDM_PWRKEY               2    // MDM_PWRKEY on GPIO2 (pin 24)
#define PIN_MDM_UART_DTR            15    // MDM_UART_DTR on GPIO15 (pin 23)

// NOTE: GPIO2 is a strapping pin. Ensure external circuitry keeps it in a safe
// state during reset/boot (do not force an illegal strap level).

// =============================================================================
// Boot / reset
// =============================================================================
// BOOT_BUT shorts GPIO0 to GND when pressed. No external pull.
// Use internal pull-up to avoid floating input during normal operation.
#define PIN_BOOT_BUTTON               0
#define BOOT_BUTTON_ACTIVE_LEVEL      0   // pressed = 0
#define BOOT_BUTTON_USE_PULLUP        1

// ESP32 EN pin is not a GPIO (RESET_BUT drives EN via discrete circuitry).
#define PIN_ESP_EN                   (-1)

// =============================================================================
// I2C (PI2C == I2C, shared bus)
// =============================================================================
#define PIN_I2C_SCL                  26   // PI2C_SCL == I2C_SCL
#define PIN_I2C_SDA                  27   // PI2C_SDA == I2C_SDA

// =============================================================================
// System enable / power gating
// =============================================================================
// SYS_EN has external 10k pulldown. Controlled devices are active high.
// GPIO12 is MTDI strap pin: keep low at boot stabilises.
#define PIN_SYS_EN                   12
#define SYS_EN_ACTIVE_LEVEL           1   // active high
#define SYS_EN_SAFE_DEFAULT           0   // off
#define SYS_EN_HAS_PULLDOWN           1

// =============================================================================
// Misc outputs
// =============================================================================
#define PIN_DOOR_LUM                 21
#define DOOR_LUM_ACTIVE_LEVEL         0   // active low
#define DOOR_LUM_SAFE_DEFAULT         1   // inactive (since active low)

// =============================================================================
// Display / UI
// =============================================================================
#define PIN_TFT_DC                   32

// TFT_INT exists, active high, pulled up via 10k.
// TFT currently unused, but line is pulled up so it will read high if not driven.
#define PIN_TFT_INT                  36
#define TFT_INT_ACTIVE_LEVEL          1

// =============================================================================
// Interrupts
// =============================================================================
// GPIO_INT is active low, connected to both KTS1622 expanders.
// Hardware: externally pulled up.
#define PIN_GPIO_INT                 39
#define GPIO_INT_ACTIVE_LEVEL         0
#define GPIO_INT_HAS_PULLUP           1

// GLOBAL_INT is output of AND chain:
//   GPIO_INT AND TFT_INT AND LIS_INT AND MODEM_RI AND '1'
// With contributors pulled high and asserting low, GLOBAL_INT is active low.
#define PIN_GLOBAL_INT               34
#define GLOBAL_INT_ACTIVE_LEVEL       0   // active low

// =============================================================================
// Analogue
// =============================================================================
#define PIN_VEXT_ADC                 35   // input only

// =============================================================================
// SPI bus (expansion header, currently unused)
// Naming confirmed: SPI_DIN = MISO, SPI_DOUT = MOSI
// =============================================================================
#define PIN_SPI_SCLK                 14
#define PIN_SPI_MISO                 13   // SPI_DIN
#define PIN_SPI_MOSI                 18   // SPI_DOUT
#define PIN_SPI_CS1                   4   // expansion header
#define PIN_SPI_CS2                   5   // expansion header (strap pin)

// =============================================================================
// I2S bus (audio)
// =============================================================================
#define PIN_I2S_DIN                  33
#define PIN_I2S_DOUT                 22
#define PIN_I2S_BCLK                 23   // I2S_SCLK
#define PIN_I2S_LRCLK                25

// =============================================================================
// I2C device inventory (7-bit addresses, no R/W bit)
// =============================================================================
#define I2C_ADDR_ES8388              0x10
#define I2C_ADDR_DS4420_0            0x50
#define I2C_ADDR_DS4420_1            0x52
#define I2C_ADDR_LIS3DH              0x18
#define I2C_ADDR_BQ25622             0x6B
#define I2C_ADDR_KTS1622_LOW         0x20
#define I2C_ADDR_KTS1622_HIGH        0x21

// =============================================================================
// KTS1622 IO expander mapping
// Two devices:
// - LOW  (0x20) : keypad, handsfree buttons, hookswitch, etc.
// - HIGH (0x21) : power and control outputs, monitors and UI inputs
//
// Hardware notes:
// - Expander RESET is active low but only pulled high (not controllable by ESP32).
// - GPIO_INT is active low and shared across both expanders.
// - Default pulls (inactive defaults):
//     HSHF_SELECT    pulled LOW
//     RINGER_CTL     pulled LOW
//     CHRG_EN        pulled LOW (explicit pulldown)
//     MDM_SIM_SEL    pulled LOW
//     HANDSFREE_CTL  pulled LOW
//
// Additional note (EXTPWR_CTL):
// - EXTPWR_CTL controls a transistor that pulls the PSU enable pin (active high) LOW.
// - Therefore: EXTPWR_CTL HIGH disables the PSU, EXTPWR_CTL LOW enables the PSU.
// =============================================================================

typedef enum {
  IOEXP_PORT0 = 0,
  IOEXP_PORT1 = 1,
} ioexp_port_t;

typedef struct {
  uint8_t addr;       // 7-bit I2C addr
  ioexp_port_t port;  // Port 0 or Port 1
  uint8_t bit;        // 0..7
  uint8_t active;     // 1 = active high, 0 = active low (if known)
} ioexp_pin_t;

// ------------------------------
// KTS1622 @ 0x20 (ADDR=LOW)
// ------------------------------

// P0 signals:
#define IOEXP20_P0_HANDSFREE_BUT1    (ioexp_pin_t){0x20, IOEXP_PORT0, 0, 1}
#define IOEXP20_P0_HANDSFREE_BUT2    (ioexp_pin_t){0x20, IOEXP_PORT0, 1, 1}
#define IOEXP20_P0_HANDSFREE_BUT3    (ioexp_pin_t){0x20, IOEXP_PORT0, 2, 1}
#define IOEXP20_P0_HANDSFREE_BUT4    (ioexp_pin_t){0x20, IOEXP_PORT0, 3, 1}
#define IOEXP20_P0_HOOK_SW           (ioexp_pin_t){0x20, IOEXP_PORT0, 4, 1}
#define IOEXP20_P0_HS_LOOP           (ioexp_pin_t){0x20, IOEXP_PORT0, 5, 1}
#define IOEXP20_P0_DOOR_SW           (ioexp_pin_t){0x20, IOEXP_PORT0, 6, 1}
// P0_7 is N/C

// P1 signals:
#define IOEXP20_P1_KEY_COL1          (ioexp_pin_t){0x20, IOEXP_PORT1, 0, 1}
#define IOEXP20_P1_KEY_COL2          (ioexp_pin_t){0x20, IOEXP_PORT1, 1, 1}
#define IOEXP20_P1_KEY_COL3          (ioexp_pin_t){0x20, IOEXP_PORT1, 2, 1}
#define IOEXP20_P1_KEY_COL4          (ioexp_pin_t){0x20, IOEXP_PORT1, 3, 1}
#define IOEXP20_P1_KEY_ROW1          (ioexp_pin_t){0x20, IOEXP_PORT1, 4, 1}
#define IOEXP20_P1_KEY_ROW2          (ioexp_pin_t){0x20, IOEXP_PORT1, 5, 1}
#define IOEXP20_P1_KEY_ROW3          (ioexp_pin_t){0x20, IOEXP_PORT1, 6, 1}
#define IOEXP20_P1_KEY_ROW4          (ioexp_pin_t){0x20, IOEXP_PORT1, 7, 1}

// ------------------------------
// KTS1622 @ 0x21 (ADDR=HIGH)
// ------------------------------

// P0 signals:
#define IOEXP21_P0_LOAD_CTL          (ioexp_pin_t){0x21, IOEXP_PORT0, 0, 1}
#define IOEXP21_P0_BEACON_CTL        (ioexp_pin_t){0x21, IOEXP_PORT0, 1, 1}
#define IOEXP21_P0_IO1_CTL           (ioexp_pin_t){0x21, IOEXP_PORT0, 2, 1}
#define IOEXP21_P0_IO2_CTL           (ioexp_pin_t){0x21, IOEXP_PORT0, 3, 1}
#define IOEXP21_P0_CHRG_EN           (ioexp_pin_t){0x21, IOEXP_PORT0, 4, 1} // pulldown, safe default low
#define IOEXP21_P0_MDM_SIM_SEL       (ioexp_pin_t){0x21, IOEXP_PORT0, 5, 1} // pulled low
#define IOEXP21_P0_CAM_EN            (ioexp_pin_t){0x21, IOEXP_PORT0, 6, 1}
#define IOEXP21_P0_EXTPWR_CTL        (ioexp_pin_t){0x21, IOEXP_PORT0, 7, 1} // HIGH disables PSU, LOW enables PSU

// EXTPWR_CTL semantics and recommended boot level
#define EXTPWR_CTL_ACTIVE_LEVEL       1   // active high means PSU disabled
#define EXTPWR_CTL_BOOT_LEVEL         0   // keep PSU enabled at boot

// P1 signals:
#define IOEXP21_P1_IO1_MON           (ioexp_pin_t){0x21, IOEXP_PORT1, 0, 1}
#define IOEXP21_P1_IO2_MON           (ioexp_pin_t){0x21, IOEXP_PORT1, 1, 1}
#define IOEXP21_P1_VOL_UP            (ioexp_pin_t){0x21, IOEXP_PORT1, 2, 1}
#define IOEXP21_P1_VOL_DOWN          (ioexp_pin_t){0x21, IOEXP_PORT1, 3, 1}
#define IOEXP21_P1_HANDSFREE_CTL     (ioexp_pin_t){0x21, IOEXP_PORT1, 4, 1} // pulled low
#define IOEXP21_P1_RINGER_CTL        (ioexp_pin_t){0x21, IOEXP_PORT1, 5, 1} // pulled low
#define IOEXP21_P1_HSHF_SELECT       (ioexp_pin_t){0x21, IOEXP_PORT1, 6, 1} // pulled low
#define IOEXP21_P1_12V_EN            (ioexp_pin_t){0x21, IOEXP_PORT1, 7, 1}

// =============================================================================
// Suggested Phase-0 safe defaults (BSP initialisation)
// =============================================================================
#define BSP_DEFAULT_SYS_EN_LEVEL      0
#define BSP_DEFAULT_DOOR_LUM_LEVEL    1
