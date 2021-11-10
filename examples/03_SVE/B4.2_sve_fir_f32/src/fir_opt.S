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
    .global calc_fir_opt
    .type calc_fir_opt, %function

    size        .req x0             // int32_t n
    taps        .req x1             // int32_t t
    xPtr        .req x2             // float32_t* x
    hPtr        .req x3             // float32_t* h
    yPtr        .req x4             // float32_t* y

calc_fir_opt:
    add         size, yPtr, size, LSL #2
    whilelt     p4.b, yPtr, size
    add         taps, hPtr, taps, LSL #2
    ptrue       p5.s
    b.none      .L_return

.L_FIR_outer_loop:
    mov         x6, #0
    mov         x7, hPtr
    ld1w        z2.s, p4/z, [xPtr, x6, LSL #2]
    ld1rw       z1.s, p5/z, [x7]
    add         x6, x6, #1
    add         x7, x7, #4
    fmul        z3.s, z2.s, z1.s

.L_FIR_inner_loop:
    ld1w        z2.s, p4/z, [xPtr, x6, LSL #2]
    // contiguous load of input data
    ld1rw       z1.s, p5/z, [x7]
    // load with broadcast of one FIR filter coefficient
    add         x6, x6, #1
    add         x7, x7, #4
    fmla        z3.s, p5/m, z2.s, z1.s
    cmp         x7, taps
    b.mi        .L_FIR_inner_loop

    st1w        z3.s, p4, [yPtr]
    incb        yPtr
    addvl       xPtr, xPtr, #1
    whilelt     p4.b, yPtr, size
    b.first     .L_FIR_outer_loop

.L_return:
    ret

    .size calc_fir_opt, .-calc_fir_opt
