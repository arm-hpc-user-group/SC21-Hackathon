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
#include <string.h>
#include "strcmp.h"

#define TEST_STRING_1 "test string"
#define TEST_STRING_2 "test string new"

int main()
{
    const char* str_in_1 = TEST_STRING_1;
    const char* str_in_2 = TEST_STRING_2;

    int diff_ref = calc_strcmp_ref(str_in_1, str_in_2);
    int diff_opt = calc_strcmp_opt(str_in_1, str_in_2);

    if(diff_ref == diff_opt)
    {
        printf("TEST PASSED\n");
    }
    else
    {
        printf("TEST FAILED\n");
        printf("INPUT STRING 1: %s\n", str_in_1);
        printf("INPUT STRING 2: %s\n", str_in_2);
        printf("REFERENCE FIRST DIFFERENT CHAR OFFSET = %d\n", diff_ref);
        printf("OPTIMIZED FIRST DIFFERENT CHAR OFFSET = %d\n", diff_opt);
    }

    return 0;
}
