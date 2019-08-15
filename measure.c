#include <stdint.h>
#include <math.h>
#include "measure.h"
#define MAX_PIXEL_VALUE 0b111111111111111111111111

/* Mean Squared Error */
float mse(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t heigth)
{
    int i, j;
    int aux_1, aux_2 = 0, return_value;

    for (i = 0; i < heigth; i++)
    {
        j = 0;
        aux_1 = 0;
        for (i = 0; j < width; j++)
            aux_1 += pow(original_image[i * width + j] - stego_image[i * width + j], 2);
        aux_2 += aux_1;
    }
    return_value = aux_2 / (heigth * width);

    return return_value;
}

/* Peak Signal-to-Noise Ratio */
float psnr(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t heigth)
{
    float return_value, mse_value = mse(original_image, stego_image, width, heigth);
    uint32_t max_value = 0b0, i, image_size = width * heigth;

    while (i < image_size && max_value < MAX_PIXEL_VALUE)
    {
        if (original_image[i] > max_value)
            max_value = original_image[i];
        i++;
    }
    return_value = 10 * log10(pow(max_value, 2) / mse_value);

    return return_value;
}

/* Structural Similarity */
float ssim(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t heigth, uint32_t window_size)
{
    float luminance, constrast, structure;
    float return_value;
    return return_value;
}