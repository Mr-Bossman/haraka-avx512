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

#include <stdio.h>
#include "haraka.h"
#include <stdint.h>
u512 rc256[20];
u512 rc[10];
u512 rc0[10] = {0};
u512 MIX_4;
u512 MIX_2;
void load_constants() {
  //MIX_4 = _mm512_set_epi32(3,11,7,15,8,0,12,4,9,1,13,5,2,10,6,14); stupid thing goes the wrong way
	MIX_4 = _mm512_set_epi32(14,6,10,2,5,13,1,9,4,12,0,8,15,7,11,3);
	MIX_2 = _mm512_set_epi32(7+8,3+8,6+8,2+8,5+8,1+8,4+8,0+8,7,3,6,2,5,1,4,0);

  rc[0] = _mm512_set_epi32(0x0ed6eae6,0x2e7b4f08,0xbbf3bcaf,0xfd5b4f79,0x3402de2d,0x53f28498,0xcf029d60,0x9f029114,0x8b66b4e1,0x88f3a06b,0x640f6ba4,0x2f08f717,0x0684704c,0xe620c00a,0xb2c5fef0,0x75817b9d);
  rc[1] = _mm512_set_epi32(0x2924d9b0,0xafcacc07,0x675ffde2,0x1fc70b3b,0x67c28f43,0x5e2e7cd0,0xe2412761,0xda4fef1b,0x7eeacdee,0x6e9032b7,0x8d5335ed,0x2b8a057b,0xcbcfb0cb,0x4872448b,0x79eecd1c,0xbe397044);
  rc[2] = _mm512_set_epi32(0xfa0478a6,0xde6f5572,0x4aaa9ec8,0x5c9d2d8a,0xb2cc0bb9,0x941723bf,0x69028b2e,0x8df69800,0x1c30bf84,0xd4b7cd64,0x5b2a404f,0xad037e33,0xab4d63f1,0xe6867fe9,0xecdb8fca,0xb9d465ee);
  rc[3] = _mm512_set_epi32(0x21025ed8,0x9d199c4f,0x78a2c7e3,0x27e593ec,0xaf044988,0x4b050084,0x5f9600c9,0x9ca8eca6,0x1ea10344,0xf449a236,0x32d611ae,0xbb6a12ee,0xdfb49f2b,0x6b772a12,0x0efa4f2e,0x29129fd4);
  rc[4] = _mm512_set_epi32(0x9223973c,0x226b68bb,0x2caf92e8,0x36d1943a,0x5aca45c2,0x21300443,0x81c29153,0xf6fc9ac6,0x6260700d,0x6186b017,0x37f2efd9,0x10307d6b,0xbf3aaaf8,0xa759c9b7,0xb9282ecd,0x82d40173);
  rc[5] = _mm512_set_epi32(0x734bd3dc,0xe2e4d19c,0x2db91a4e,0xc72bf77d,0xbb606268,0xffeba09c,0x83e48de3,0xcb2212b1,0xdb863ce5,0xaef0c677,0x933dfddd,0x24e1128d,0xd3bf9238,0x225886eb,0x6cbab958,0xe51071b4);
  rc[6] = _mm512_set_epi32(0xcda75a17,0xd6de7d77,0x6d1be5b9,0xb88617f9,0x6df3614b,0x3c755977,0x8e5e2302,0x7eca472c,0xdba775a8,0xe707eff6,0x03b231dd,0x16eb6899,0x43bb47c3,0x61301b43,0x4b1415c4,0x2cb3924e);
  rc[7] = _mm512_set_epi32(0xf0b1a5a1,0x96e90cab,0x80bbbabc,0x63a4a350,0x2cee0c75,0x00da619c,0xe4ed0353,0x600ed0d9,0xcb1e6950,0xf957332b,0xa2531159,0x3bf327c1,0xec6b43f0,0x6ba8e9aa,0x9d6c069d,0xa946ee5d);
  rc[8] = _mm512_set_epi32(0x26f65241,0xcbe55438,0x43ce5918,0xffbaafde,0x34bb8a5b,0x5f427fd7,0xaeb6b779,0x360a16f6,0x17bb8f38,0xd554a40b,0x8814f3a8,0x2e75b442,0xae3db102,0x5e962988,0xab0dde30,0x938dca39);
  rc[9] = _mm512_set_epi32(0x756acc03,0x02288288,0x4ad6bdfd,0xe9c59da1,0xa0c1613c,0xba7ed22b,0xc173bc0f,0x48a659cf,0xae51a51a,0x1bdff7be,0x40c06e28,0x22901235,0x4ce99a54,0xb9f3026a,0xa2ca9cf7,0x839ec978);


  rc256[0] = _mm512_set_epi32(0x8b66b4e1,0x88f3a06b,0x640f6ba4,0x2f08f717,0x0684704c,0xe620c00a,0xb2c5fef0,0x75817b9d,0x8b66b4e1,0x88f3a06b,0x640f6ba4,0x2f08f717,0x0684704c,0xe620c00a,0xb2c5fef0,0x75817b9d);
  rc256[1] = _mm512_set_epi32(0x7eeacdee,0x6e9032b7,0x8d5335ed,0x2b8a057b,0xcbcfb0cb,0x4872448b,0x79eecd1c,0xbe397044,0x7eeacdee,0x6e9032b7,0x8d5335ed,0x2b8a057b,0xcbcfb0cb,0x4872448b,0x79eecd1c,0xbe397044);
  rc256[2] = _mm512_set_epi32(0x1c30bf84,0xd4b7cd64,0x5b2a404f,0xad037e33,0xab4d63f1,0xe6867fe9,0xecdb8fca,0xb9d465ee,0x1c30bf84,0xd4b7cd64,0x5b2a404f,0xad037e33,0xab4d63f1,0xe6867fe9,0xecdb8fca,0xb9d465ee);
  rc256[3] = _mm512_set_epi32(0x1ea10344,0xf449a236,0x32d611ae,0xbb6a12ee,0xdfb49f2b,0x6b772a12,0x0efa4f2e,0x29129fd4,0x1ea10344,0xf449a236,0x32d611ae,0xbb6a12ee,0xdfb49f2b,0x6b772a12,0x0efa4f2e,0x29129fd4);
  rc256[4] = _mm512_set_epi32(0x6260700d,0x6186b017,0x37f2efd9,0x10307d6b,0xbf3aaaf8,0xa759c9b7,0xb9282ecd,0x82d40173,0x6260700d,0x6186b017,0x37f2efd9,0x10307d6b,0xbf3aaaf8,0xa759c9b7,0xb9282ecd,0x82d40173);
  rc256[5] = _mm512_set_epi32(0xdb863ce5,0xaef0c677,0x933dfddd,0x24e1128d,0xd3bf9238,0x225886eb,0x6cbab958,0xe51071b4,0xdb863ce5,0xaef0c677,0x933dfddd,0x24e1128d,0xd3bf9238,0x225886eb,0x6cbab958,0xe51071b4);
  rc256[6] = _mm512_set_epi32(0xdba775a8,0xe707eff6,0x03b231dd,0x16eb6899,0x43bb47c3,0x61301b43,0x4b1415c4,0x2cb3924e,0xdba775a8,0xe707eff6,0x03b231dd,0x16eb6899,0x43bb47c3,0x61301b43,0x4b1415c4,0x2cb3924e);
  rc256[7] = _mm512_set_epi32(0xcb1e6950,0xf957332b,0xa2531159,0x3bf327c1,0xec6b43f0,0x6ba8e9aa,0x9d6c069d,0xa946ee5d,0xcb1e6950,0xf957332b,0xa2531159,0x3bf327c1,0xec6b43f0,0x6ba8e9aa,0x9d6c069d,0xa946ee5d);
  rc256[8] = _mm512_set_epi32(0x17bb8f38,0xd554a40b,0x8814f3a8,0x2e75b442,0xae3db102,0x5e962988,0xab0dde30,0x938dca39,0x17bb8f38,0xd554a40b,0x8814f3a8,0x2e75b442,0xae3db102,0x5e962988,0xab0dde30,0x938dca39);
  rc256[9] = _mm512_set_epi32(0xae51a51a,0x1bdff7be,0x40c06e28,0x22901235,0x4ce99a54,0xb9f3026a,0xa2ca9cf7,0x839ec978,0xae51a51a,0x1bdff7be,0x40c06e28,0x22901235,0x4ce99a54,0xb9f3026a,0xa2ca9cf7,0x839ec978);
  rc256[10] = _mm512_set_epi32(0x0ed6eae6,0x2e7b4f08,0xbbf3bcaf,0xfd5b4f79,0x3402de2d,0x53f28498,0xcf029d60,0x9f029114,0x0ed6eae6,0x2e7b4f08,0xbbf3bcaf,0xfd5b4f79,0x3402de2d,0x53f28498,0xcf029d60,0x9f029114); // definatly a better way to do this than this
  rc256[11] = _mm512_set_epi32(0x2924d9b0,0xafcacc07,0x675ffde2,0x1fc70b3b,0x67c28f43,0x5e2e7cd0,0xe2412761,0xda4fef1b,0x2924d9b0,0xafcacc07,0x675ffde2,0x1fc70b3b,0x67c28f43,0x5e2e7cd0,0xe2412761,0xda4fef1b);
  rc256[12] = _mm512_set_epi32(0xfa0478a6,0xde6f5572,0x4aaa9ec8,0x5c9d2d8a,0xb2cc0bb9,0x941723bf,0x69028b2e,0x8df69800,0xfa0478a6,0xde6f5572,0x4aaa9ec8,0x5c9d2d8a,0xb2cc0bb9,0x941723bf,0x69028b2e,0x8df69800);
  rc256[13] = _mm512_set_epi32(0x21025ed8,0x9d199c4f,0x78a2c7e3,0x27e593ec,0xaf044988,0x4b050084,0x5f9600c9,0x9ca8eca6,0x21025ed8,0x9d199c4f,0x78a2c7e3,0x27e593ec,0xaf044988,0x4b050084,0x5f9600c9,0x9ca8eca6);
  rc256[14] = _mm512_set_epi32(0x9223973c,0x226b68bb,0x2caf92e8,0x36d1943a,0x5aca45c2,0x21300443,0x81c29153,0xf6fc9ac6,0x9223973c,0x226b68bb,0x2caf92e8,0x36d1943a,0x5aca45c2,0x21300443,0x81c29153,0xf6fc9ac6);
  rc256[15] = _mm512_set_epi32(0x734bd3dc,0xe2e4d19c,0x2db91a4e,0xc72bf77d,0xbb606268,0xffeba09c,0x83e48de3,0xcb2212b1,0x734bd3dc,0xe2e4d19c,0x2db91a4e,0xc72bf77d,0xbb606268,0xffeba09c,0x83e48de3,0xcb2212b1);
  rc256[16] = _mm512_set_epi32(0xcda75a17,0xd6de7d77,0x6d1be5b9,0xb88617f9,0x6df3614b,0x3c755977,0x8e5e2302,0x7eca472c,0xcda75a17,0xd6de7d77,0x6d1be5b9,0xb88617f9,0x6df3614b,0x3c755977,0x8e5e2302,0x7eca472c);
  rc256[17] = _mm512_set_epi32(0xf0b1a5a1,0x96e90cab,0x80bbbabc,0x63a4a350,0x2cee0c75,0x00da619c,0xe4ed0353,0x600ed0d9,0xf0b1a5a1,0x96e90cab,0x80bbbabc,0x63a4a350,0x2cee0c75,0x00da619c,0xe4ed0353,0x600ed0d9);
  rc256[18] = _mm512_set_epi32(0x26f65241,0xcbe55438,0x43ce5918,0xffbaafde,0x34bb8a5b,0x5f427fd7,0xaeb6b779,0x360a16f6,0x26f65241,0xcbe55438,0x43ce5918,0xffbaafde,0x34bb8a5b,0x5f427fd7,0xaeb6b779,0x360a16f6);
  rc256[19] = _mm512_set_epi32(0x756acc03,0x02288288,0x4ad6bdfd,0xe9c59da1,0xa0c1613c,0xba7ed22b,0xc173bc0f,0x48a659cf,0x756acc03,0x02288288,0x4ad6bdfd,0xe9c59da1,0xa0c1613c,0xba7ed22b,0xc173bc0f,0x48a659cf);


}//duck its endien ness // good programers make programs to make their programs i fixed it


