#include "vfd.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

// RP2040 does not have LSB_FIRST option
static inline uint8_t bitrev8(uint8_t x) {
  x = (x >> 4) | (x << 4);
  x = ((x & 0xCC) >> 2) | ((x & 0x33) << 2);
  x = ((x & 0xAA) >> 1) | ((x & 0x55) << 1);
  return x;
}

static inline void vfd_cs(vfd_t *vfd, bool level) {
  gpio_put(vfd->cs_pin, level);
}

static inline void vfd_delay_short(void) {}

static void vfd_write_byte(vfd_t *vfd, uint8_t data) {
  data = bitrev8(data); // Manual LSB FIRST
  spi_write_blocking(vfd->spi, &data, 1);
  vfd_delay_short();
}

/* ---------- Public API ---------- */

void vfd_init(vfd_t *vfd) {
  /* CS */
  gpio_init(vfd->cs_pin);
  gpio_set_dir(vfd->cs_pin, GPIO_OUT);
  gpio_put(vfd->cs_pin, 1);

  /* Optional EN */
  if (vfd->en_pin != UINT32_MAX) {
    gpio_init(vfd->en_pin);
    gpio_set_dir(vfd->en_pin, GPIO_OUT);
    gpio_put(vfd->en_pin, 1);
  }

  /* Optional RESET */
  if (vfd->reset_pin != UINT32_MAX) {
    gpio_init(vfd->reset_pin);
    gpio_set_dir(vfd->reset_pin, GPIO_OUT);
    gpio_put(vfd->reset_pin, 1);
  }

  /* DIM setup */
  vfd_cs(vfd, 0);
  vfd_write_byte(vfd, 0xE0);
  vfd_write_byte(vfd, 0x05); // display SKU dependent: 6-digit: 0x05 8-digit:
                             // 0x07 16-digit: 0x0f
  vfd_cs(vfd, 1);

  /* Default brightness */
  vfd_set_brightness(vfd, 0xFF);
}

void vfd_set_brightness(vfd_t *vfd, uint8_t value) {
  vfd_cs(vfd, 0);
  vfd_write_byte(vfd, 0xE4);
  vfd_write_byte(vfd, value);
  vfd_cs(vfd, 1);
}

void vfd_refresh(vfd_t *vfd) {
  vfd_cs(vfd, 0);
  vfd_write_byte(vfd, 0xE8);
  vfd_cs(vfd, 1);
}

void vfd_write_char(vfd_t *vfd, uint8_t pos, uint8_t chr) {
  vfd_cs(vfd, 0);
  vfd_write_byte(vfd, 0x20 + pos);
  vfd_write_byte(vfd, chr + 0x30);
  vfd_cs(vfd, 1);
}

void vfd_write_string(vfd_t *vfd, uint8_t pos, const char *str) {
  vfd_cs(vfd, 0);
  vfd_write_byte(vfd, 0x20 + pos);

  while (*str) {
    vfd_write_byte(vfd, (uint8_t)*str++);
  }

  vfd_cs(vfd, 1);
}

void vfd_fullbright_test(vfd_t *vfd) {
  vfd_cs(vfd, 0);
  vfd_write_byte(vfd, 0xE9);
  vfd_cs(vfd, 1);
}
