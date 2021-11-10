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

int main()
{
    int16_t input[INPUT_SIZE];
    int16_t coeff[T];
    int16_t result_ref[N];
    int16_t result_opt[N];

    srand((unsigned int)time(0));

    for(int32_t i = 0; i < INPUT_SIZE; ++i)
    {
        input[i] = (int16_t)((rand() % 2048) - 1024);
    }
    for(int32_t i = 0; i < T; ++i)
    {
        coeff[i] = (int16_t)((rand() % 2048) - 1024);
    }

    calc_fir_ref(N, T, input, coeff, result_ref);
    calc_fir_opt(N, T, input, coeff, result_opt);

    int error = 0;
    for(int32_t i = 0; i < N; ++i)
    {
        if(result_ref[i] != result_opt[i])
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
