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

void calc_matmul_opt_rearrange_left(uint64_t M, uint64_t K, float16_t* input_left, float16_t* input_left_mod);
void calc_matmul_opt_dotp(uint64_t M, uint64_t K, uint64_t N, float16_t* input_left_mod, float16_t* input_right, float16_t* output);

void calc_matmul_opt(uint64_t M, uint64_t K, uint64_t N, float16_t* input_left, float16_t* input_right, float16_t* output)
{
    float16_t input_left_mod[K*M];
    calc_matmul_opt_rearrange_left(M, K, input_left, input_left_mod);
    calc_matmul_opt_dotp(M, K, N, input_left_mod, input_right, output);
}

// Below is the C implementation of the optimised code for calc_matmul_opt_rearrange_left
/*
void calc_matmul_opt_rearrange_left(uint64_t M, uint64_t K, float16_t* input_left, float16_t* input_left_mod)
{
    for(uint64_t x = 0; x < M; x += 8)
    {
        float16_t* ptr_in = &input_left[x*K];
        float16_t* ptr_out = &input_left_mod[x*K];

        for(uint64_t y = 0; y < K; ++y)
        {
            for(uint64_t z = 0; z < 8; ++z)
            {
                *ptr_out = ptr_in[(z*K)+y];
                ptr_out++;
            }
        }
    }
}
*/

void calc_matmul_opt_rearrange_left(uint64_t M, uint64_t K, float16_t* input_left, float16_t* input_left_mod)
{
    uint64_t nb_st_elems, init_nb_elems;
    svbool_t p_ld;
    svfloat16_t r0, r1, r2, r3, r4, r5, r6, r7;
    uint64_t offset_in_1, offset_in_2, offset_in_3, offset_in_4, offset_in_5, offset_in_6, offset_in_7;

    float16_t *ptr_in, *ptr_out;

    uint64_t vl = svcnth();
    svbool_t p16_all = svptrue_b16();

    offset_in_1 = K;
    offset_in_2 = 2 * K;
    offset_in_3 = 3 * K;
    offset_in_4 = 4 * K;
    offset_in_5 = 5 * K;
    offset_in_6 = 6 * K;
    offset_in_7 = 7 * K;

    init_nb_elems = (8 * K) / vl;

    for(uint64_t x = 0; x < M; x += 8)
    {
        ptr_in = &input_left[x*K];
        ptr_out = &input_left_mod[x*K];

        nb_st_elems = init_nb_elems;

        for(uint64_t y = 0; svptest_first(p16_all, p_ld = svwhilelt_b16(y, K)); y += vl)
        {
            r0 = svld1(p_ld, ptr_in);
            r1 = svld1(p_ld, &ptr_in[offset_in_1]);
            r2 = svld1(p_ld, &ptr_in[offset_in_2]);
            r3 = svld1(p_ld, &ptr_in[offset_in_3]);
            r4 = svld1(p_ld, &ptr_in[offset_in_4]);
            r5 = svld1(p_ld, &ptr_in[offset_in_5]);
            r6 = svld1(p_ld, &ptr_in[offset_in_6]);
            r7 = svld1(p_ld, &ptr_in[offset_in_7]);

            svfloat16_t t8 = svzip1(r0, r4);
            svfloat16_t t9 = svzip1(r2, r6);
            svfloat16_t t10 = svzip1(r1, r5);
            svfloat16_t t11 = svzip1(r3, r7);

            svfloat16_t t12 = svzip2(r0, r4);
            svfloat16_t t13 = svzip2(r2, r6);
            svfloat16_t t14 = svzip2(r1, r5);
            svfloat16_t t15 = svzip2(r3, r7);

            svfloat16_t t16 = svzip1(t8, t9);
            svfloat16_t t17 = svzip1(t10, t11);
            svfloat16_t t18 = svzip2(t8, t9);
            svfloat16_t t19 = svzip2(t10, t11);

            r0 = svzip1(t16, t17);
            r1 = svzip2(t16, t17);
            r2 = svzip1(t18, t19);
            r3 = svzip2(t18, t19);

            t16 = svzip1(t12, t13);
            t17 = svzip1(t14, t15);
            t18 = svzip2(t12, t13);
            t19 = svzip2(t14, t15);

            r4 = svzip1(t16, t17);
            r5 = svzip2(t16, t17);
            r6 = svzip1(t18, t19);
            r7 = svzip2(t18, t19);

            switch(nb_st_elems)
            {
                default:
                    svst1_vnum(p16_all, ptr_out, 7, r7);
                    svst1_vnum(p16_all, ptr_out, 6, r6);
                    svst1_vnum(p16_all, ptr_out, 5, r5);
                    svst1_vnum(p16_all, ptr_out, 4, r4);
                case 4:
                    svst1_vnum(p16_all, ptr_out, 3, r3);
                    svst1_vnum(p16_all, ptr_out, 2, r2);
                case 2:
                    svst1_vnum(p16_all, ptr_out, 1, r1);
                    svst1(p16_all, ptr_out, r0);
            }

            ptr_in += vl;
            ptr_out += 8 * vl;
            nb_st_elems -= 8;
        }
    }
}

