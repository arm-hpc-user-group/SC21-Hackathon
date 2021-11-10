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
#include <arm_sve.h>

#define SIZE 32

uint32_t max_scalar(uint32_t array[SIZE])
{
    uint32_t max = 0.0;

    uint32_t vl = svcntw();
    svbool_t p32_all = svptrue_b32();

    for (int i=0; i<SIZE; i+=vl) {
        // load array elements from memory
        svuint32_t vec = svld1(p32_all, &array[i]);
        // get max within vector
        uint32_t vecmax = svmaxv(p32_all, vec);
        if (vecmax > max) {
            max = vecmax;
        }
    }

    return max;
}


int main()
{
    uint32_t array[SIZE] = {130,4,9,23,1899,543,31,256,0,93,3,2,11,1,5,18};
    
    uint32_t result = max_scalar(array);

    printf("Max: %u\n", result);
    
    return 0;
}
