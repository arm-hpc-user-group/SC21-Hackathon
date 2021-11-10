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

void calc_matmul_opt_rearrange_left(uint64_t M, uint64_t K, uint8_t* input_left, uint8_t* input_left_mod);
void calc_matmul_opt_rearrange_right(uint64_t K, uint64_t N, uint8_t* input_right, uint8_t* input_right_mod);
void calc_matmul_opt_dotp(uint64_t M, uint64_t K, uint64_t N, uint8_t* input_left_mod, uint8_t* input_right_mod, uint32_t* output);

void calc_matmul_opt(uint64_t M, uint64_t K, uint64_t N, uint8_t* input_left, uint8_t* input_right, uint32_t* output)
{
    uint8_t input_left_mod[K*M];
    uint8_t input_right_mod[N*K];
    calc_matmul_opt_rearrange_left(M, K, input_left, input_left_mod);
    calc_matmul_opt_rearrange_right(K, N, input_right, input_right_mod);
    calc_matmul_opt_dotp(M, K, N, input_left_mod, input_right_mod, output);
}

// Below is the C implementation of the optimised code for calc_matmul_opt_rearrange_left
/*
void calc_matmul_opt_rearrange_left(uint64_t M, uint64_t K, uint8_t* input_left, uint8_t* input_left_mod)
{
    for(uint64_t x = 0; x < M; x += 8)
    {
        uint8_t* ptr_out = &input_left_mod[x*K];

        for(uint64_t y = 0; y < K; y += 4)
        {
            uint8_t* ptr_in = &input_left[(x*K)+y];

            for(uint64_t z = 0; z < 8; ++z)
            {
                for(uint64_t w = 0; w < 4; ++w)
                {
                    *ptr_out = ptr_in[(z*K)+w];
                    ptr_out++;
                }
            }
        }
    }
}
*/

void calc_matmul_opt_rearrange_left(uint64_t M, uint64_t K, uint8_t* input_left, uint8_t* input_left_mod)
{
    uint64_t nb_st_elems, init_nb_elems;
    svbool_t p_ld;
    uint64_t offset_in_1, offset_in_2, offset_in_3, offset_in_4, offset_in_5, offset_in_6, offset_in_7;

    svuint8_t r0, r1, r2, r3, r4, r5, r6, r7;
    svuint32_t r00, r11, r22, r33, r44, r55, r66, r77;
    svuint32_t t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19;

    uint64_t vl = svcntb();
    svbool_t p8_all = svptrue_b8();

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
        uint8_t* ptr_in = &input_left[x*K];
        uint8_t* ptr_out = &input_left_mod[x*K];

        nb_st_elems = init_nb_elems;

        for(uint64_t y = 0; svptest_first(p8_all, p_ld = svwhilelt_b8(y, K)); y += vl)
        {
            r0 = svld1(p_ld, ptr_in);
            r1 = svld1(p_ld, &ptr_in[offset_in_1]);
            r2 = svld1(p_ld, &ptr_in[offset_in_2]);
            r3 = svld1(p_ld, &ptr_in[offset_in_3]);
            r4 = svld1(p_ld, &ptr_in[offset_in_4]);
            r5 = svld1(p_ld, &ptr_in[offset_in_5]);
            r6 = svld1(p_ld, &ptr_in[offset_in_6]);
            r7 = svld1(p_ld, &ptr_in[offset_in_7]);

            t8 = svzip1(svreinterpret_u32(r0), svreinterpret_u32(r4));
            t9 = svzip1(svreinterpret_u32(r2), svreinterpret_u32(r6));
            t10 = svzip1(svreinterpret_u32(r1), svreinterpret_u32(r5));
            t11 = svzip1(svreinterpret_u32(r3), svreinterpret_u32(r7));
            t12 = svzip2(svreinterpret_u32(r0), svreinterpret_u32(r4));
            t13 = svzip2(svreinterpret_u32(r2), svreinterpret_u32(r6));
            t14 = svzip2(svreinterpret_u32(r1), svreinterpret_u32(r5));
            t15 = svzip2(svreinterpret_u32(r3), svreinterpret_u32(r7));

            t16 = svzip1(t8, t9);
            t17 = svzip1(t10, t11);
            t18 = svzip2(t8, t9);
            t19 = svzip2(t10, t11);
            r00 = svzip1(t16, t17);
            r11 = svzip2(t16, t17);
            r22 = svzip1(t18, t19);
            r33 = svzip2(t18, t19);

            t16 = svzip1(t12, t13);
            t17 = svzip1(t14, t15);
            t18 = svzip2(t12, t13);
            t19 = svzip2(t14, t15);
            r44 = svzip1(t16, t17);
            r55 = svzip2(t16, t17);
            r66 = svzip1(t18, t19);
            r77 = svzip2(t18, t19);

            switch(nb_st_elems)
            {
                default:
                    svst1_vnum(p8_all, ptr_out, 7, svreinterpret_u8(r77));
                    svst1_vnum(p8_all, ptr_out, 6, svreinterpret_u8(r66));
                    svst1_vnum(p8_all, ptr_out, 5, svreinterpret_u8(r55));
                    svst1_vnum(p8_all, ptr_out, 4, svreinterpret_u8(r44));
                case 4:
                    svst1_vnum(p8_all, ptr_out, 3, svreinterpret_u8(r33));
                    svst1_vnum(p8_all, ptr_out, 2, svreinterpret_u8(r22));
                case 2:
                    svst1_vnum(p8_all, ptr_out, 1, svreinterpret_u8(r11));
                    svst1(p8_all, ptr_out, svreinterpret_u8(r00));
            }

            ptr_in += vl;
            ptr_out += 8 * vl;
            nb_st_elems -= 8;
        }
    }
}

