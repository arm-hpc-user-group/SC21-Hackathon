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

#include "sobel.h"
#include <arm_sve.h>

void calc_sobel_opt_hor(float* input, float* out_hor, const float* kx, int64_t height, int64_t width);
void calc_sobel_opt_vert(float* out_hor, float* output, const float* ky, int64_t height, int64_t width);

void calc_sobel_opt(float* input, float* output, const float* kx, const float* ky, int64_t height, int64_t width)
{
    float out_hor[height*(width-2)];

    calc_sobel_opt_hor(input, out_hor, kx, height, width);
    calc_sobel_opt_vert(out_hor, output, ky, height, width);
}

void calc_sobel_opt_hor(float* input, float* out_hor, const float* kx, int64_t height, int64_t width)
{
    svbool_t p_row;
    svfloat32_t in1, in2, in3, res;
    uint64_t vl = svcntw();

    for(int64_t j = 0; j < height; ++j)
    {
        float32_t* in_ptr = &input[j*width];
        float32_t* out_ptr = &out_hor[j*(width-2)];

        for(int64_t i = 0; i < (width - 2); i += vl)
        {
            p_row = svwhilelt_b32(i, width - 2);
            in1 = svld1(p_row, &in_ptr[i]);
            in2 = svld1(p_row, &in_ptr[i+1]);
            in3 = svld1(p_row, &in_ptr[i+2]);

            res = svmul_z(p_row, in1, kx[0]);
            res = svmla_m(p_row, res, in2, kx[1]);
            res = svmla_m(p_row, res, in3, kx[2]);

            svst1(p_row, &out_ptr[i], res);
        }
    }
}

void calc_sobel_opt_vert(float* out_hor, float* output, const float* ky, int64_t height, int64_t width)
{
    svbool_t p_row;
    svfloat32_t in1, in2, in3, res;
    uint64_t vl = svcntw();

    for(int64_t j = 0; j < (height - 2); ++j)
    {
        float32_t* in_ptr = &out_hor[j*(width-2)];
        float32_t* out_ptr = &output[j*(width-2)];

        for(int64_t i = 0; i < (width - 2); i += vl)
        {
            p_row = svwhilelt_b32(i, width - 2);
            in1 = svld1(p_row, &in_ptr[i]);
            in2 = svld1(p_row, &in_ptr[i+(width-2)]);
            in3 = svld1(p_row, &in_ptr[i+(2*(width-2))]);

            res = svmul_z(p_row, in1, ky[0]);
            res = svmla_m(p_row, res, in2, ky[1]);
            res = svmla_m(p_row, res, in3, ky[2]);

            svst1(p_row, &out_ptr[i], res);
        }
    }
}
