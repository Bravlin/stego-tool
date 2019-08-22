#include <stdint.h>

double mse(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);
double psnr(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);
double ssim(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);