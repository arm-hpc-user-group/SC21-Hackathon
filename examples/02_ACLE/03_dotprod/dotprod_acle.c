//------------------------------------------------------------------------------------
// Copyright (C) Arm Limited, 2019-2020 All rights reserved.
//
// The example code is provided to you as an aid to learning when working
// with Arm-based technology, including but not limited to programming tutorials.
//
// Arm hereby grants to you, subject to the terms and conditions of this Licence,
// a non-exclusive, non-transferable, non-sub-licensable, free-of-charge copyright
// licence, to use, copy and modify the Software solely for the purpose of internal
// demonstration and evaluation.
//
// You accept that the Software has not been tested by Arm therefore the Software
// is provided "as is", without warranty of any kind, express or implied. In no
// event shall the authors or copyright holders be liable for any claim, damages
// or other liability, whether in action or contract, tort or otherwise, arising
// from, out of or in connection with the Software or the use of Software.
//------------------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arm_sve.h>

const uint32_t SIZE = 1024;

// Dot product
// c += a * b
uint32_t dot_product(uint8_t a[SIZE], uint8_t b[SIZE])
{
    uint32_t vl = svcntb();

    svbool_t p8_all = svptrue_b8();
    svbool_t p32_all = svptrue_b32();

    svuint32_t vc = svdup_u32(0);

    for (int i=0; i<SIZE; i+=vl) {
        svuint8_t va = svld1(p8_all, &a[i]);
        svuint8_t vb = svld1(p8_all, &b[i]);

        vc = svdot(vc, va, vb); 
    }
    return svaddv(p32_all, vc);
}

int main()
{
    uint8_t a[SIZE] = {1,2,2,3,-1,1,2,0,-2,1}; //initializes first 5 numbers
    uint8_t b[SIZE] = {2,2,3,2,1,-2,0,1,2,-3}; //initializes first 5 numbers

    uint32_t res = dot_product(a, b);

    printf("Dotprod = %i\n", res);

    return 0;
}
