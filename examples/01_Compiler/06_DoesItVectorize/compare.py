#!/usr/bin/env python3


import os
import sys


def main():
    compare_threads = sys.argv[1]
    input_files = sys.argv[2:]

    compilers = []
    perf_data = {}

    for filename in input_files:
        compilers.append(filename[4:].replace(':', '/'))
        with open(filename) as fin:
            kernels = None
            for line in fin:
                if line.startswith("# threads"):
                    kernels = line.split()[2:]
                    for kern_name in kernels:
                        perf_data.setdefault(kern_name, [])
                    continue
                elif line.startswith("# -------"):
                    continue
                else:
                    if not kernels:
                        print("Invalid file format: %s" % filename)
                        sys.exit(1)
                    fields = line.split()
                    nthreads = fields[0]
                    if nthreads != compare_threads:
                        continue
                    values = [float(x) for x in fields[1:]]
                    for i, kern_name in enumerate(kernels):
                        perf_data[kern_name].append(values[i])

    print("")
    for i, compiler_path in enumerate(compilers):
        print("Compiler %3d: %s" % (i, compiler_path))
    print("")
    print("                 ", end="")
    for i in range(len(compilers)):
        print("%10d " % i, end="")
    print("")
    print("-"*(20+10*len(compilers)))
    for kern_name, values in perf_data.items():
        print(("%15s: " % kern_name), end="")
        for value in values:
            print("%10g " % value, end="")
        print("")


if __name__ == "__main__":
    main()
