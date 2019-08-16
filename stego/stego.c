#include "stego.h"

#define BITS8 0b11111111 

int hide_text(uint32_t* pixels, unsigned int pixel_size, uint32_t x, uint32_t y, uint8_t k, unsigned char* text)
{
    int8_t i;
    uint8_t shift;
    uint8_t pixels_per_char;
    uint8_t rest;
    unsigned long int available_bits;
    int64_t to_hide = -1;
    unsigned int pixel = 0;
    unsigned long int text_size = (strlen(text) + 1) * 8;
    unsigned long int total_pixels = x * y;
    

    if (k < 1)
        return 0; // Check for a proper return code later
    if (pixels == NULL)
        return 0;
    if (text == NULL)
        return 0;

    // Checks if k bits per pixel are enough.
    // If not, then calculate an appropiate minimum size for k
    if ((total_pixels - 3) * k < text_size)
    {  
        available_bits = (total_pixels - 3) * 8;
        k = (available_bits % text_size == 0) ? available_bits / text_size : available_bits / text_size + 1;
    }

    // Hides k within the first 3 pixels
    for (i = 2; i >= 0; i--)
    {
        pixels[pixel] &= 0b11111110;
        pixels[pixel] |= ((k - 1) >> i) & 0b00000001;
        pixel++; 
    }

    pixels_per_char = 8 / k;
    rest = 8 % k;
    do
    {
        to_hide++;
        shift = 8 - k;
        for (i = pixels_per_char - 1; i >= 0; i--)
        {
            pixels[pixel] &= BITS8 << k;
            pixels[pixel] |= (text[to_hide] >> shift) & ~(BITS8 << k);
            pixel++;
            shift -= k;
        }
        if (rest > 0)
        {
            pixels[pixel] &= BITS8 << rest;
            pixels[pixel] |= text[to_hide] & ~(BITS8 << rest);
            pixel++;
        } 
    }
    while (text[to_hide] != '\0');
    return 1;
}

int get_text(uint32_t* pixels, unsigned char* text)
{
    int8_t i;
    uint8_t shift;
    uint8_t mask;
    uint8_t pixels_per_char;
    uint8_t rest;
    uint8_t k = 0;
    int64_t to_get = -1;
    unsigned int pixel = 0;

    // Recovers k from the first 3 pixels
    for (i = 2; i >= 0; i--)
    {
        k |= (pixels[pixel] & 0b00000001) << i;
        pixel++; 
    }
    k++;
    if (k > 8)
        return 0;

    pixels_per_char = 8 / k;
    rest = 8 % k;
    do
    {
        to_get++;
        mask = BITS8;
        text[to_get] &= ~(mask);
        shift = 8 - k;
        for (i = pixels_per_char - 1; i >= 0; i--)
        {
            text[to_get] |= (pixels[pixel] << shift) & mask; 
            pixel++;
            shift -= k;
            mask >>= k;
        }
        if (rest > 0)
        {
            text[to_get] |= pixels[pixel] & ~(BITS8 << rest);
            pixel++;
        }
    }
    while (text[to_get] != '\0');
    return 1;
}