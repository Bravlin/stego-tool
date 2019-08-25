#include "bitmap.h"
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int get_data(int fd, uint32_t *x, uint32_t *y, uint16_t *pixel_size, uint16_t *pixel_bytes, uint8_t *padding)
{
    char buff[1024];
    uint32_t offset;
    read(fd, buff, 2); // must be BM
    if (buff[0] != 'B' || buff[1] != 'M') return 0;
    read(fd, buff, 4); // discard the file size
    read(fd, buff, 4); // discard reserved values
    read(fd, &offset, 4); // get the pixel data offset
    read(fd, buff, 4); // discard image header size
    read(fd, x, 4); // get the width in pixels
    read(fd, y, 4); // get the height in pixels
    read(fd, buff, 2); // discard number of color planes
    read(fd, pixel_size, 2); // get the bits per pixel
    if (*pixel_size < 8) return 0;
    read(fd, buff, offset - 30); // discard bytes until the pixel data
    *pixel_bytes = *pixel_size / 8;
    *padding = ((*x) * (*pixel_bytes)) % 4;
    return 1;
}

int get_pixels(char *filename, uint32_t *pixels, uint32_t *x, uint32_t *y, uint16_t *pixel_size)
{
    char buff[1024];
    uint8_t padding;
    uint16_t pixel_bytes;
    uint32_t i, j;
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
        return 0;
    if (!get_data(fd, x, y, pixel_size, &pixel_bytes, &padding))
        return 0;
    for (j = 0; j < *y; j++)
    {
        for (i = 0; i < *x; i++)
            read(fd, pixels + j * (*x) + i, pixel_bytes); // get a pixel
        read(fd, buff, padding); // discard padding
    } 
    close(fd);
    return 1;
}

int set_pixels(char *filename, uint32_t *pixels)
{
    char buff[1024], filename_h;
    uint8_t padding;
    uint16_t pixel_size, pixel_bytes;
    uint32_t offset, x, y, i, j;
    strcpy(filename_h, filename);
    filename_h[strlen(filename_h) - 4] = '\0'; // remove extension
    strcat(filename_h, "_h.bmp");
    system("cp %s %s", filename, filename_h);
    int fd = open(filename_h, O_RDWR);
    if (fd < 0)
        return 0;
    if (!get_data(fd, &x, &y, &pixel_size, &pixel_bytes, &padding))
        return 0;
    for (j = 0; j < y; j++)
    {
        for (i = 0; i < x; i++)
            write(fd, pixels + j * x + i, pixel_bytes); // put a pixel
        read(fd, buff, padding); // discard padding
    }
}