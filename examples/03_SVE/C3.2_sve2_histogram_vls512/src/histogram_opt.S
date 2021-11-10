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
    .global histogram_vls512
    .type histogram_vls512, %function


histogram_vls512:
    // SVE2 implementation for 256-entries histogram
    // VLS implementation (with HISTSEG) for VL512
    // x0: histogram buffer address, histogram entries are 32-bit. There are 256 entries since records are 8-bit
    // x1: records buffer address, records are 8-bit
    // w2: number of records (multiple of 64) = image size

    mov     w2, w2
    ptrue   p0.b
    dup     z29.b, #191     // histogram overflow guard: 255-4*16

    cbz     w2, .L_return

    // Initialize z10.b-z13.b with all existing histogram entries: 0..255
    index   z10.b, #0, #1                   // z10: 0..63
    movprfx z11, z10
    add     z11.b, z11.b, #64               // z11: 64..127
    movprfx z12, z10
    add     z12.b, z12.b, #128              // z12: 128..191
    movprfx z13, z10
    add     z13.b, z13.b, #192              // z13: 192..255

    add     x6, x1, x2

.L_loop_outer:
    // Initialize z20.b-z23.b counters to 0
    mov     z20.b, #0       // will contain counters for histogram entries 0..63
    mov     z21.b, #0
    mov     z22.b, #0
    mov     z23.b, #0

.L_loop_inner:
    ld1rqb  z0.b, p0/Z, [x1, #0]
    ld1rqb  z1.b, p0/Z, [x1, #16]
    ld1rqb  z2.b, p0/Z, [x1, #32]
    ld1rqb  z3.b, p0/Z, [x1, #48]

    histseg z14.b, z10.b, z0.b
    histseg z15.b, z11.b, z0.b
    histseg z16.b, z12.b, z0.b
    histseg z17.b, z13.b, z0.b

    histseg z18.b, z10.b, z1.b
    histseg z19.b, z11.b, z1.b
    histseg z4.b,  z12.b, z1.b
    histseg z5.b,  z13.b, z1.b

    add     x1, x1, #64
    histseg z6.b,  z10.b, z2.b
    histseg z7.b,  z11.b, z2.b
    histseg z8.b,  z12.b, z2.b
    histseg z9.b,  z13.b, z2.b

    histseg z24.b, z10.b, z3.b
    histseg z25.b, z11.b, z3.b
    histseg z26.b, z12.b, z3.b
    histseg z27.b, z13.b, z3.b

    add     z20.b, z20.b, z14.b
    add     z21.b, z21.b, z15.b
    add     z22.b, z22.b, z16.b
    add     z23.b, z23.b, z17.b

    cmp     x1, x6
    add     z20.b, z20.b, z18.b
    add     z21.b, z21.b, z19.b
    add     z22.b, z22.b, z4.b
    add     z23.b, z23.b, z5.b

    add     z20.b, z20.b, z6.b
    add     z21.b, z21.b, z7.b
    add     z22.b, z22.b, z8.b
    add     z23.b, z23.b, z9.b

    add     z20.b, z20.b, z24.b
    add     z21.b, z21.b, z25.b
    add     z22.b, z22.b, z26.b
    add     z23.b, z23.b, z27.b

    b.hs    .L_loop_inner_end

    cmphs   p1.b, p0/Z, z29.b, z20.b
    cmphs   p2.b, p0/Z, z29.b, z21.b
    cmphs   p3.b, p0/Z, z29.b, z22.b
    cmphs   p4.b, p0/Z, z29.b, z23.b
    and     p1.b, p1/Z, p2.b, p3.b  // p1 = p1 & p2 & p3
    nands   p4.b, p0/Z, p1.b, p4.b  // p4 = ~(p0 & p1 & p4)
    b.none  .L_loop_inner

.L_loop_inner_end:
    ld4w    {z2.s, z3.s, z4.s, z5.s}, p0/Z, [x0, #0, MUL VL]
    ld4w    {z6.s, z7.s, z8.s, z9.s}, p0/Z, [x0, #4, MUL VL]
    uxtb    z14.h, p0/M, z20.h
    lsr     z15.h, z20.h, #8
    uxtb    z16.h, p0/M, z21.h
    lsr     z17.h, z21.h, #8
    uaddwb  z2.s, z2.s, z14.h
    uaddwt  z4.s, z4.s, z14.h
    uaddwb  z3.s, z3.s, z15.h
    uaddwt  z5.s, z5.s, z15.h
    uaddwb  z6.s, z6.s, z16.h
    uaddwt  z8.s, z8.s, z16.h
    uaddwb  z7.s, z7.s, z17.h
    uaddwt  z9.s, z9.s, z17.h
    st4w    {z2.s, z3.s, z4.s, z5.s}, p0, [x0, #0, MUL VL]
    ld4w    {z2.s, z3.s, z4.s, z5.s}, p0/Z, [x0, #8, MUL VL]
    uxtb    z14.h, p0/M, z22.h
    lsr     z15.h, z22.h, #8
    uxtb    z16.h, p0/M, z23.h
    lsr     z17.h, z23.h, #8
    st4w    {z6.s, z7.s, z8.s, z9.s}, p0, [x0, #4, MUL VL]
    ld4w    {z6.s, z7.s, z8.s, z9.s}, p0/Z, [x0, #12, MUL VL]
    uaddwb  z2.s, z2.s, z14.h
    uaddwt  z4.s, z4.s, z14.h
    uaddwb  z3.s, z3.s, z15.h
    uaddwt  z5.s, z5.s, z15.h
    uaddwb  z6.s, z6.s, z16.h
    uaddwt  z8.s, z8.s, z16.h
    uaddwb  z7.s, z7.s, z17.h
    uaddwt  z9.s, z9.s, z17.h
    st4w    {z2.s, z3.s, z4.s, z5.s}, p0, [x0, #8, MUL VL]
    st4w    {z6.s, z7.s, z8.s, z9.s}, p0, [x0, #12, MUL VL]

    cmp     x1, x6
    b.lo    .L_loop_outer

.L_return:
    ret

    .size   histogram_vls512, .-histogram_vls512