int test_implementations() {
	alignas(64)  unsigned char in[64*8];
	alignas(64)  unsigned char out256[32*8];
	alignas(64)  unsigned char out512[32*8];
	alignas(64)  unsigned char testvector256[32] = {0x80, 0x27, 0xcc, 0xb8, 0x79, 0x49, 0x77, 0x4b,
                                     0x78, 0xd0, 0x54, 0x5f, 0xb7, 0x2b, 0xf7, 0x0c,
                                     0x69, 0x5c, 0x2a, 0x09, 0x23, 0xcb, 0xd4, 0x7b,
                                     0xba, 0x11, 0x59, 0xef, 0xbf, 0x2b, 0x2c, 0x1c};

	alignas(64)  unsigned char testvector512[32] = {0xbe, 0x7f, 0x72, 0x3b, 0x4e, 0x80, 0xa9, 0x98,
                                    0x13, 0xb2, 0x92, 0x28, 0x7f, 0x30, 0x6f, 0x62,
                                    0x5a, 0x6d, 0x57, 0x33, 0x1c, 0xae, 0x5f, 0x34,
                                    0xdd, 0x92, 0x77, 0xb0, 0x94, 0x5b, 0xe2, 0xaa};



  int i;

  // Input for testvector
  for(i = 0; i < 512; i++) {
    in[i] = i % 64;
  }

  load_constants();
  haraka256_4x(out512, in);

  // Verify output
  for(i = 0; i < 32; i++) {
    if (out512[i % 32] != testvector256[i]) {
      printf("Error: testvector incorrect.\n");
      return -1;
    }
  }
  return 0;
}

