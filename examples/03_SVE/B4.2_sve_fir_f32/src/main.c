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
#include "fir.h"

#define N 512
#define T 32

#define INPUT_SIZE (N + T - 1)

#define ERROR_TOLERANCE 0.05

int main()
{
    float input[INPUT_SIZE];
    float coeff[T];
    float result_ref[N];
    float result_opt[N];

    srand((unsigned int)time(0));

    for(int32_t i = 0; i < INPUT_SIZE; ++i)
    {
        input[i] = ((float)(rand() % 1000000) / 100.f) - 2500.f;
    }
    for(int32_t i = 0; i < T; ++i)
    {
        coeff[i] = ((float)(rand() % 1000000) / 100.f) - 2500.f;
    }

    calc_fir_ref(N, T, input, coeff, result_ref);
    calc_fir_opt(N, T, input, coeff, result_opt);

    printf("ERROR TOLERANCE = %f%%\n", (float)ERROR_TOLERANCE);
    int error = 0;
    for(int32_t i = 0; i < N; ++i)
    {
        if((fabs((result_ref[i] - result_opt[i]) / result_ref[i]) * 100.0) > ERROR_TOLERANCE)
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
