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
    xPtr        .req x2             // int16_t* x
    hPtr        .req x3             // int16_t* h
    yPtr        .req x4             // int16_t* y

calc_fir_opt:
    add         size, yPtr, size, LSL #1
    whilelt     p4.b, yPtr, size
    add         taps, hPtr, taps, LSL #1
    ptrue       p5.h
    b.none      .L_return

.L_FIR_outer_loop:
    mov         x6, #0
    mov         x7, hPtr
    ld1h        z2.h, p4/z, [xPtr, x6, LSL #1]
    ld1rh       z1.h, p5/z, [x7]
    add         x6, x6, #1
    add         x7, x7, #2
    smullb      z3.s, z2.h, z1.h
    smullt      z4.s, z2.h, z1.h

.L_FIR_inner_loop:
    ld1h        z2.h, p4/z, [xPtr, x6, LSL #1]
    // contiguous load of input data
    ld1rh       z1.h, p5/z, [x7]
    // load with broadcast of one FIR filter coefficient
    add         x6, x6, #1
    add         x7, x7, #2
    smlalb      z3.s, z2.h, z1.h
    smlalt      z4.s, z2.h, z1.h
    cmp         x7, taps
    b.mi        .L_FIR_inner_loop

    sqshrnb     z3.h, z3.s, #16
    sqshrnt     z3.h, z4.s, #16
    st1h        z3.h, p4, [yPtr]
    incb        yPtr
    addvl       xPtr, xPtr, #1
    whilelt     p4.b, yPtr, size
    b.first     .L_FIR_outer_loop

.L_return:
    ret

    .size calc_fir_opt, .-calc_fir_opt
