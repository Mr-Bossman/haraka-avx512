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


static void phex(uint32_t* str)
{
    uint8_t len = 16;

    unsigned char i;
    for (i = 0; i < len; ++i)
        printf("%lx, ", str[i]);
    printf("\n");
}
int main() {
 	alignas(64) uint32_t a[16] = {1,2,4,5,6,7,8,9,10,12,13,14,15};
	alignas(64) uint32_t b[16] = {1};

	load_constants();
	haraka512((uint8_t*)b,(uint8_t*)a);
phex(b);
}
