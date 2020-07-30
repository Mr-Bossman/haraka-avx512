
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
 	alignas(64) uint8_t v[64];
	alignas(64) uint8_t x[64] = {0};
	for(int i = 0; i <64;i++)v[i] = i;
	load_constants();
	haraka512((uint8_t*)x,(uint8_t*)v);
phex(b);
	alignas(64) uint8_t a[64] = {0x6b, 0x2e, 0xe9, 0x73, 0xc1, 0x40, 0x3d, 0x93, 0xbe, 0x9f, 0x7e, 0x17, 0xe2, 0x96, 0x11, 0x2a};
	alignas(64) uint8_t b[64] = {0x6b, 0x2e, 0xe9, 0x73, 0xc1, 0x40, 0x3d, 0x93, 0xbe, 0x9f, 0x7e, 0x17, 0xe2, 0x96, 0x11, 0x2a};
	alignas(64) uint8_t key[64] = {0x6b, 0x2e, 0xe9, 0x73, 0xc1, 0x40, 0x3d, 0x93, 0xbe, 0x9f, 0x7e, 0x17, 0xe2, 0x96, 0x11, 0x2a};

	__m128i k = _mm_loadu_si128((__m128i *) key);
	__m128i test = _mm_loadu_si128((__m128i *) b);
        __m128i m =_mm_aesenc_si128    (test, k);
   	 _mm_storeu_si128((__m128i *) a,m);
 __m512i s = _mm512_load_epi32 (b);
 __m512i i = _mm512_load_epi32 (key);
  s = aes2(s,i);
  _mm512_store_epi32(b,s);
  phex(b,64);
  phex(a,64);
}
