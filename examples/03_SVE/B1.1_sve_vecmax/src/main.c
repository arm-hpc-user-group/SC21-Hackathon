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
#include "vecmax.h"

#define WORKLOAD_SIZE 1024

int main()
{
    int16_t arrayD[WORKLOAD_SIZE];

    int16_t max_elem_ref;
    int16_t max_elem_opt;
    uint16_t max_index_ref;
    uint16_t max_index_opt;

    srand((unsigned int)time(0));

    for(int i = 0; i < WORKLOAD_SIZE; ++i)
    {
        arrayD[i] = (int16_t)((rand() % 20000) - 10000);
    }

    calc_vecmax_first_ref(arrayD, WORKLOAD_SIZE, &max_elem_ref, &max_index_ref);
    calc_vecmax_first_opt(arrayD, WORKLOAD_SIZE, &max_elem_opt, &max_index_opt);

    if((max_elem_ref == max_elem_opt) && (max_index_ref == max_index_opt))
    {
        printf("TEST PASSED\n");
    }
    else
    {
        printf("TEST FAILED\n");
        printf("REFERENCE MAXIMUM ELEMENT = %d\n", max_elem_ref);
        printf("OPTIMIZED MAXIMUM ELEMENT = %d\n", max_elem_opt);
        printf("REFERENCE MAXIMUM INDEX = %u\n", max_index_ref);
        printf("OPTIMIZED MAXIMUM INDEX = %u\n", max_index_opt);
    }

    return 0;
}
