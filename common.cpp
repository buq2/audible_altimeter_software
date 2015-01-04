#include "common.hh"

uint8_t bit_flip_lookup[16] = {
   0x0, 0x8, 0x4, 0xC,
   0x2, 0xA, 0x6, 0xE,
   0x1, 0x9, 0x5, 0xD,
   0x3, 0xB, 0x7, 0xF };

uint8_t BitFlip(uint8_t n)
{
   // http://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
   //This should be just as fast and it is easier to understand.
   //return (bit_flip_lookup[n%16] << 4) | bit_flip_lookup[n/16];
   return (bit_flip_lookup[n&0x0F] << 4) | bit_flip_lookup[n>>4];
}
