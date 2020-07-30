/*
Timing code for optimized implementation of Haraka.
*/

#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <stdint.h>
#include "haraka.h"
#include "timing.h"
#include <stdalign.h>


static void phex(uint8_t* str)
{
    uint8_t len = 64;

    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%lx, ", str[i]);
    printf("\n");
}
int main() {
 	alignas(64) uint8_t a[64];
	alignas(64) uint8_t b[64] = {0};
	for(int i = 0; i <64;i++)a[i] = i;
	load_constants();
	haraka512(b,a);
phex(b);
}
