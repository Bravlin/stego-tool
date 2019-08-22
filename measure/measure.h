#include <stdint.h>
#include <math.h>

float mse(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);
float psnr(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);
float ssim(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height)