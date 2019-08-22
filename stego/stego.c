#include "stego.h"

#define BITS8 0b11111111
#define LAST_BIT 0b00000001
#define CHAR_SIZE 8
#define MAX_TEXT_SIZE 256

int hide_text(const unsigned char* text, unsigned int pixel_size, uint32_t x, uint32_t y,
        uint8_t k, uint32_t* pixels)
{
    int8_t i;
    unsigned long int padding_left;
    uint8_t pixels_per_char;
    unsigned long int jump;
    uint8_t to_hide = 0;
    unsigned long int pixel = 0;
    unsigned long int text_size_chars = strlen(text);
    unsigned long int text_size_bits = (text_size_chars + 1) * CHAR_SIZE;
    unsigned long int usable_pixels = x * y - 3;

    if (k < 1 || k > pixel_size || pixels == NULL || text == NULL
            || text_size_chars > MAX_TEXT_SIZE)
        return 0;

    // Checks if k bits per pixel are enough.
    // If not, then calculate an appropiate minimum size for k
    if (usable_pixels * k < text_size_bits)
        k = text_size_bits / usable_pixels + (text_size_bits % usable_pixels != 0);

    // Checks if more bits than what is allowed to hide per pixel are needed 
    if (k > CHAR_SIZE)
        return 0;

    // Hides k within the first 3 pixels
    for (i = 2; i >= 0; i--)
    {
        pixels[pixel] &= 0b11111110;
        pixels[pixel] |= ((k - 1) >> i) & LAST_BIT;
        pixel++; 
    }

    // Hides the text length as the first character
    hide_char(text_size_chars, k, pixels, &pixel);

    pixels_per_char = CHAR_SIZE / k + (CHAR_SIZE % k != 0);
    padding_left = usable_pixels - (text_size_chars + 1) * pixels_per_char;
    while (text[to_hide] != '\0')
    {
        if (padding_left > 0)
        {
            jump = padding_jump(pixels[pixel - 1], k, padding_left);
            pixel += jump;
            padding_left -= jump;
        }
        hide_char(text[to_hide], k, pixels, &pixel);
        to_hide++;
    }

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
    pixels[*pixel] &= BITS8 << shift;
    pixels[*pixel] |= c & ~(BITS8 << shift);
    (*pixel)++;
}

int get_text(const uint32_t* pixels, uint32_t x, uint32_t y, unsigned char* text)
{
    int8_t i;
    uint8_t to_get;
    uint8_t text_size_chars;
    uint8_t pixels_per_char;
    unsigned char last_char;
    unsigned long int jump;
    unsigned long int padding_left;
    uint8_t k = 0;
    unsigned long int pixel = 0;
    unsigned long int usable_pixels = x * y - 3;

    if (pixels == NULL || text == NULL)
        return 0;

    // Recovers k from the first 3 pixels
    for (i = 2; i >= 0; i--)
    {
        k |= (pixels[pixel] & LAST_BIT) << i;
        pixel++; 
    }
    k++;

    // Recovers the text size
    get_char(pixels, k, &text_size_chars, &pixel);

    pixels_per_char = CHAR_SIZE / k + (CHAR_SIZE % k != 0);
    padding_left = usable_pixels - (text_size_chars + 1) * pixels_per_char;
    for (to_get = 0; to_get < text_size_chars; to_get++)
    {
        if (padding_left > 0)
        {
            jump = padding_jump(pixels[pixel - 1], k, padding_left);
            pixel += jump;
            padding_left -= jump;
        }
        get_char(pixels, k, &(text[to_get]), &pixel);
    }
    text[to_get] = '\0';

    return 1;
}

void get_char(const uint32_t* pixels, uint8_t k, unsigned char* c, unsigned long int* pixel)
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
    *c |= pixels[*pixel] & ~(BITS8 << shift);
    (*pixel)++;
}

// Calculates a pseudo-random jump
unsigned long int padding_jump(uint32_t prev_pixel, uint8_t k, unsigned long int padding_left)
{
    unsigned char xor = padding_left ^ prev_pixel;
    unsigned long int jump;
    if (xor % 2 == 1)
    {
        jump = xor % (CHAR_SIZE * k) + 1;
        jump %= padding_left + 1;
    }
    else
        jump = 0;
    return jump;
}
