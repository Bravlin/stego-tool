#include "stego.h"

#define mask8(shift) (shift >= 0) ? 0b11111111 >> shift : 0b11111111 << (shift * -1)
#define mask32(shift) (shift >= 0) ? 0xFF >> shift : 0xFF << (shift * -1)
#define BITS8 0b11111111 

/*
unsigned char mask8(char shift)
{
    if (shift > 0)
        return 0b11111111 >> shift;
    else
        return 0b11111111 << (shift * -1);
}
*/

/*
uint32_t mask32(char shift)
{
    if (shift > 0)
        return 0xFF >> shift;
    else
        return 0xFF << (shift * -1);
}
*/

int hide_text(uint32_t* pixels, unsigned int pixel_size, uint32_t x, uint32_t y, uint8_t k, unsigned char* text)
{
    int8_t i;
    uint8_t pixels_per_char;
    uint8_t rest;
    uint8_t to_hide = 0;
    unsigned int pixel = 0;
    unsigned long int text_size = (strlen(text) + 1) * 8;
    unsigned long int total_pixels = x * y;
    unsigned long int image_size;

    if (k < 1)
        return 0; // Check for a proper return code later
    if (pixels == NULL)
        return 0;
    if (text == NULL)
        return 0;

    // Checks if k bits per pixel are enough.
    // If not, then calculate an appropiate minimum size for k
    if (total_pixels * k < text_size)
    {  
        image_size = total_pixels * pixel_size;
        k = (text_size % image_size == 0) ? text_size / image_size : text_size / image_size + 1;
    }

    pixels_per_char = 8 / k;
    rest = 8 % k;
    do
    {
        for (i = pixels_per_char - 1; i >= 0; i--)
        {
            pixels[pixel] &= BITS8 << k;
            pixels[pixel] |= (text[to_hide] >> (i * k)) & ~(BITS8 << k);
            pixel++;
        }
        if (rest > 0)
        {
            pixels[pixel] &= BITS8 << rest;
            pixels[pixel] |= (text[to_hide] >> (i * rest)) & ~(BITS8 << rest);
            pixel++;
        }
        to_hide++;
    }
    while (text[to_hide] != '\0');
}

int get_text(uint32_t* pixels, uint8_t k, unsigned char* text)
{
    int8_t i;
    int8_t to_get = -1;
    uint8_t pixels_per_char;
    uint8_t rest;
    unsigned int pixel = 0;
    unsigned char hidden_char;

    pixels_per_char = 8 / k;
    rest = 8 % k;
    do
    {
        to_get++;
        hidden_char &= ~(BITS8);
        for (i = pixels_per_char - 1; i >= 0; i--)
        {
            hidden_char |= (pixels[pixel] << (i * k)) & (BITS8 >> (pixels_per_char - 1 - i)); 
            pixel++;
        }
        if (rest > 0)
        {
            hidden_char |= pixels[pixel] & ~(BITS8 << rest);
            pixel++;
        }
        text[to_get] = hidden_char;
    }
    while (text[to_get] != '\0');
}