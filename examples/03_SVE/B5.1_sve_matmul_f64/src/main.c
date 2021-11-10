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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matmul.h"

#define M 128
#define K 128
#define N 128

#define ERROR_TOLERANCE 0.01

int main()
{
    float64_t input_left[M*K];
    float64_t input_right[K*N];

    float64_t output_ref[M*N] = {0};
    float64_t output_opt[M*N] = {0};

    srand((unsigned int)time(0));

    for(int64_t x = 0; x < (M * K); ++x)
    {
        input_left[x] = ((double)(rand() % 2000000) / 100.f) - 10000.0;
    }
    for(int64_t x = 0; x < (K * N); ++x)
    {
        input_right[x] = ((double)(rand() % 2000000) / 100.f) - 10000.0;
    }

    calc_matmul_ref(M, K, N, input_left, input_right, output_ref);
    calc_matmul_opt(M, K, N, input_left, input_right, output_opt);

    printf("ERROR TOLERANCE = %f%%\n", (float)ERROR_TOLERANCE);
    uint32_t error = 0;
    for(int64_t i = 0; i < (M * N); ++i)
    {
        if((fabs((output_ref[i] - output_opt[i]) / output_ref[i]) * 100.0) > ERROR_TOLERANCE)
        {
            error++;
        }
    }

    if(!error)
    {
        printf("TEST PASSED\n");
    }
    else
    {
        printf("TEST FAILED\n");
        printf("NUMBER OF ERRORS: %d\n", error);
    }

    return 0;
}
