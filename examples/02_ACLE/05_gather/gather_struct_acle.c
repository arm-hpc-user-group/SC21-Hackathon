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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arm_sve.h>

const uint32_t SIZE = 1024;

typedef struct {
    int32_t x;
    int32_t y;
} Particle_t;

void move(Particle_t p[SIZE], int32_t x, int32_t y)
{
    uint32_t vl = svcntw();

    svbool_t p32_all = svptrue_b32();

    for (int i=0; i<SIZE; i+=vl) {
        svint32x2_t vp = svld2(p32_all, (int32_t*)&p[i]);

        vp.v0 = svadd_m(p32_all, vp.v0, x);
        vp.v1 = svadd_m(p32_all, vp.v1, y);

        svst2(p32_all, (int32_t*)&p[i], vp);
    }
}

int main()
{
    Particle_t particles[SIZE] = {{1,2},{2,3},{-1,1},{2,0},{-2,1}}; //initializes first 5 numbers

    for (int i=0; i<1; i++) {
	move(particles, 1, -1);
    }

    for (int i=0; i<5; i++) {
        printf("[%i] = (%i,%i)\n", i, particles[i].x, particles[i].y);
    }

    return 0;
}
