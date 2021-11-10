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

void calc_integral_image_opt_vert_vla(int64_t height, int64_t width, float input[height][width]);
void calc_integral_image_opt_hor_vls128(int64_t height, int64_t width, float input[height][width], float output[height][width]);

void calc_integral_image_opt(int64_t height, int64_t width, float input[height][width], float output[height][width])
{
    calc_integral_image_opt_vert_vla(height, width, input);
    calc_integral_image_opt_hor_vls128(height, width, input, output);
}

void calc_integral_image_opt_vert_vla(int64_t height, int64_t width, float input[height][width])
{
    svbool_t p_ld_st;
    svfloat32_t acc, acc_up;

    uint64_t vl = svcntw();

    for(int64_t x = 1; x < height; ++x)
    {
        float32_t* row = input[x];
        float32_t* row_up = input[x-1];

        for(int64_t y = 0; y < width; y += vl)
        {
            p_ld_st = svwhilelt_b32(y, width);
            acc = svld1(p_ld_st, &row[y]);
            acc_up = svld1(p_ld_st, &row_up[y]);
            acc = svadd_m(p_ld_st, acc, acc_up);
            svst1(p_ld_st, &row[y], acc);
        }
    }
}

void calc_integral_image_opt_hor_vls128(int64_t height, int64_t width, float input[height][width], float output[height][width])
{
    uint64_t vl = svcntw();
    svint32_t vec_offsets;
    svfloat32_t in0, in1, in2, in3;
    svfloat32_t tmp4, tmp5, tmp6, tmp7;
    svfloat32_t acc_sums;
    svbool_t p_ld_st, p_ld_st_34, p_gather;

    svbool_t p_true = svptrue_b32();
    svbool_t p_row34 = p_true;
    svbool_t p_vert = p_true;

    int32_t ptr_offset = (int32_t)(width << 2);

    svint32_t vec_offsets_init = svmul_z(p_true, svindex_s32(0,1), ptr_offset);
    vec_offsets_init = svsub_x(p_true, vec_offsets_init, 4);

    int32_t value_offset = 16;

    for(int64_t x = 0; x < height; x += 4)
    {
        if(height < (x + 3))
        {
            p_row34 = svpfalse();
            p_vert = svptrue_pat_b32(SV_VL2);
        }

        float32_t* ptrOut_0 = output[x];
        float32_t* ptrOut_1 = output[x+1];
        float32_t* ptrOut_2 = output[x+2];
        float32_t* ptrOut_3 = output[x+3];

        vec_offsets = vec_offsets_init;

        for(int64_t y = 0; y < width; y += vl)
        {
            p_ld_st = svwhilelt_b32(y, width);
            p_ld_st_34 = svmov_z(p_row34, p_ld_st);

            in0 = svld1(p_ld_st, &input[x][y]);
            in1 = svld1(p_ld_st, &input[x+1][y]);
            in2 = svld1(p_ld_st_34, &input[x+2][y]);
            in3 = svld1(p_ld_st_34, &input[x+3][y]);

            tmp4 = svtrn1(in0, in1);
            tmp5 = svtrn2(in0, in1);
            tmp6 = svtrn1(in2, in3);
            tmp7 = svtrn2(in2, in3);

            in0 = svreinterpret_f32(svtrn1(svreinterpret_f64(tmp4), svreinterpret_f64(tmp6)));
            in1 = svreinterpret_f32(svtrn1(svreinterpret_f64(tmp5), svreinterpret_f64(tmp7)));
            in2 = svreinterpret_f32(svtrn2(svreinterpret_f64(tmp4), svreinterpret_f64(tmp6)));
            in3 = svreinterpret_f32(svtrn2(svreinterpret_f64(tmp5), svreinterpret_f64(tmp7)));

            p_gather = svwhilelt_b32((int64_t)0, y);
            p_gather = svmov_z(p_vert, p_gather);
            acc_sums = svld1_gather_offset(p_gather, ptrOut_0, vec_offsets);
            vec_offsets = svadd_x(p_true, vec_offsets, value_offset);

            in0 = svadd_x(p_true, in0, acc_sums);
            in1 = svadd_x(p_true, in1, in0);
            in2 = svadd_x(p_true, in2, in1);
            in3 = svadd_x(p_true, in3, in2);

            tmp4 = svtrn1(in0, in1);
            tmp5 = svtrn2(in0, in1);
            tmp6 = svtrn1(in2, in3);
            tmp7 = svtrn2(in2, in3);

            in0 = svreinterpret_f32(svtrn1(svreinterpret_f64(tmp4), svreinterpret_f64(tmp6)));
            in1 = svreinterpret_f32(svtrn1(svreinterpret_f64(tmp5), svreinterpret_f64(tmp7)));
            in2 = svreinterpret_f32(svtrn2(svreinterpret_f64(tmp4), svreinterpret_f64(tmp6)));
            in3 = svreinterpret_f32(svtrn2(svreinterpret_f64(tmp5), svreinterpret_f64(tmp7)));

            svst1(p_ld_st, &ptrOut_0[y], in0);
            svst1(p_ld_st, &ptrOut_1[y], in1);
            svst1(p_ld_st_34, &ptrOut_2[y], in2);
            svst1(p_ld_st_34, &ptrOut_3[y], in3);
        }
    }
}
