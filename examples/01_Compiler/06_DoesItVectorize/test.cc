// BSD 3-Clause License
// 
// Copyright (c) 2021, esiegmann
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <iostream>
#include <unistd.h>
#include <random>
#include <vector>
#include <cstdio>
#include <cmath>
#include <omp.h>

#define ALIGNAS alignas(256)

static size_t const GATHERVL = 16;
static size_t const N = 84*GATHERVL; // must be multiple of GATHERVL and 3*N*8 fit into L1
static size_t const NREPEAT = 10000;

// Thread count of 0 means single thread without OpenMP pragmas
static std::vector<size_t> const THREAD_COUNTS = {
  0, 1, 4, 8, 12, 24, 36, 48
};

// These are copied into a thread's stack before use
static std::vector<size_t> globalmap(N);
static std::vector<size_t> shortmap(N);


void Xsimple(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  for (size_t i=0; i<n; i++) {
    y[i] = 2.0*x[i] + 3.0*x[i]*x[i];
  }
}

void Xpredicate(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  for (size_t i=0; i<n; i++) {
    if (x[i]>0) y[i] = x[i];
  }
}

void Xrecip(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  for (size_t i=0; i<n; i++) {
    y[i] = 1.0/x[i];
  }
}

void Xsqrt(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  for (size_t i=0; i<n; i++) {
    y[i] = std::sqrt(x[i]);
  }
}

void Xexp(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  for (size_t i=0; i<n; i++) {
    y[i] = std::exp(x[i]);
  }
}

void Xsin(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y)
{
  for (size_t i=0; i<n; i++) {
    y[i] = std::sin(x[i]);
  }
}

void Xpow(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  for (size_t i=0; i<n; i++) {
    y[i] = std::pow(x[i],0.55);
  }
}

void Xgather(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  #pragma omp simd
  for (size_t i=0; i<n; i++) {
    y[i] = x[map[i]];
  }
}

void Xscatter(size_t n, const size_t* __restrict__ map, const double* __restrict__ x, double* __restrict__ y) 
{
  #pragma omp simd
  for (size_t i=0; i<n; i++) {
    y[map[i]] = x[i];
  }
}

//double timer(size_t nthread, void(*f)(size_t, const size_t* __restrict__, const double* __restrict__, double* __restrict__)) {
double timer(size_t nthread, decltype(Xsimple)* f)
{
  double used = 0.0;
  double result = 0;

  if (nthread == 0) {

    ALIGNAS double x[N], y[N];
    ALIGNAS size_t map[N];
    for (size_t i=0; i<N; i++) {x[i] = 1.0; y[i] = 1.0; map[i]=globalmap[i];}

    used = omp_get_wtime();
    for (size_t repeat=0; repeat<NREPEAT; repeat++) {
      f(N, map, x, y);
      asm volatile("" ::: "memory");  // to stop loop reordering
    }
    used = omp_get_wtime() - used;
    if (y[10] > 1e99) std::cout << y[10] << std::endl; // To ensure loop is not completely optimized away

  } else {

    omp_set_num_threads(nthread);
#pragma omp parallel
    {
      int tid = omp_get_thread_num();
      ALIGNAS double x[N], y[N];
      ALIGNAS size_t map[N];
      for (size_t i=0; i<N; i++) {x[i] = 1.0; y[i] = 1.0; map[i]=globalmap[i];}
      
#pragma omp barrier
      if (tid == 0) used = omp_get_wtime();
      for (size_t repeat=0; repeat<NREPEAT; repeat++) {
	f(N, map, x, y);
	asm volatile("" ::: "memory");  // to stop loop reordering
      }
#pragma omp barrier
      if (tid == 0) used = omp_get_wtime() - used;
      if (y[10] > 1e99) std::cout << y[10] << std::endl; // To ensure loop is not completely optimized away
    }

  }

  result = used/(N*NREPEAT);
  printf("    %10.2e   ", result);
  return result;
}

int main() {
  std::random_device rdev;
  std::mt19937 rgen(rdev());
  std::uniform_int_distribution<size_t> idist(0,N-1);
  std::uniform_int_distribution<size_t> idist8(0,GATHERVL-1);

  // Make permutation arrays to test gather/scatter
  for (size_t i=0; i<N; i++) globalmap[i] = shortmap[i] = i;
  for (size_t i=0; i<N; i++) std::swap(globalmap[i],globalmap[idist(rgen)]);

  for (size_t i=0; i<N; i+=GATHERVL)
    for (size_t j=0; j<GATHERVL; j++)
      std::swap(shortmap[i+j],shortmap[i+idist8(rgen)]);
			       
  std::vector<std::pair<decltype(Xsimple)*,std::string>> kernels =
    {
     {&Xsimple,"simple"},
     {&Xgather,"gather"},
     {&Xscatter,"scatter"},
     {&Xgather,"shortgather"},
     {&Xscatter,"shortscatter"},
     {&Xpredicate,"predicate"},
     {&Xrecip,"reciprocal"},
     {&Xsqrt,"sqrt"},
     {&Xexp,"exp"},
     {&Xsin,"sin"},
     {&Xpow,"pow"},
    };

  printf("# threads");
  for (auto const & pair : kernels) {
    auto const & name = pair.second;
    printf("%15s  ", name.c_str());
  }
  printf("\n");
  printf("# -------");
  for (size_t i=0; i<kernels.size(); ++i) {
    printf("  ---------------");
  }
  printf("\n");
  fflush(stdout);

  //for (int nthread=0; nthread<=MAXTHREAD; nthread+=THREADSTRIDE) {
  for (size_t const nthread : THREAD_COUNTS) {
    printf("%6zu    ", nthread);
    for (auto const & pair : kernels) {
      auto const & kernel = pair.first;
      auto const & name = pair.second;

      if (name == "shortgather" or name == "shortscatter") std::swap(globalmap,shortmap);
#ifndef __aarch64__
      sleep(1); // to let x86 processor cool down so turbo mode can kick in
#endif
      timer(nthread, kernel);
      if (name == "shortgather" or name == "shortscatter") std::swap(globalmap,shortmap);
    }
    printf("\n");
    fflush(stdout);
  }

  return 0;
}

