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

#include <inttypes.h>
#include "vecmax.h"

void calc_vecmax_first_ref(int16_t* restrict src, int length, int16_t* restrict ptr_max_elem, uint16_t* restrict ptr_max_index)
{
    int16_t max_val = src[0];
    uint16_t max_index = 0;

    for(uint16_t i = 1; i < length; ++i)
    {
        if(src[i] > max_val)
        {
            max_val = src[i];
            max_index = i;
        }
    }

    *ptr_max_elem = max_val;
    *ptr_max_index = max_index;
}
