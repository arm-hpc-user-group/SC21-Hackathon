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

const size_t SIZE = 1024*1024;


void vec_add(float out[SIZE], float a[SIZE], float b[SIZE])
{
  for (size_t i=0; i<SIZE; ++i) {
    out[i] = a[i] + b[i];
  }
}

int main(int argc, char * argv[])
{
  float * x = (float*)malloc(SIZE*sizeof(float)); 
  float * a = (float*)malloc(SIZE*sizeof(float)); 
  float * b = (float*)malloc(SIZE*sizeof(float)); 

  for (size_t i=0; i<SIZE; ++i) {
    x[i] = 0;
    a[i] = 1;
    b[i] = 2;
  }

  vec_add(x, a, b);

  for (size_t i=0; i<SIZE; ++i) {
    if (x[i] != 3) {
      printf("ERROR: x[%zd] = %g\n", i, x[i]);
      return 1;
    }
  }
  printf("OK\n");

  return EXIT_SUCCESS;
}