// Below is the C implementation of the optimised code for calc_matmul_opt_rearrange_right
/*
void calc_matmul_opt_rearrange_right(uint64_t K, uint64_t N, uint8_t* input_right, uint8_t* input_right_mod)
{
    uint64_t vl_4 = svcntw();   // svcntb >> 2

    for(uint64_t y = 0; y < N; y += vl_4)
    {
        uint8_t* ptr_out = &input_right_mod[y*K];

        for(uint64_t x = 0; x < K; x += 4)
        {
            uint8_t* ptr_in = &input_right[(x*N)+y];

            for(uint64_t z = 0; z < vl_4; ++z)
            {
                for(uint64_t w = 0; w < 4; ++w)
                {
                    *ptr_out = ptr_in[(w*N)+z];
                    ptr_out++;
                }
            }
        }
    }
}
*/

void calc_matmul_opt_rearrange_right(uint64_t K, uint64_t N, uint8_t* input_right, uint8_t* input_right_mod)
{
    uint64_t nb_st_elems;
    svbool_t p_ld;
    svuint8_t r0, r1, r2, r3;
    uint64_t offset_in_1, offset_in_2, offset_in_3;
    uint64_t offset_out_1, offset_out_2, offset_out_3;

    svbool_t p8_all = svptrue_b8();
    uint64_t vl = svcntb();
    uint64_t vl_4 = vl >> 2;

    offset_in_1 = N;
    offset_in_2 = 2 * N;
    offset_in_3 = 3 * N;

    offset_out_1 = K * vl_4;
    offset_out_2 = K * vl_4 * 2;
    offset_out_3 = K * vl_4 * 3;

    nb_st_elems = (4 * N) / vl;

    for(uint64_t y = 0; svptest_first(p8_all, p_ld = svwhilelt_b8(y, N)); y += vl)
    {
        uint8_t* ptr_out = &input_right_mod[y*K];

        for(uint64_t x = 0; x < K; x += 4)
        {
            uint8_t* ptr_in = &input_right[(x*N)+y];

            r0 = svld1(p_ld, ptr_in);
            r1 = svld1(p_ld, &ptr_in[offset_in_1]);
            r2 = svld1(p_ld, &ptr_in[offset_in_2]);
            r3 = svld1(p_ld, &ptr_in[offset_in_3]);

            svuint8_t t4 = svzip1(r0, r2);
            svuint8_t t5 = svzip1(r1, r3);
            svuint8_t t6 = svzip2(r0, r2);
            svuint8_t t7 = svzip2(r1, r3);

            r0 = svzip1(t4, t5);
            r1 = svzip2(t4, t5);
            r2 = svzip1(t6, t7);
            r3 = svzip2(t6, t7);

            switch(nb_st_elems)
            {
                default:
                    svst1(p8_all, &ptr_out[offset_out_3], r3);
                    svst1(p8_all, &ptr_out[offset_out_2], r2);
                case 2:
                    svst1(p8_all, &ptr_out[offset_out_1], r1);
                case 1:
                    svst1(p8_all, ptr_out, r0);
            }

            ptr_out += vl;
        }

        nb_st_elems -= 4;
    }
}

