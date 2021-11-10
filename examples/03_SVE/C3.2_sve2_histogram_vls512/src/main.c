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
#include "histogram.h"

#define NUM_RECORDS  262144
#define HIST_ENTRIES 256
extern uint8_t image[NUM_RECORDS];

int main()
{
    uint32_t* histogram_ref = (uint32_t*)calloc(HIST_ENTRIES, sizeof(uint32_t));
    uint32_t* histogram_opt = (uint32_t*)calloc(HIST_ENTRIES, sizeof(uint32_t));

    calc_histogram_ref(histogram_ref, image, NUM_RECORDS);
    calc_histogram_opt(histogram_opt, image, NUM_RECORDS);

    uint32_t error = 0;
    for(uint32_t i = 0; i < HIST_ENTRIES; ++i)
    {
        if(histogram_ref[i] != histogram_opt[i])
        {
            error++;
        }
    }

    free(histogram_ref);
    free(histogram_opt);

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
