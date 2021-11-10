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

#include <inttypes.h>
#include "vecmul.h"

#define MIN_INT16        ((int32_t)(0xFFFF8000))
#define MAX_INT16        ((int32_t)(0x00007FFF))

int16_t Sat(int32_t a)
{
    int16_t b = (int16_t)a;
    if(a > MAX_INT16){b = (int16_t)MAX_INT16;}
    if(a < MIN_INT16){b = (int16_t)MIN_INT16;}
    return b;
}

void calc_vecmul_ref(int64_t n, cplx_int16_t* restrict a, cplx_int16_t* restrict b, cplx_int16_t* c)
{
    for(int64_t i = 0; i < n; ++i)
    {
        c[i].re = Sat((((int32_t)(a[i].re * b[i].re) + (int32_t)0x4000) >> 15) -
                      (((int32_t)(a[i].im * b[i].im) + (int32_t)0x4000) >> 15));
        c[i].im = Sat((((int32_t)(a[i].re * b[i].im) + (int32_t)0x4000) >> 15) +
                      (((int32_t)(a[i].im * b[i].re) + (int32_t)0x4000) >> 15));
    }
}