// Below is the C implementation of the optimised code for calc_matmul_opt_dotp
/*
void calc_matmul_opt_dotp(uint64_t M, uint64_t K, uint64_t N, uint8_t* input_left_mod, uint8_t* input_right_mod, uint32_t* output)
{
    uint64_t vl_4 = svcntw();   // svcntb >> 2

    for(uint64_t x = 0; x < M; ++x)
    {
        uint8_t* ptr_in_left = &input_left_mod[(x/8)*8*K];

        for(uint64_t y = 0; y < N; ++y)
        {
            uint8_t* ptr_in_right = &input_right_mod[(y/vl_4)*vl_4*K];
            uint32_t acc = 0;

            for(uint64_t z = 0; z < K; ++z)
            {
                acc += ptr_in_left[((z/4)*4*8)+((x%8)*4)+(z%4)] * ptr_in_right[((z/4)*4*vl_4)+((y%vl_4)*4)+(z%4)];
            }

            output[(x*N)+y] = acc;
        }
    }
}
*/

void calc_matmul_opt_dotp(uint64_t M, uint64_t K, uint64_t N, uint8_t* input_left_mod, uint8_t* input_right_mod, uint32_t* output)
{
    svuint32_t acc0, acc1, acc2, acc3, acc4, acc5, acc6, acc7;
    uint64_t offset_out_1, offset_out_2, offset_out_3, offset_out_4, offset_out_5, offset_out_6, offset_out_7;

    uint8_t* ptr_in_left;
    uint8_t* ptr_in_right;
    uint32_t* ptr_out;

    svbool_t p8_all = svptrue_b8();
    uint64_t vl = svcntb();
    uint64_t vl_4 = vl >> 2;

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
        ptr_in_right = &input_right_mod[0];

        for(uint64_t y = 0; y < N; y += vl_4)
        {
            ptr_in_left = &input_left_mod[x*K];

            acc0 = svdup_u32(0);
            acc1 = svdup_u32(0);
            acc2 = svdup_u32(0);
            acc3 = svdup_u32(0);
            acc4 = svdup_u32(0);
            acc5 = svdup_u32(0);
            acc6 = svdup_u32(0);
            acc7 = svdup_u32(0);

            for(uint64_t z = 0; z < K; z += 4)
            {
                svuint8_t a0123 = svld1rq(p8_all, ptr_in_left);
                svuint8_t a4567 = svld1rq(p8_all, &ptr_in_left[16]);
                svuint8_t b_vec = svld1(p8_all, ptr_in_right);

                acc0 = svdot_lane(acc0, b_vec, a0123, 0);
                acc1 = svdot_lane(acc1, b_vec, a0123, 1);
                acc2 = svdot_lane(acc2, b_vec, a0123, 2);
                acc3 = svdot_lane(acc3, b_vec, a0123, 3);
                acc4 = svdot_lane(acc4, b_vec, a4567, 0);
                acc5 = svdot_lane(acc5, b_vec, a4567, 1);
                acc6 = svdot_lane(acc6, b_vec, a4567, 2);
                acc7 = svdot_lane(acc7, b_vec, a4567, 3);

                ptr_in_left += 32;
                ptr_in_right += vl;
            }

            svst1(p8_all, ptr_out, acc0);
            svst1(p8_all, &ptr_out[offset_out_1], acc1);
            svst1(p8_all, &ptr_out[offset_out_2], acc2);
            svst1(p8_all, &ptr_out[offset_out_3], acc3);
            svst1(p8_all, &ptr_out[offset_out_4], acc4);
            svst1(p8_all, &ptr_out[offset_out_5], acc5);
            svst1(p8_all, &ptr_out[offset_out_6], acc6);
            svst1(p8_all, &ptr_out[offset_out_7], acc7);

            ptr_out += vl_4;
        }
    }
}
