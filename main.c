#include <stdio.h>
#include <string.h>
#include "stego/stego.h"
#include "bitmap/bitmap.h"
#include "measure/measure.h"

#define IMG_MAX_SIZE 2073600

#define LOREN_IPSUM "Lorem ipsum dolor sit amet consectetur adipiscing elit sodales integer, taciti odio vivamus eros sociis augue purus eget, nullam tempor nec parturient ut ornare habitasse dictumst. Orci viverra lectus hac facilisis mi et habitasse, augue erat ullamcorper."

void copy_pixels(uint32_t *pixels, uint32_t *copy, uint32_t x, uint32_t y);

int main(int argc, char **argv)
{
    uint32_t *pixels = (uint32_t *) malloc(sizeof(uint32_t) * IMG_MAX_SIZE);
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
        uint32_t *old_pixels = (uint32_t *) malloc(sizeof(uint32_t) * IMG_MAX_SIZE);
        get_pixels(img_file, pixels, &x, &y, &pixel_size);
        copy_pixels(pixels, old_pixels, x, y);
        hide_text(text, x, y, k, pixels);
        set_pixels(img_file, pixels);
        printf("MSE = %Lf\n", mse(old_pixels, pixels, x, y));
        printf("PSNR = %Lf\n", psnr(old_pixels, pixels, x, y));
        printf("SSIM = %Lf\n", ssim(old_pixels, pixels, x, y));
    }
    else if (action == 'p' && argc == 3)
    {
        char text[256], hid_file[256];
        strcpy(text, LOREN_IPSUM);
        uint32_t *old_pixels = (uint32_t *) malloc(sizeof(uint32_t) * IMG_MAX_SIZE);
        get_pixels(img_file, pixels, &x, &y, &pixel_size);
        copy_pixels(pixels, old_pixels, x, y);
        for (uint8_t k = 1; k <= 8; k++)
        {
            hide_text(text, x, y, k, old_pixels);
            strcpy(hid_file, img_file);
            set_pixels(hid_file, pixels);
            printf("MSE = %Lf\n", mse(old_pixels, pixels, x, y));
            printf("PSNR = %Lf\n", psnr(old_pixels, pixels, x, y));
            printf("SSIM = %Lf\n", ssim(old_pixels, pixels, x, y));
        }
    }
}

void copy_pixels(uint32_t *pixels, uint32_t *copy, uint32_t x, uint32_t y)
{
    uint64_t i;
    for (i = 0; i < x * y; i++)
        copy[i] = pixels[i];
}