void haraka256(unsigned char *out, const unsigned char *in) {
	  u512 s,i;
	  s = LOAD (in);
	  i = s;

	  AES2(s, 0);
	  MIX2(s);

	  AES2(s, 1);
	  MIX2(s);

	  AES2(s, 2);
	  MIX2(s);

	  AES2(s, 3);
	  MIX2(s);

	  AES2(s, 4);
	  MIX2(s);

	  s = _mm512_xor_si512(s, i);

	  STORE(out, s);
}



void haraka256_4x(unsigned char *out, const unsigned char *in) {
	  u512 s[2] ,i[2];
	  s[0] = LOAD (in);
	  s[1] = LOAD (in+512);

	  i[0] = s[0];
	  i[1] = s[1];


	  AES2_4x(s, 0);

	  MIX2(s[0]);
	  MIX2(s[1]);

	  AES2_4x(s, 1);

	  MIX2(s[0]);
	  MIX2(s[1]);

	  AES2_4x(s, 2);

	  MIX2(s[0]);
	  MIX2(s[1]);

	  AES2_4x(s, 3);

	  MIX2(s[0]);
	  MIX2(s[1]);

	  AES2_4x(s, 4);

	  MIX2(s[0]);
	  MIX2(s[1]);

	  s[0] = _mm512_xor_si512(s[0], i[0]);
	  s[1] = _mm512_xor_si512(s[1], i[1]);

	  STORE(out, s[0]);
	  STORE(out+512, s[1]);

}

