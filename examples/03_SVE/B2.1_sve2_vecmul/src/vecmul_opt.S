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


    .text
    .global calc_vecmul_opt
    .type calc_vecmul_opt, %function

    size        .req x0             // N
    aPtr        .req x1             // cplx_int16_t * a
    bPtr        .req x2             // cplx_int16_t * b
    outPtr      .req x3             // cplx_int16_t * c

    aPtr_1st    .req x4
    bPtr_1st    .req x5
    outPtr_1st  .req x6
    count       .req x7

calc_vecmul_opt:
    ptrue       p2.h
    lsl         size, size, #1
    dup         z31.s, #0

    cnth        count
    whilelt     p4.h, count, size
    b.nfrst     .L_tail_vecmul

    addvl       aPtr_1st, aPtr, #-1
    addvl       bPtr_1st, bPtr, #-1
    addvl       outPtr_1st, outPtr, #-1

.L_unrolled_loop_vecmul:
    ld1h        z0.h, p2/z, [aPtr_1st, count, LSL #1]
    ld1h        z2.h, p4/z, [aPtr, count, LSL #1]
    ld1h        z1.h, p2/z, [bPtr_1st, count, LSL #1]
    ld1h        z3.h, p4/z, [bPtr, count, LSL #1]

    movprfx     z4, z31
    sqrdcmlah   z4.h, z0.h, z1.h, #0
    // c0.re += a0.re * b0.re | c0.im += a0.re * b0.im
    sqrdcmlah   z4.h, z0.h, z1.h, #90
    // c0.re += - a0.im * b0.im | c0.im += a0.im * b0.re

    movprfx     z5, z31
    sqrdcmlah   z5.h, z2.h, z3.h, #0
    sqrdcmlah   z5.h, z2.h, z3.h, #90

    st1h        z4.h, p2, [outPtr_1st, count, LSL #1]
    st1h        z5.h, p4, [outPtr, count, LSL #1]
    inch        count, ALL, MUL #2

    whilelt     p4.h, count, size
    b.first     .L_unrolled_loop_vecmul

.L_tail_vecmul:
    dech        count
    whilelt     p2.h, count, size
    b.nfrst     .L_return_vecmul

    ld1h        z0.h, p2/z, [aPtr, count, LSL #1]
    ld1h        z1.h, p2/z, [bPtr, count, LSL #1]

    movprfx     z4, z31
    sqrdcmlah   z4.h, z0.h, z1.h, #0
    sqrdcmlah   z4.h, z0.h, z1.h, #90
    st1h        z4.h, p2, [outPtr, count, LSL #1]

.L_return_vecmul:
    ret

    .size calc_vecmul_opt, .-calc_vecmul_opt
