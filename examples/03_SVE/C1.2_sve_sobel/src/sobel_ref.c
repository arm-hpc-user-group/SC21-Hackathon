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

void calc_sobel_ref_hor(float* input, float* out_hor, const float* kx, int64_t height, int64_t width);
void calc_sobel_ref_vert(float* out_hor, float* output, const float* ky, int64_t height, int64_t width);

void calc_sobel_ref(float* input, float* output, const float* kx, const float* ky, int64_t height, int64_t width)
{
    float out_hor[height*(width-2)];

    calc_sobel_ref_hor(input, out_hor, kx, height, width);
    calc_sobel_ref_vert(out_hor, output, ky, height, width);
}

void calc_sobel_ref_hor(float* input, float* out_hor, const float* kx, int64_t height, int64_t width)
{
    for(int64_t j = 0; j < height; ++j)
    {
        for(int64_t i = 0; i < (width - 2); ++i)
        {
            float res = 0.0;
            for(int64_t k = 0; k < 3; ++k)
            {
                res += kx[k] * input[(j*width)+i+k];
            }
            out_hor[(j*(width-2))+i] = res;
        }
    }
}

void calc_sobel_ref_vert(float* out_hor, float* output, const float* ky, int64_t height, int64_t width)
{
    for(int64_t j = 0; j < (height - 2); ++j)
    {
        for(int64_t i = 0; i < (width - 2); ++i)
        {
            float res = 0.0;
            for(int64_t k = 0; k < 3; ++k)
            {
                res += ky[k] * out_hor[((j+k)*(width-2))+i];
            }
            output[(j*(width-2))+i] = res;
        }
    }
}