// Below is the C implementation of the optimised code for calc_matmul_opt_dotp
/*
void calc_matmul_opt_dotp(uint64_t M, uint64_t K, uint64_t N, float16_t* input_left_mod, float16_t* input_right, float16_t* output)
{
    for(uint64_t x = 0; x < M; ++x)
    {
        float16_t* ptr_in_left = &input_left_mod[(((x/8)*8)*K)+(x%8)];

        for(uint64_t y = 0; y < N; ++y)
        {
            float16_t acc = 0.0;

            for(uint64_t z = 0; z < K; ++z)
            {
                acc = (float16_t)((float32_t)acc + (float32_t)ptr_in_left[z*8] * (float32_t)input_right[(z*N)+y]);
            }
            output[(x*N)+y] = acc;
        }
    }
}
*/

void calc_matmul_opt_dotp(uint64_t M, uint64_t K, uint64_t N, float16_t* input_left_mod, float16_t* input_right, float16_t* output)
{
    svfloat16_t in_right, in_left;
    svfloat16_t acc0, acc1, acc2, acc3, acc4, acc5, acc6, acc7;
    svbool_t p_ld_st;
    uint64_t offset_out_1, offset_out_2, offset_out_3, offset_out_4, offset_out_5, offset_out_6, offset_out_7;

    svbool_t p16_all = svptrue_b16();
    uint64_t vl = svcnth();

    float16_t *ptr_in_left, *ptr_in_right, *ptr_out;

    offset_out_1 = N;
    offset_out_2 = 2 * N;
    offset_out_3 = 3 * N;
    offset_out_4 = 4 * N;
    offset_out_5 = 5 * N;
    offset_out_6 = 6 * N;
    offset_out_7 = 7 * N;

    for(uint64_t x = 0; x < M; x += 8)
    {
        ptr_out = &output[x*N];

        for(uint64_t y = 0; svptest_first(p16_all, p_ld_st = svwhilelt_b16(y, N)); y += vl)
        {
            ptr_in_left = &input_left_mod[x*K];
            ptr_in_right = &input_right[y];

            acc0 = svdup_f16(0.0);
            acc1 = svdup_f16(0.0);
            acc2 = svdup_f16(0.0);
            acc3 = svdup_f16(0.0);
            acc4 = svdup_f16(0.0);
            acc5 = svdup_f16(0.0);
            acc6 = svdup_f16(0.0);
            acc7 = svdup_f16(0.0);

            for(uint64_t z = 0; z < K; ++z)
            {
                in_right = svld1(p_ld_st, &ptr_in_right[z*N]);
                in_left = svld1rq(p16_all, &ptr_in_left[8*z]);

                acc0 = svmla_lane(acc0, in_right, in_left, 0);
                acc1 = svmla_lane(acc1, in_right, in_left, 1);
                acc2 = svmla_lane(acc2, in_right, in_left, 2);
                acc3 = svmla_lane(acc3, in_right, in_left, 3);
                acc4 = svmla_lane(acc4, in_right, in_left, 4);
                acc5 = svmla_lane(acc5, in_right, in_left, 5);
                acc6 = svmla_lane(acc6, in_right, in_left, 6);
                acc7 = svmla_lane(acc7, in_right, in_left, 7);
            }

            svst1(p_ld_st, ptr_out, acc0);
            svst1(p_ld_st, &ptr_out[offset_out_1], acc1);
            svst1(p_ld_st, &ptr_out[offset_out_2], acc2);
            svst1(p_ld_st, &ptr_out[offset_out_3], acc3);
            svst1(p_ld_st, &ptr_out[offset_out_4], acc4);
            svst1(p_ld_st, &ptr_out[offset_out_5], acc5);
            svst1(p_ld_st, &ptr_out[offset_out_6], acc6);
            svst1(p_ld_st, &ptr_out[offset_out_7], acc7);

            ptr_out += vl;
        }
    }
}
