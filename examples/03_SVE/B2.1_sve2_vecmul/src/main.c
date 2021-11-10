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
#include "vecmul.h"

#define WORKLOAD_SIZE 512
#define ERROR_TOLERANCE 1

int main()
{
    cplx_int16_t arrayA[WORKLOAD_SIZE];
    cplx_int16_t arrayB[WORKLOAD_SIZE];

    cplx_int16_t result_ref[WORKLOAD_SIZE];
    cplx_int16_t result_opt[WORKLOAD_SIZE];

    srand((unsigned int)time(0));

    for(int32_t i = 0; i < WORKLOAD_SIZE; ++i)
    {
        arrayA[i].re = (int16_t)((rand() % 2048) - 1024);
        arrayA[i].im = (int16_t)((rand() % 2048) - 1024);
        arrayB[i].re = (int16_t)((rand() % 2048) - 1024);
        arrayB[i].im = (int16_t)((rand() % 2048) - 1024);
    }

    calc_vecmul_ref(WORKLOAD_SIZE, arrayA, arrayB, result_ref);
    calc_vecmul_opt(WORKLOAD_SIZE, arrayA, arrayB, result_opt);

    printf("ERROR TOLERANCE = %d\n", (int)ERROR_TOLERANCE);
    uint32_t real_error = 0;
    uint32_t imag_error = 0;
    for(int32_t i = 0; i < WORKLOAD_SIZE; ++i)
    {
        if(abs(result_ref[i].re - result_opt[i].re) > ERROR_TOLERANCE)
        {
            real_error++;
        }
        if(abs(result_ref[i].im - result_opt[i].im) > ERROR_TOLERANCE)
        {
            imag_error++;
        }
    }

    if(!real_error && !imag_error)
    {
        printf("TEST PASSED\n");
    }
    else
    {
        printf("TEST FAILED\n");
        printf("NUMBER OF REAL ERRORS: %d\n", real_error);
        printf("NUMBER OF IMAGINARY ERRORS: %d\n", imag_error);
    }

    return 0;
}
