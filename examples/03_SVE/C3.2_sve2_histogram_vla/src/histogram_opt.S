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
    .global calc_histogram_opt
    .type calc_histogram_opt, %function


calc_histogram_opt:
    // SVE2 implementation for 256-entries histogram
    // VLA implementation
    // x0: histogram buffer address, histogram entries are 32-bit. There are 256 entries since records are 8-bit
    // x1: records buffer address, records are 8-bit
    // w2: number of records = image size

    mov     w2, w2
    mov     x4, #0              // loop counter on num_records

    whilelo p1.s, x4, x2
    b.nfrst .L_return

.L_loop:
    ld1b    z1.s, p1/Z, [x1, x4]
    ld1w    z2.s, p1/Z, [x0, z1.s, UXTW #2]
    histcnt z0.s, p1/Z, z1.s, z1.s
    add     z2.s, p1/M, z2.s, z0.s
    st1w    z2.s, p1, [x0, z1.s, UXTW #2]
    incw    x4
    whilelo p1.s, x4, x2
    b.first .L_loop

.L_return:
    ret

    .size   calc_histogram_opt, .-calc_histogram_opt
