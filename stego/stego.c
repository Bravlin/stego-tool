#include "stego.h"

#define BITS8 0b11111111
#define LAST_BIT 0b00000001
#define CHAR_SIZE 8

int hide_text(uint32_t* pixels, unsigned int pixel_size, uint32_t x, uint32_t y, uint8_t k,
        unsigned char* text)
{
    int8_t i;
    //uint8_t pixels_per_char;
    int64_t to_hide = -1;
    unsigned long int pixel = 0;
    unsigned long int text_size_chars = (strlen(text) + 1);
    unsigned long int text_size_bits = text_size_chars * CHAR_SIZE;
    unsigned long int usable_pixels = x * y - 3;

    if (k < 1 || k > pixel_size)
        return 0; // Check for a proper return code later
    if (pixels == NULL)
        return 0;
    if (text == NULL)
        return 0;

    // Checks if k bits per pixel are enough.
    // If not, then calculate an appropiate minimum size for k
    if (usable_pixels * k < text_size_bits)
        k = text_size_bits / usable_pixels + (text_size_bits % usable_pixels != 0);

    // Checks if more bits than what is intended to hide per pixel are needed 
    if (k > CHAR_SIZE)
        return 0;

    // Hides k within the first 3 pixels
    for (i = 2; i >= 0; i--)
    {
        pixels[pixel] &= 0b11111110;
        pixels[pixel] |= ((k - 1) >> i) & LAST_BIT;
        pixel++; 
    }

    //pixels_per_char = CHAR_SIZE / k;
    do
    {
        to_hide++;
        hide_char(text[to_hide], k, pixels, &pixel);
    }
    while (text[to_hide] != '\0');

    return 1;
}

void hide_char(unsigned char c, uint8_t k, uint32_t* pixels, unsigned long int* pixel)
{
    uint8_t shift = CHAR_SIZE;
    while (shift > k)
    {
        shift -= k;
        pixels[*pixel] &= BITS8 << k;
        pixels[*pixel] |= (c >> shift) & ~(BITS8 << k);
        (*pixel)++;
    }
    if (shift > 0)
    {
        pixels[*pixel] &= BITS8 << shift;
        pixels[*pixel] |= c & ~(BITS8 << shift);
        (*pixel)++;
    }
}

int get_text(uint32_t* pixels, unsigned char* text)
{
    int8_t i;
    //uint8_t pixels_per_char;
    uint8_t k = 0;
    int64_t to_get = -1;
    unsigned long int pixel = 0;

    if (pixels == NULL)
        return 0;

    // Recovers k from the first 3 pixels
    for (i = 2; i >= 0; i--)
    {
        k |= (pixels[pixel] & LAST_BIT) << i;
        pixel++; 
    }
    k++;

    // Checks for an impossible value for k
    if (k > 8)
        return 0;

    //pixels_per_char = CHAR_SIZE / k;
    do
    {
        to_get++;
        get_char(pixels, k, &(text[to_get]), &pixel);
    }
    while (text[to_get] != '\0');

    return 1;
}

void get_char(uint32_t* pixels, uint8_t k, unsigned char* c, unsigned long int* pixel)
{
    uint8_t mask = BITS8;
    uint8_t shift = CHAR_SIZE;
    *c &= ~(mask);
    while (shift > k)
    {
        shift -= k;
        *c |= (pixels[*pixel] << shift) & mask;
        mask >>= k;
        (*pixel)++;
    }
    if (shift > 0)
    {
        *c |= pixels[*pixel] & ~(BITS8 << shift);
        (*pixel)++;
    }
}

/*
unsigned long int padding_jump(uint32_t* pixels, unsigned char last_char, uint8_t k,
        unsigned long int next_pixel)
{
    unsigned char xor = last_char ^ pixels[next_pixel];
    if ((xor >> (k - 1)) & LAST_BIT)
        return (xor * k);
    return 0;
}
*/