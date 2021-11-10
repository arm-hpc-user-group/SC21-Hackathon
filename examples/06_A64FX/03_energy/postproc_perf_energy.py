#!/usr/bin/env python
#
# John Linford <john.linford@arm.com>
#

"""
Post-process `perf stat` output on the Fujitsu A64FX
to calculate per-component energy consumption.
Example input file:

# started on Fri Feb 26 10:21:06 2021

1867047551;;r11:u;787943040;75.19;;
211827633;;r1e0:u;787951340;75.19;;
54584799;;r3e0:u;787894830;75.19;;
7136583;;r3e8:u;779850490;74.42;;
"""


def parse_args():
    import argparse
    parser = argparse.ArgumentParser(description="Postprocess perf stat output in CSV format")
    parser.add_argument('csv_file', help="`perf stat` output in CSV format (run with -x, flag)")
    parser.add_argument('-c', '--core_fact', help="Scalaing factor for the EA_CORE event 0x01e0", type=float, default=8.04)
    parser.add_argument('-l', '--l2_fact', help="Scalaing factor for the EA_L2 event 0x03e0", type=float, default=32.8)
    parser.add_argument('-m', '--mem_fact', help="Scalaing factor for the EA_MEM event 0x03e8", type=float, default=271)
    return parser.parse_args()

def calc_power(fact, pJ, ns):
    """Calculate energy consumption in Watts"""
    return fact * pJ * 1e-2 / ns

def calc_freq(cycles, ns):
    """ Calculate frequency in gigahertz"""
    return cycles * 1e-2 / ns

def report_metric(args, value, name, runtime, perc_duration):
    value = float(value)
    time = float(runtime) / float(perc_duration)
    if name.startswith('duration'):
        print("Elapsed Time: %g ns" % value)
    elif name.startswith('r11'):
        print("Core freq: %g GHz" % calc_freq(value, time))
    elif name.startswith('r1e0'):
        print("Per-core power: %g Watt" % calc_power(args.core_fact, value, time))
    elif name.startswith('r3e0'):
        print("Per-CMG L2 power: %g Watt" % calc_power(args.l2_fact, value, time))
    elif name.startswith('r3e8'):
        print("Per-CMG HBM power: %g Watt" % calc_power(args.mem_fact, value, time))
    else:
        print("Warning: unknown event: %s" % name)

def main():
    args = parse_args()
    with open(args.csv_file) as csv:
        for line in csv:
            line = line.strip()
            if line.startswith('#'):
                continue
            if not line:
                continue
            value, _, name, runtime, perc_duration, _, _ = line.split(';')
            report_metric(args, value, name, runtime, perc_duration)


if __name__ == "__main__":
    main()
