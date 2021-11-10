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
    .global calc_vecmax_first_opt
    .type calc_vecmax_first_opt, %function

    srcPtr      .req x0
    size        .req x1
    maxElemPtr  .req x2
    maxIndexPtr .req x3

calc_vecmax_first_opt:
    uxth        w1, w1
    mov         x7, #0
    dup         z0.h, #-1
    whilelt     p1.h, x7, size
    ld1h        z5.h, p1/z, [srcPtr]
    index       z1.h, #0, #1
    inch        x7
    whilelt     p2.h, x7, size
    b.none      .LoopEnd
    mov         z2.d, z1.d

.LoopStart:
    ld1h        z6.h, p2/z, [srcPtr, x7, LSL #1]
    inch        z2.h
    inch        x7
    cmpgt       p3.h, p2/z, z6.h, z5.h
    smax        z5.h, p2/m, z5.h, z6.h
    sel         z1.h, p3, z2.h, z1.h
    whilelt     p2.h, x7, size
    b.first     .LoopStart

.LoopEnd:
    smaxv       h6, p1, z5.h
    mov         z6.h, h6
    cmpeq       p2.h, p1/z, z5.h, z6.h
    ptrue       p0.h
    sel         z1.h, p2, z1.h, z0.h
    uminv       h7, p0, z1.h
    str         h6, [maxElemPtr]
    str         h7, [maxIndexPtr]

    ret

    .size calc_vecmax_first_opt, .-calc_vecmax_first_opt
