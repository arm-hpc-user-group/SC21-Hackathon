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
    .global calc_vecdot_opt
    .type calc_vecdot_opt, %function

    size        .req x0             // N
    aPtr        .req x1             // complex float32_t * a
    bPtr        .req x2             // complex float32_t * b
    outPtr      .req x3             // complex float32_t * c

calc_vecdot_opt:
    dup     z4.d, #0
    dup     z5.d, #0
    ptrue   p2.s

    add     size, aPtr, size, lsl #3
    incb    aPtr
    whilelt p4.b, aPtr, size
    b.nfrst .L_tail_vecdot

.L_unrolled_loop_vecdot:
    ld1w    z0.s, p2/z, [aPtr, #-1, mul vl]
    ld1w    z1.s, p4/z, [aPtr]
    ld1w    z2.s, p2/z, [bPtr]
    ld1w    z3.s, p4/z, [bPtr, #1, mul vl]

    fcmla   z4.s, p2/m, z0.s, z2.s, #0
    fcmla   z4.s, p2/m, z0.s, z2.s, #90
    fcmla   z5.s, p2/m, z1.s, z3.s, #0
    fcmla   z5.s, p2/m, z1.s, z3.s, #90

    incb    aPtr, all, mul #2
    incb    bPtr, all, mul #2
    whilelt p4.b, aPtr, size
    b.first .L_unrolled_loop_vecdot

.L_tail_vecdot:
    decb    aPtr
    whilelt p4.b, aPtr, size
    b.nfrst .L_return_vecdot

    ld1w    z0.s, p4/z, [aPtr]
    ld1w    z1.s, p4/z, [bPtr]

    fcmla   z4.s, p4/m, z0.s, z1.s, #0
    fcmla   z4.s, p4/m, z0.s, z1.s, #90

.L_return_vecdot:
    uzp1    z6.s, z4.s, z5.s
    uzp2    z7.s, z4.s, z5.s
    faddv   s6, p2, z6.s
    faddv   s7, p2, z7.s
    stp     s6, s7, [outPtr]

    ret

    .size calc_vecdot_opt, .-calc_vecdot_opt
