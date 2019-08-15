float mse(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t heigth);
float psnr(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t heigth);
float ssim(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t heigth, uint32_t window_size)