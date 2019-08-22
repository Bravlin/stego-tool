#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int hide_text(const unsigned char*, unsigned int, uint32_t, uint32_t, uint8_t, uint32_t*);
void hide_char(unsigned char, uint8_t, uint32_t*, unsigned long int*);
int get_text(const uint32_t*, uint32_t, uint32_t, unsigned char*);
void get_char(const uint32_t*, uint8_t, unsigned char*, unsigned long int*);
unsigned long int padding_jump(uint32_t, uint8_t, unsigned long int);