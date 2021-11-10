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

typedef struct {
    float32_t re;
    float32_t im;
} Complex_t;

// Complex dot product
// (a+ib)*(c+id) = (ac - bd) + i(ad + bc)
void complex_dot_product(Complex_t c[SIZE], Complex_t a[SIZE], Complex_t b[SIZE])
{
    uint32_t vl = svcntw();

    svbool_t p32_all = svptrue_b32();

    for (int i=0; i<SIZE; i+=vl/2) {
        svfloat32_t va = svld1(p32_all, (float32_t*)&a[i]);
        svfloat32_t vb = svld1(p32_all, (float32_t*)&b[i]);
        svfloat32_t vc = svld1(p32_all, (float32_t*)&c[i]);

        vc = svcmla_m(p32_all, vc, va, vb, 0);  //c += a * b
        vc = svcmla_m(p32_all, vc, va, vb, 90);

        svst1(p32_all, (float32_t*)&c[i], vc);
    }
}

int main()
{
    Complex_t a[SIZE] = {{1,2}, {2,3}, {-1,1}, {2,0}, {-2,1}}; //initializes first 5 numbers
    Complex_t b[SIZE] = {{2,2}, {3,2}, {1,-2}, {0,1}, {2,-3}}; //initializes first 5 numbers
    Complex_t c[SIZE] = {{0,0}}; //initializes all numbers to 0,0

    complex_dot_product(c, a, b);

    for (int i=0; i<5; i++) {
        printf("(%.1f+i%.1f).(%.1f+i%.1f) = (%.1f+i%.1f)\n", a[i].re, a[i].im, b[i].re, b[i].im, c[i].re, c[i].im);
    }
    return 0;
}
