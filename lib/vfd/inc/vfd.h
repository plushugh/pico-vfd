#ifndef VFD_H
#define VFD_H

#include "hardware/spi.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct {
  spi_inst_t *spi;

  uint cs_pin;
  uint reset_pin; /* pass UINT32_MAX if unused */
  uint en_pin;    /* pass UINT32_MAX if unused */

} vfd_t;

/* Initialization */
void vfd_init(vfd_t *vfd);

/* Display control */
void vfd_set_brightness(vfd_t *vfd, uint8_t value);
void vfd_refresh(vfd_t *vfd);

/* Text output */
void vfd_write_char(vfd_t *vfd, uint8_t pos, uint8_t chr);
void vfd_write_string(vfd_t *vfd, uint8_t pos, const char *str);

/* Testing functions */
void vfd_fullbright_test(vfd_t *vfd);

// TODO: User fonts, symbol support

#ifdef __cplusplus
}
#endif

#endif // !VFD_H
