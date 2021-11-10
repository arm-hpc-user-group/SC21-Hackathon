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
#include "integral_image.h"

#define HEIGHT 128
#define WIDTH 128

#define ERROR_TOLERANCE 0.05

int main()
{
    float input[HEIGHT][WIDTH];

    float output_ref[HEIGHT+1][WIDTH+1] = {{0}};
    float output_opt[HEIGHT][WIDTH] = {{0}};

    srand((unsigned int)time(0));

    for(int64_t x = 0; x < HEIGHT; ++x)
    {
        for(int64_t y = 0; y < WIDTH; ++y)
        {
            input[x][y] = (float)(rand() % 1000000) / 100.f;
        }
    }

    calc_integral_image_ref(HEIGHT + 1, WIDTH + 1, input, output_ref);
    calc_integral_image_opt(HEIGHT, WIDTH, input, output_opt);

    printf("ERROR TOLERANCE = %f%%\n", (float)ERROR_TOLERANCE);
    uint32_t error = 0;
    for(int64_t i = 0; i < HEIGHT; ++i)
    {
        for(int64_t j = 0; j < WIDTH; ++j)
        {
            if((fabsf((output_ref[i+1][j+1] - output_opt[i][j]) / output_ref[i+1][j+1]) * 100.0) > ERROR_TOLERANCE)
            {
                error++;
            }
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
