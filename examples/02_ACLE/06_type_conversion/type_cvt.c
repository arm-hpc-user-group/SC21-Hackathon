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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <arm_sve.h>


void print_predicate(char const * var, svbool_t pg)
{
  printf("%s = ", var);

  char const * b = (char*)&pg;
  int const bytes = svcntb() / 8;
  for (int i=bytes-1; i>=0; --i) {
    for (int j=7; j>=0; --j) {
      putchar(((b[i] >> j) & 1) + 48);
    }
    if (i) putchar('|');
  }
  putchar('\n');
}

void print_svfloat64(char const * var, svfloat64_t vd)
{
  printf("%s = ", var);

  double const * p = &vd;
  uint32_t const n = svcntd();
  for (int i=n-1; i>=0; --i) {
    printf("%g", p[i]);
    if (i) putchar('|');
  }
  putchar('\n');
}

void print_svfloat32(char const * var, svfloat32_t vs)
{
  printf("%s = ", var);

  float const * p = &vs;
  uint32_t const n = svcntw();
  for (int i=n-1; i>=0; --i) {
    printf("%g", p[i]);
    if (i) putchar('|');
  }
  putchar('\n');
}



int main(int argc, char * argv[])
{
  // Get SVE vector width in bytes
  uint32_t const vec_bytes = svcntb();

  svbool_t pg;
  svfloat64_t vf64;
  svfloat32_t vf32;
  svfloat16_t vf16;

  // Maximum SVE vector width is 2048 bits
  float64_t sf64[32];
  float32_t sf32[64];
  float16_t sf16[128];

  // Initialize arrays with series 0..1..N
  for (int i=0; i<32; ++i) {
    sf64[i] = i;
  }
  for (int i=0; i<64; ++i) {
    sf32[i] = i;
  }
  for (int i=0; i<128; ++i) {
    sf16[i] = i;
  }

  printf("Vectors are %d bytes wide\n", vec_bytes);

  // ------------------------------------------------------------------------
  // Show predicate patterns
  printf("\n");

  print_predicate("svptrue_b64", svptrue_b64());
  print_predicate("svptrue_b32", svptrue_b32());
  print_predicate("svptrue_b16", svptrue_b16());
  print_predicate(" svptrue_b8", svptrue_b8());

  // ------------------------------------------------------------------------
  // float64 => float32, predicate = svptrue_b8()
  printf("\n");

  pg = svptrue_b8();
  print_predicate("pg", pg);

  vf64 = svld1(pg, sf64);
  print_svfloat64("vf64", vf64);

  vf32 = svcvt_f32_z(pg, vf64);
  print_svfloat32("svcvt_f32_z(pg, vf64)", vf32);

  // ------------------------------------------------------------------------
  // float64 => float32, predicate = svptrue_b32()
  printf("\n");

  pg = svptrue_b32();
  print_predicate("pg", pg);

  vf64 = svld1(pg, sf64);
  print_svfloat64("vf64", vf64);

  vf32 = svcvt_f32_z(pg, vf64);
  print_svfloat32("svcvt_f32_z(pg, vf64)", vf32);

  // ------------------------------------------------------------------------
  // float64 => float32 with extreme values, predicate = svptrue_b8()
  printf("\n");

  pg = svptrue_b8();
  print_predicate("pg", pg);

  sf64[0] = FLT_MAX;
  sf64[1] = DBL_MAX;
  vf64 = svld1(pg, sf64);
  print_svfloat64("vf64", vf64);

  vf32 = svcvt_f32_z(pg, vf64);
  print_svfloat32("svcvt_f32_z(pg, vf64)", vf32);

  // ------------------------------------------------------------------------
  // float32 => float64, predicate = svptrue_b8()
  printf("\n");

  pg = svptrue_b8();
  print_predicate("pg", pg);

  vf32 = svld1(pg, sf32);
  print_svfloat32("vf32", vf32);

  vf64 = svcvt_f64_z(pg, vf32);
  print_svfloat64("svcvt_f64_z(pg, vf32)", vf64);

  // ------------------------------------------------------------------------
  // float32 => float64, predicate = svptrue_b32()
  printf("\n");

  pg = svptrue_b32();
  print_predicate("pg", pg);

  vf32 = svld1(pg, sf32);
  print_svfloat32("vf32", vf32);

  vf64 = svcvt_f64_z(pg, vf32);
  print_svfloat64("svcvt_f64_z(pg, vf32)", vf64);

  // ------------------------------------------------------------------------
  // float32 => float64, predicate = svptrue_b64()
  printf("\n");

  pg = svptrue_b64();
  print_predicate("pg", pg);

  vf32 = svld1(pg, sf32);
  print_svfloat32("vf32", vf32);

  vf64 = svcvt_f64_z(pg, vf32);
  print_svfloat64("svcvt_f64_z(pg, vf32)", vf64);

  // ------------------------------------------------------------------------
  // float32 => float64, predicate = svptrue_pat_b64(SV_VL3)
  printf("\n");

  pg = svptrue_pat_b64(SV_VL3);
  print_predicate("pg", pg);

  vf32 = svld1(pg, sf32);
  print_svfloat32("vf32", vf32);

  vf64 = svcvt_f64_z(pg, vf32);
  print_svfloat64("svcvt_f64_z(pg, vf32)", vf64);

  return EXIT_SUCCESS;
}

