#include <stdint.h>

long double mse(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);
long double psnr(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);
long double ssim(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height);