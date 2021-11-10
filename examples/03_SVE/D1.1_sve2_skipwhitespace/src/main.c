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
#include "skipwhitespace.h"

#define TEST_STRING "         test string"
#define STR_SIZE strlen(TEST_STRING)

int main()
{
    const char* str_in = TEST_STRING;

    const char* str_out_ref = calc_skipwhitespace_ref(str_in, str_in + STR_SIZE);
    const char* str_out_opt = calc_skipwhitespace_opt(str_in, str_in + STR_SIZE);

    if(*str_out_ref == *str_out_opt)
    {
        printf("TEST PASSED\n");
    }
    else
    {
        printf("TEST FAILED\n");
        printf("INPUT STRING: %s\n", str_in);
        printf("REFERENCE STRING POINTER: %s\n", str_out_ref);
        printf("OPTIMIZED STRING POINTER: %s\n", str_out_opt);
    }

    return 0;
}
