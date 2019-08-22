#include <stdint.h>

int get_pixels(char *filename, uint32_t *pixels, uint32_t *x, uint32_t *y, uint16_t *pixel_size);
int set_pixels(char *filename, uint32_t *pixels);