#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/stdio.h"
#include "vfd.h"
#include <pico/time.h>
#include <stdio.h>

// Hardware Connections:
// VCC - 5V supply, at least 160mA
// EN  - Bridged to VCC
// GND - GND
// CLK - GPIO 18
// DIN - GPIO 19
// CS  - GPIO 17

// fps calculation logic
static uint32_t frames = 0;
static uint64_t last_us = 0;

void frame(void) {
  uint64_t now = time_us_64();
  frames++;

  if (now - last_us >= 1000000) {
    uint32_t fps = frames;
    printf("FPS: %u\n", fps);

    frames = 0;
    last_us = now;
  }
}

int main() {
  stdio_init_all();

  spi_init(spi0, 500 * 1000); // 500kHz SPI clock, no need for delay in lib
                              // FIXME: REMOVE DELAY IN LIB
  spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0,
                 SPI_MSB_FIRST); // this chip has no LSB_FIRST option,
                                 // bit-reversing manually implemented in lib

  gpio_set_function(18, GPIO_FUNC_SPI);
  gpio_set_function(19, GPIO_FUNC_SPI);

  vfd_t vfd0 = {
      .spi = spi0,
      .cs_pin = 17,
      .reset_pin = UINT32_MAX, // Reset pin not used
      .en_pin = UINT32_MAX,    // EN pin not used
  };

  vfd_init(&vfd0);

  vfd_fullbright_test(&vfd0); // Turn on all pixels

  sleep_ms(1000);

  uint8_t count = 0;

  while (1) {
    char str[6 + 1];                             // 6ch + \0
    snprintf(str, sizeof(str), "CT:%3u", count); // CT:000
    vfd_write_string(&vfd0, 0, str);
    vfd_refresh(&vfd0); // Need to send flush output command
    count++;

    frame(); // FPS counting, output to USB COM, should be 20 FPS
    sleep_ms(50);
  }

  return 0;
}
