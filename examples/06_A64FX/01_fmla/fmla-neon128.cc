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

#ifdef _CRAYC
#error Example does not support cray compiler
#endif

#include <iostream>
#include <cstdint>
#include <chrono>
#include <cstdlib>

extern "C" void kernel(std::uint64_t);

// unroll 4x
asm(R"(
	.global kernel
kernel:
	mov x20, xzr
  
loop:
	fmla v0.2d, v30.2d, v31.2d
	fmla v1.2d, v30.2d, v31.2d
	fmla v2.2d, v30.2d, v31.2d
	fmla v3.2d, v30.2d, v31.2d
	fmla v4.2d, v30.2d, v31.2d
	fmla v5.2d, v30.2d, v31.2d
	fmla v6.2d, v30.2d, v31.2d
	fmla v7.2d, v30.2d, v31.2d
	fmla v8.2d, v30.2d, v31.2d
	fmla v9.2d, v30.2d, v31.2d
	fmla v10.2d, v30.2d, v31.2d
	fmla v11.2d, v30.2d, v31.2d
	fmla v12.2d, v30.2d, v31.2d
	fmla v13.2d, v30.2d, v31.2d
	fmla v14.2d, v30.2d, v31.2d
	fmla v15.2d, v30.2d, v31.2d
	add x20, x20, 1
	fmla v0.2d, v30.2d, v31.2d
	fmla v1.2d, v30.2d, v31.2d
	fmla v2.2d, v30.2d, v31.2d
	fmla v3.2d, v30.2d, v31.2d
	fmla v4.2d, v30.2d, v31.2d
	fmla v5.2d, v30.2d, v31.2d
	fmla v6.2d, v30.2d, v31.2d
	fmla v7.2d, v30.2d, v31.2d
	fmla v8.2d, v30.2d, v31.2d
	fmla v9.2d, v30.2d, v31.2d
	fmla v10.2d, v30.2d, v31.2d
	fmla v11.2d, v30.2d, v31.2d
	fmla v12.2d, v30.2d, v31.2d
	fmla v13.2d, v30.2d, v31.2d
	fmla v14.2d, v30.2d, v31.2d
	fmla v15.2d, v30.2d, v31.2d
	cmp x0, x20
	fmla v0.2d, v30.2d, v31.2d
	fmla v1.2d, v30.2d, v31.2d
	fmla v2.2d, v30.2d, v31.2d
	fmla v3.2d, v30.2d, v31.2d
	fmla v4.2d, v30.2d, v31.2d
	fmla v5.2d, v30.2d, v31.2d
	fmla v6.2d, v30.2d, v31.2d
	fmla v7.2d, v30.2d, v31.2d
	fmla v8.2d, v30.2d, v31.2d
	fmla v9.2d, v30.2d, v31.2d
	fmla v10.2d, v30.2d, v31.2d
	fmla v11.2d, v30.2d, v31.2d
	fmla v12.2d, v30.2d, v31.2d
	fmla v13.2d, v30.2d, v31.2d
	fmla v14.2d, v30.2d, v31.2d
	fmla v15.2d, v30.2d, v31.2d
	fmla v0.2d, v30.2d, v31.2d
	fmla v1.2d, v30.2d, v31.2d
	fmla v2.2d, v30.2d, v31.2d
	fmla v3.2d, v30.2d, v31.2d
	fmla v4.2d, v30.2d, v31.2d
	fmla v5.2d, v30.2d, v31.2d
	fmla v6.2d, v30.2d, v31.2d
	fmla v7.2d, v30.2d, v31.2d
	fmla v8.2d, v30.2d, v31.2d
	fmla v9.2d, v30.2d, v31.2d
	fmla v10.2d, v30.2d, v31.2d
	fmla v11.2d, v30.2d, v31.2d
	fmla v12.2d, v30.2d, v31.2d
	fmla v13.2d, v30.2d, v31.2d
	fmla v14.2d, v30.2d, v31.2d
	fmla v15.2d, v30.2d, v31.2d
	bne loop
end:
	ret
)");

int main(int argc, char ** argv) 
{
    using namespace std;

    const uint64_t iters = 1000000ul;

    const uint64_t lanes = 16 / sizeof(double);
    const uint64_t flops = 32; // 16 fmla = 32 flops
    const uint64_t unroll = 4; // match kernel asm
    const uint64_t kernel_flops = lanes * flops * unroll * iters;

    auto start = chrono::steady_clock::now();
    kernel(iters);
    auto end = chrono::steady_clock::now();
    double seconds=chrono::duration_cast<chrono::microseconds>(end-start).count();
    seconds = seconds * 1e-6;
    double gflops = kernel_flops / (seconds * 1e9);

    cout << kernel_flops << " Flops in " << seconds << " seconds" << endl;
    cout << gflops << " GFlops" << endl;

    return 0;
}
