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

#include <arm_sve.h>
#include "integral_image.h"

void calc_integral_image_opt(int64_t height, int64_t width, float input[height-1][width], float output[height][width])
{
    svbool_t p_gov, p_gov_update, p_single, p_x_0;
    float32_t *in_row, *out_row;
    svfloat32_t acc, carry_sum, acc_row_up, vec_zero;

    vec_zero = svdup_f32(0.0);
    p_x_0 = svpfalse();
    uint64_t vl = svcntw();

    for(int64_t x = 1; x < height; ++x)
    {
        carry_sum = vec_zero;
        out_row = output[x];
        in_row = input[x-1];

        for(int64_t y = 0; y < width; y += vl)
        {
            p_gov = svwhilelt_b32(y, width);
            p_gov_update = svmov_z(p_x_0, p_gov);
            p_single = svpfalse();
            acc = svld1(p_gov, &in_row[y]);
            p_single = svpnext_b32(p_gov, p_single);

            do
            {
                acc = svadd_m(p_single, acc, carry_sum);
                carry_sum = svclastb(p_single, vec_zero, acc);
                p_single = svpnext_b32(p_gov, p_single);
            }
            while(svptest_any(p_gov, p_single));

            acc_row_up = svld1(p_gov_update, &output[x-1][y]);
            acc = svadd_m(p_gov_update, acc, acc_row_up);

            svst1(p_gov, &out_row[y], acc);
        }

        p_x_0 = svptrue_b32();
    }
}
