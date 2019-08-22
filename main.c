#include <stdio.h>
#include "stego/stego.h"
#include "bitmap/bitmap.h"

int main(int argc, char **argv)
{
    uint32_t pixels[2073600];
    uint32_t x, y;
    uint16_t pixel_size;
    char action = argv[1][0];
    char *img_file = argv[2];
    if (action == 'h' && argc == 5)
    {
        char *text = argv[3];
        uint8_t k = argv[4][0] - '0';
        get_pixels(img_file, pixels, &x, &y, &pixel_size);
        hide_text(text, pixel_size, x, y, k, pixels);
        set_pixels(img_file, pixels);
    }
    else if (action == 'g' && argc == 3)
    {
        char text[256];
        get_pixels(img_file, pixels, &x, &y, &pixel_size);
        get_text(pixels, x, y, text);
        printf("%s\n", text);
    }
}