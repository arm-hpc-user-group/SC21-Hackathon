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
    .global calc_skipwhitespace_opt
    .type calc_skipwhitespace_opt, %function

    startStr    .req x0
    endStr      .req x1

    returnStr   .req x2

calc_skipwhitespace_opt:
    mov         w3, #0xd090000
    add         w3, w3, #0xa20
    mov         z1.s, w3
    // z1.s = 0x0d090a20, which is ascii for '\r\t\n '
    whilelt     p0.b, startStr, endStr

.L_start:
    ld1b        z0.b, p0/z, [startStr]
    nmatch      p1.b, p0/z, z0.b, z1.b
    b.any       .L_spacesEnd
    incb        startStr
    whilelt     p0.b, startStr, endStr
    b.first     .L_start

    mov         startStr, endStr
    b           .L_return

.L_spacesEnd:
    brkb        p2.b, p0/z, p1.b
    incp        startStr, p2.b

.L_return:
    mov         returnStr, startStr

    ret

    .size calc_skipwhitespace_opt, .-calc_skipwhitespace_opt
