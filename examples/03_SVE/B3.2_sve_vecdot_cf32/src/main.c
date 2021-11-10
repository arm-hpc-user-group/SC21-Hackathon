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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include "vecdot.h"

#define WORKLOAD_SIZE 512
#define ERROR_TOLERANCE 0.05

int main()
{
    cplx_f32_t arrayA[WORKLOAD_SIZE];
    cplx_f32_t arrayB[WORKLOAD_SIZE];

    cplx_f32_t result_ref;
    cplx_f32_t result_opt;

    srand((unsigned int)time(0));

    for (int64_t i = 0; i < WORKLOAD_SIZE; ++i)
    {
        arrayA[i].re = ((float)(rand() % 2000000) / 100.f) - 10000.f;
        arrayA[i].im = ((float)(rand() % 2000000) / 100.f) - 10000.f;
        arrayB[i].re = ((float)(rand() % 2000000) / 100.f) - 10000.f;
        arrayB[i].im = ((float)(rand() % 2000000) / 100.f) - 10000.f;
    }

    calc_vecdot_ref(WORKLOAD_SIZE, arrayA, arrayB, &result_ref);
    calc_vecdot_opt(WORKLOAD_SIZE, arrayA, arrayB, &result_opt);

    double real_error = fabsf((result_ref.re - result_opt.re) / result_ref.re) * 100.0;
    double imag_error = fabsf((result_ref.im - result_opt.im) / result_ref.im) * 100.0;
    printf("ERROR TOLERANCE = %f%%\n", (float)ERROR_TOLERANCE);
    if ((real_error < ERROR_TOLERANCE) && (imag_error < ERROR_TOLERANCE))
    {
        printf("TEST PASSED\n");
    }
    else
    {
        printf("TEST FAILED\n");
    }
    printf("REAL PART ERROR = %f%%\n", real_error);
    printf("IMAGINARY PART ERROR = %f%%\n", imag_error);

    return 0;
}
