#include <stdio.h>
#include <string.h>
#include "stego/stego.h"
#include "bitmap/bitmap.h"
#include "measure/measure.h"
#include "measure/plot.h"

#define IMG_MAX_SIZE 2073600

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
        printf("PSNR = %Lf\n", psnr(old_pixels, pixels, x, y, pixel_size));
        printf("SSIM = %Lf\n", ssim(old_pixels, pixels, x, y));
    }
    else if (action == 'p' && argc == 5)
    {
        char *text_const = argv[3];
        uint8_t k_const = argv[4][0] - '0';
        char text[256], output[256];
        int k_value[8], t_value[256];
        long double mse_value[256], psnr_value[256], ssim_value[256];
        int count = 0;
        uint32_t *old_pixels = (uint32_t *) malloc(sizeof(uint32_t) * IMG_MAX_SIZE);
        get_pixels(img_file, old_pixels, &x, &y, &pixel_size);
        for (uint8_t k = 1; k <= 8; k++)
        {
            copy_pixels(old_pixels, pixels, x, y);
            hide_text(text_const, x, y, k, pixels);
            k_value[count] = k;
            mse_value[count] = mse(old_pixels, pixels, x, y);
            psnr_value[count] = psnr(old_pixels, pixels, x, y, pixel_size);
            ssim_value[count] = ssim(old_pixels, pixels, x, y);
            printf("k = %d | MSE = %Lf | PSNR = %Lf | SSIM = %Lf\n", k, mse_value[count], psnr_value[count], ssim_value[count]);
            count++;
        }
        strcpy(output, img_file);
        output[strlen(output) - 4] = '\0'; // remove extension
        strcat(output, "_mse_k");
        plot(k_value, mse_value, count, "k", "MSE", "MSE vs k", output);
        strcpy(output, img_file);
        output[strlen(output) - 4] = '\0'; // remove extension
        strcat(output, "_psnr_k");
        plot(k_value, psnr_value, count, "k", "PSNR", "PSNR vs k", output);
        strcpy(output, img_file);
        output[strlen(output) - 4] = '\0'; // remove extension
        strcat(output, "_ssim_k");
        plot(k_value, ssim_value, count, "k", "SSIM", "SSIM vs k", output);
        count = 0;
        for (int t = 1; t <= strlen(text_const); t++)
        {
            strcpy(text, text_const);
            text[t] = '\0';
            copy_pixels(old_pixels, pixels, x, y);
            hide_text(text, x, y, k_const, pixels);
            t_value[count] = t;
            mse_value[count] = mse(old_pixels, pixels, x, y);
            psnr_value[count] = psnr(old_pixels, pixels, x, y, pixel_size);
            ssim_value[count] = ssim(old_pixels, pixels, x, y);
            printf("|t| = %d | MSE = %Lf | PSNR = %Lf | SSIM = %Lf\n", t, mse_value[count], psnr_value[count], ssim_value[count]);
            count++;
        }
        strcpy(output, img_file);
        output[strlen(output) - 4] = '\0'; // remove extension
        strcat(output, "_mse_t");
        plot(t_value, mse_value, count, "|t|", "MSE", "MSE vs |t|", output);
        strcpy(output, img_file);
        output[strlen(output) - 4] = '\0'; // remove extension
        strcat(output, "_psnr_t");
        plot(t_value, psnr_value, count, "|t|", "PSNR", "PSNR vs |t|", output);
        strcpy(output, img_file);
        output[strlen(output) - 4] = '\0'; // remove extension
        strcat(output, "_ssim_t");
        plot(t_value, ssim_value, count, "|t|", "SSIM", "SSIM vs |t|", output);
    }
}

void copy_pixels(uint32_t *pixels, uint32_t *copy, uint32_t x, uint32_t y)
{
    uint64_t i;
    for (i = 0; i < x * y; i++)
        copy[i] = pixels[i];
}