void haraka256_8x(unsigned char *out, const unsigned char *in) {
  haraka256_4x(out, in);
  haraka256_4x(out + 128, in + 128);
}

void haraka512(unsigned char *out, const unsigned char *in) {
  u512 s,i;
  s = LOAD (in);
  i = s;

  AES4(s, 0);
  MIX4(s);

  AES4(s, 2);
  MIX4(s);

  AES4(s, 4);
  MIX4(s);

  AES4(s, 6);
  MIX4(s);

  AES4(s, 8);
  MIX4(s);

  s = _mm512_xor_si512(s, i);

  TRUNCSTORE(out, s);
}

void haraka512_zero(unsigned char *out, const unsigned char *in) {
  u512 s,i;
  s = LOAD (in);
  i = s;
  AES4_zero(s, 0);
  MIX4(s);

  AES4_zero(s, 2);
  MIX4(s);

  AES4_zero(s, 4);
  MIX4(s);

  AES4_zero(s, 6);
  MIX4(s);

  AES4_zero(s, 8);
  MIX4(s);

  s = _mm512_xor_si512(s, i);

  TRUNCSTORE(out, s);
}

void haraka512_4x(unsigned char *out, const unsigned char *in) {
	u512 s[4],i[4];

	s[0] = LOAD(in);
	s[1] = LOAD(in + 64);
	s[2] = LOAD(in + 128);
	s[3] = LOAD(in + 192);
	i[0] = s[0];
	i[1] = s[1];
	i[2] = s[2];
	i[3] = s[3];



  AES4_4x(s, 0);
  MIX4(s[0]);
  MIX4(s[1]);
  MIX4(s[2]);
  MIX4(s[3]);

  AES4_4x(s, 2);
  MIX4(s[0]);
  MIX4(s[1]);
  MIX4(s[2]);
  MIX4(s[3]);

  AES4_4x(s, 4);
  MIX4(s[0]);
  MIX4(s[1]);
  MIX4(s[2]);
  MIX4(s[3]);

  AES4_4x(s, 6);
  MIX4(s[0]);
  MIX4(s[1]);
  MIX4(s[2]);
  MIX4(s[3]);

  AES4_4x(s, 8);
  MIX4(s[0]);
  MIX4(s[1]);
  MIX4(s[2]);
  MIX4(s[3]);


  s[0] = _mm512_xor_si512(s[0], i[0]);
  s[1] = _mm512_xor_si512(s[1], i[1]);
  s[2] = _mm512_xor_si512(s[2], i[2]);
  s[3] = _mm512_xor_si512(s[3], i[3]);


  TRUNCSTORE(out, s[0]);
  TRUNCSTORE(out + 32, s[1]);
  TRUNCSTORE(out + 64, s[2]);
  TRUNCSTORE(out + 96, s[3]);
}

void haraka512_8x(unsigned char *out, const unsigned char *in) {
  haraka512_4x(out, in);
  haraka512_4x(out + 128, in + 256);
}
