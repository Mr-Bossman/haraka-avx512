
/*
The MIT License (MIT)

Copyright (c) 2016 kste

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Optimized Implementations for Haraka256 and Haraka512
*/
#ifndef HARAKA_H_
#define HARAKA_H_
#include "immintrin.h"
#include "aes.h"
#define NUMROUNDS 5

#ifdef _WIN32
typedef unsigned long long u64;
#else
typedef unsigned long u64;
#endif
typedef __m512i u512;

extern u512 rc[10];
u512 MIX_4;
u512 MIX_2;

#define LOAD(src) _mm512_load_epi32((u512 *)(src))
#define STORE(dest,src) _mm512_store_epi32((u512 *)(dest),src)

#define AES2(s, rci) \
		s = aes2(s,rc[rci]);\
		s = aes2(s,(_mm512_slli_epi32(rc[rci],128))); // wtf without () aorund the shift it gives errors for no reason could gcc expalin
/*
#define AES2_4x(s0, s1, s2, s3, rci) \
  AES2(s0[0], s0[1], rci); \
  AES2(s1[0], s1[1], rci); \
  AES2(s2[0], s2[1], rci); \
  AES2(s3[0], s3[1], rci);

#define AES2_8x(s0, s1, s2, s3, s4, s5, s6, s7, rci) \
  AES2_4x(s0, s1, s2, s3, rci); \
  AES2_4x(s4, s5, s6, s7, rci);
*/
#define AES4(s, rci)\
  s = aes2(aes2(s,rc[rci]),rc[rci+1])

#define AES4_zero(a, rci)\
  a = aes2(aes2(a,rc0[rci]),rc0[rci+1])

#define AES4_4x(s, rci) \
  AES4(s[0], rci); \
  AES4(s[1], rci); \
  AES4(s[2], rci); \
  AES4(s[3], rci);

#define AES4_8x(s0, s1, rci) \
  AES4_4x(s0, rci); \
  AES4_4x(s1, rci);

#define MIX2(s) \
  s = _mm512_permutexvar_epi32 (MIX_2, s);


#define MIX4(s) \
  s = _mm512_permutexvar_epi32 (MIX_4, s);

#define TRUNCSTORE(out, s) \
  *(u64*)(out) = *(((u64*)&s + 1)); \
  *(u64*)(out + 8) = *(((u64*)&s + 3)); \
  *(u64*)(out + 16) = *(((u64*)&s + 4)); \
  *(u64*)(out + 24) = *(((u64*)&s + 6));
void load_constants();
int test_implementations();


void haraka256(unsigned char *out, const unsigned char *in);
/*void haraka256_4x(unsigned char *out, const unsigned char *in);
void haraka256_8x(unsigned char *out, const unsigned char *in);
*/
void haraka512(unsigned char *out, const unsigned char *in);
void haraka512_zero(unsigned char *out, const unsigned char *in);
void haraka512_4x(unsigned char *out, const unsigned char *in);
void haraka512_8x(unsigned char *out, const unsigned char *in);

#endif

