#include <stdio.h>
#include "stego/stego.h"
#include "bitmap/bitmap.h"
#include "measure/measure.h"

void copy_pixels(uint32_t *pixels, uint32_t *copy, uint32_t x, uint32_t y);

int main(int argc, char **argv)
{
    uint32_t pixels[2073600];
    uint32_t x, y;
    uint16_t pixel_size;
    char action = argv[1][0];
    char *img_file = argv[2];
    if (action == 'g' && argc == 3)
    {
        char text[256];
        get_pixels(img_file, pixels, &x, &y, &pixel_size);
        get_text(pixels, x, y, text);
        printf("%s\n", text);
    }
    else if (action == 'h' && argc == 5)
    {
        char *text = argv[3];
        uint8_t k = argv[4][0] - '0';
        uint32_t old_pixels[2073600];
        get_pixels(img_file, pixels, &x, &y, &pixel_size);
        copy_pixels(pixels, old_pixels, x, y);
        hide_text(text, pixel_size, x, y, k, pixels);
        set_pixels(img_file, pixels);
        printf("MSE = %f\n", mse(old_pixels, pixels, x, y));
        printf("PSNR = %f\n", psnr(old_pixels, pixels, x, y));
        printf("SSIM = %f\n", ssim(old_pixels, pixels, x, y));
    }
}

void copy_pixels(uint32_t *pixels, uint32_t *copy, uint32_t x, uint32_t y)
{
    uint64_t i;
    for (i = 0; i < x * y; i++)
        copy[i] = pixels[i];
}