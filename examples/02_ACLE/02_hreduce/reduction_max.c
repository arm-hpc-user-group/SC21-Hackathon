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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define SIZE 32

uint64_t max(uint64_t array[SIZE])
{
    uint64_t max = 0;

    for (int i=0; i<SIZE; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }

    return max;
}

int main()
{
    uint64_t array[SIZE] = { 130,   4,   9,  23,1899, 543,  31, 256,
                               0,  93,   3,   2,  11,   1,   5,  18,
                            1024, 999,  46,   4,  99,  63,   9,  12,
                            1714,  19,  26,  44,  41, 111,  27,  34};

    uint64_t result = max(array);

    printf("Max: %lu\n", result);
    
    return 0;
}
