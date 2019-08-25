#include "measure.h"
#include <math.h>

#define k1 0.01
#define k2 0.03
#define L 255

/* Mean Squared Error */
long double mse(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height)
{
    int i, j;
    int aux_1, aux_2 = 0;
    long double return_value;

    for (i = 0; i < height; i++)
    {
        aux_1 = 0;
        for (j = 0; j < width; j++)
            aux_1 += pow(original_image[i * width + j] - stego_image[i * width + j], 2);
        aux_2 += aux_1;
    }
    return_value = aux_2 / (height * width);

    return return_value;
}

/* Peak Signal-to-Noise Ratio */
long double psnr(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height, uint32_t pixel_size)
{
    long double return_value, mse_value = mse(original_image, stego_image, width, height);
    uint32_t actual_max_value = 0, max_value, i = 0, image_size = width * height;

    max_value = pow(2, pixel_size) - 1;
    while (i < image_size && actual_max_value < max_value)
    {
        if (original_image[i] > max_value)
            actual_max_value = original_image[i];
        i++;
    }
    return_value = 10 * log10(pow(max_value, 2) / mse_value);

    return return_value;
}

/* Structural Similarity */
long double ssim(uint32_t *original_image, uint32_t *stego_image, uint32_t width, uint32_t height)
{
    long double return_value, luminance, constrast, structure, c1, c2, c3, mu_x = 0, mu_y = 0, var_x = 0, var_y = 0, covar_xy = 0;
    uint32_t i, image_size = width * height;

    c1 = pow(k1 * L, 2);
    c2 = pow(k2 * L, 2);
    c3 = c2 / 2;

    /* First loop to calculate de mean of each image */
    for (i = 0; i < image_size; i++)
    {
        mu_x += original_image[i];
        mu_y += stego_image[i];
    }
    mu_x /= image_size;
    mu_y /= image_size;

    /* Second loop to calculate variance and covariance */
    for (i = 0; i < image_size; i++)
    {
        var_x += pow(original_image[i] - mu_x, 2);
        var_y += pow(stego_image[i] - mu_y, 2);
        covar_xy += (original_image[i] - mu_x) * (stego_image[i] - mu_y);
    }
    var_x = sqrt(var_x / (image_size - 1));
    var_y = sqrt(var_y / (image_size - 1));
    covar_xy /= image_size;

    luminance = (2 * mu_x * mu_y + c1) / (pow(mu_x, 2) + pow(mu_y, 2) + c1);
    constrast = (2 * var_x * var_y + c2) / (pow(var_x, 2) + pow(var_y, 2) + c2);
    structure = (covar_xy + c3) / (var_x * var_y + c3);

    return_value = luminance * constrast * structure;

    return return_value;
}
