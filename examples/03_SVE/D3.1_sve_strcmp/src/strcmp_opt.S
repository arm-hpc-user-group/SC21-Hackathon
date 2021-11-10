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
    .global calc_strcmp_opt
    .type calc_strcmp_opt, %function

    str1        .req x0
    str2        .req x1

    returnDiff  .req x2

calc_strcmp_opt:
    ptrue       p5.b
    setffr
    mov         x5, #0

.L_loop:
    ldff1b      z0.b, p5/z, [str1, x5]
    ldff1b      z1.b, p5/z, [str2, x5]
    rdffrs      p7.b, p5/z
    b.nlast     .L_fault

    incb        x5
    cmpeq       p0.b, p5/z, z0.b, #0
    cmpne       p1.b, p5/z, z0.b, z1.b

.L_test:
    orrs        p4.b, p5/z, p0.b, p1.b
    b.none      .L_loop

.L_return:
    brkb        p4.b, p5/z, p4.b
    lasta       w0, p4, z0.b
    lasta       w1, p4, z1.b
    sub         w0, w0, w1

    mov         returnDiff, str1

    ret

.L_fault:
    incp        x5, p7.b
    setffr
    cmpeq       p0.b, p7/z, z0.b, #0
    cmpne       p1.b, p7/z, z0.b, z1.b
    b           .L_test

    .size calc_strcmp_opt, .-calc_strcmp_opt
