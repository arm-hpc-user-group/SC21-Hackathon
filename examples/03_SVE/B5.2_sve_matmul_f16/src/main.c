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
#include <arm_fp16.h>
#include "matmul.h"

#define M 96
#define K 96
#define N 96

#define ERROR_TOLERANCE 0.5

int main()
{
    float16_t input_left[M*K];
    float16_t input_right[K*N];

    float16_t output_ref[M*N] = {0};
    float16_t output_opt[M*N] = {0};

    srand((unsigned int)time(0));

    for(int64_t x = 0; x < (M * K); ++x)
    {
        input_left[x] = (float16_t)(((float)(rand() % 2000) / 100.f) - 5.f);
    }
    for(int64_t x = 0; x < (K * N); ++x)
    {
        input_right[x] = (float16_t)(((float)(rand() % 2000) / 100.f) - 5.f);
    }

    calc_matmul_ref(M, K, N, input_left, input_right, output_ref);
    calc_matmul_opt(M, K, N, input_left, input_right, output_opt);

    printf("ERROR TOLERANCE = %f%%\n", (float16_t)ERROR_TOLERANCE);
    uint32_t error = 0;
    for(int64_t i = 0; i < (M * N); ++i)
    {
        if((fabsf((output_ref[i] - output_opt[i]) / output_ref[i]) * 100.0) > ERROR_TOLERANCE)
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
