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

#include "matmul.h"
#include <arm_sve.h>

void calc_matmul_opt(uint64_t M, uint64_t K, uint64_t N, float64_t* input_left, float64_t* input_right, float64_t* output)
{
    svbool_t p64_all = svptrue_b64();
    uint64_t vl = svcntd();
    uint64_t offset_in_1, offset_in_2, offset_in_3;
    uint64_t offset_out_1, offset_out_2, offset_out_3;

    float64_t *ptr_in_left, *ptr_in_right, *ptr_out;

    svfloat64_t acc0, acc1, acc2, acc3;
    svfloat64_t in_right_0, in_right_1;
    svfloat64_t in_left_0, in_left_1, in_left_2, in_left_3;

    offset_in_1 = K;
    offset_in_2 = 2 * K;
    offset_in_3 = 3 * K;

    offset_out_1 = N;
    offset_out_2 = 2 * N;
    offset_out_3 = 3 * N;

    for(uint64_t x = 0; x < M; x += 4)
    {
        ptr_out = &output[x*N];

        for(uint64_t y = 0; y < N; y += vl)
        {
            acc0 = svdup_f64(0.0);
            acc1 = svdup_f64(0.0);
            acc2 = svdup_f64(0.0);
            acc3 = svdup_f64(0.0);

            ptr_in_left = &input_left[x*K];
            ptr_in_right = &input_right[y];

            for(uint64_t z = 0; z < K; z += 2)
            {
                in_right_0 = svld1(p64_all, ptr_in_right);
                in_right_1 = svld1(p64_all, &ptr_in_right[offset_out_1]);

                in_left_0 = svld1rq(p64_all, ptr_in_left);
                in_left_1 = svld1rq(p64_all, &ptr_in_left[offset_in_1]);
                in_left_2 = svld1rq(p64_all, &ptr_in_left[offset_in_2]);
                in_left_3 = svld1rq(p64_all, &ptr_in_left[offset_in_3]);

                acc0 = svmla_lane(acc0, in_right_0, in_left_0, 0);
                acc0 = svmla_lane(acc0, in_right_1, in_left_0, 1);

                acc1 = svmla_lane(acc1, in_right_0, in_left_1, 0);
                acc1 = svmla_lane(acc1, in_right_1, in_left_1, 1);

                acc2 = svmla_lane(acc2, in_right_0, in_left_2, 0);
                acc2 = svmla_lane(acc2, in_right_1, in_left_2, 1);

                acc3 = svmla_lane(acc3, in_right_0, in_left_3, 0);
                acc3 = svmla_lane(acc3, in_right_1, in_left_3, 1);

                ptr_in_right += 2 * N;
                ptr_in_left += 2;
            }

            svst1(p64_all, ptr_out, acc0);
            svst1(p64_all, &ptr_out[offset_out_1], acc1);
            svst1(p64_all, &ptr_out[offset_out_2], acc2);
            svst1(p64_all, &ptr_out[offset_out_3], acc3);

            ptr_out += vl;
        }
    }
}
