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
#include "histogram.h"

void calc_histogram_opt(uint32_t* histogram, uint8_t* records, unsigned int num_records)
{
    unsigned int i;
    unsigned int num_records_div;

    num_records_div = num_records - (num_records % 64);

    histogram_vls512(histogram, records, num_records_div);

    for(i = num_records_div; i < num_records; i++)
    {
        histogram[records[i]] += 1;
    }
}
