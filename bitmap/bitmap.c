#include "bitmap.h"
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int get_pixels(char *filename, uint32_t *pixels, uint32_t *x, uint32_t *y, uint16_t *pixel_size)
{
    char buff[1024];
    uint8_t padding;
    uint16_t pixel_bytes;
    uint32_t offset, i, j;
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
        return 0;
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
    pixel_bytes = *pixel_size / 8;
    padding = ((*x) * (*pixel_bytes)) % 4;
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
    char buff[1024], filename_h[1024];
    uint8_t padding;
    uint16_t pixel_size, pixel_bytes;
    uint32_t offset, x, y, i, j;
    int fd = open(filename, O_RDONLY);
    strcpy(filename_h, filename);
    filename_h[strlen(filename_h) - 4] = '\0'; // remove extension
    strcat(filename_h, "_h.bmp");
    int fd_h = open(filename_h, O_WRONLY);
    if (fd < 0)
        return 0;
    read(fd, buff, 2); // must be BM
    if (buff[0] != 'B' || buff[1] != 'M') return 0;
    write(fd_h, buff, 2); // copy BM
    read(fd, buff, 4); // get the file size
    write(fd_h, buff, 4); // copy the file size
    read(fd, buff, 4); // get reserved values
    write(fd_h, buff, 4); // copy reserved values
    read(fd, &offset, 4); // get the pixel data offset
    write(fd_h, &offset, 4); // copy the pixel data offset
    read(fd, buff, 4); // get image header size
    write(fd_h, buff, 4); // copy image header size
    read(fd, &x, 4); // get the width in pixels
    write(fd_h, &x, 4); // copy the width in pixels
    read(fd, &y, 4); // get the height in pixels
    write(fd_h, &y, 4); // copy the height in pixels
    read(fd, buff, 2); // get number of color planes
    write(fd_h, buff, 2); // copy number of color planes
    read(fd, &pixel_size, 2); // get the bits per pixel
    if (pixel_size < 8) return 0;
    write(fd_h, &pixel_size, 2); // copy the bits per pixel
    read(fd, buff, offset - 30); // get bytes until the pixel data
    write(fd, buff, offset - 30); // copy bytes until the pixel data
    pixel_bytes = pixel_size / 8;
    padding = (x * pixel_bytes) % 4;
    for (j = 0; j < y; j++)
    {
        for (i = 0; i < x; i++)
            write(fd_h, pixels + j * x + i, pixel_bytes); // put a pixel
        read(fd, buff, padding); // get padding
        write(fd, buff, padding); // copy padding
    }
    while (i = read(fd, buff, sizeof(buff)) > 0)
        write(fd_h, buff, i);
    close(fd);
    return 1;
}