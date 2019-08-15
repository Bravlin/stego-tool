#include "stego.h"

unsigned char mask8(char shift)
{
    if (shift > 0)
        return 0b11111111 >> shift;
    else
        return 0b11111111 << (shift * -1);
}

uint32_t mask32(char shift)
{
    if (shift > 0)
        return 0xFF >> shift;
    else
        return 0xFF << (shift * -1);
}

int hide_text(uint32_t* pixels, unsigned int pixel_size, uint32_t x, uint32_t y, unsigned int k, unsigned char* text)
{
    unsigned char mask;
    unsigned int i;
    unsigned char to_hide = 0;
    unsigned int pixel = 0;
    unsigned int image_size = x * y;
    unsigned int text_size = (strlen(text) + 1) * 8;
    unsigned int available_bits = image_size * k;

    if (k < 1)
        return 0; // Check for a proper return code later
    if (pixels == NULL)
        return 0;
    if (text == NULL)
        return 0;

    // Think a formula for getting k directly
    while (available_bits < text_size)
    {
        k++;
        available_bits = image_size * k;
    }

    while (text[to_hide] != '\0')
    {
        for (i = sizeof(unsigned char) - 1; i >= 0; i--)
        {
            pixels[pixel] &= mask32(-1);
            pixels[pixel] |= (text[to_hide] >> i) & mask8(7);
            pixel++;
        }
        to_hide++;
    }
}