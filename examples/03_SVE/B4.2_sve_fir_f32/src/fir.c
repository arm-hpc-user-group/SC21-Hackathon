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

#include "fir.h"

void calc_fir_ref(int32_t n, int32_t t, float* restrict x, float* restrict h, float* y)
{
    float acc;

    for(int32_t i = 0; i < n; ++i)
    {
        acc = 0;
        for(int32_t j = 0; j < t; ++j)
        {
            acc += x[i+j] * h[j];
        }
        y[i] = acc;
    }
}
