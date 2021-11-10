#!/usr/bin/env python

import sys
import re

with open(sys.argv[1]) as fp:  
   line = fp.readline()
   cnt = 0
   rds = 0
   wts = 0
   sizes = {}
   bundle = {}
   while line:
       line = fp.readline()
       values = re.split(': |, ', line)
       if len(values) == 7:     
         cnt += 1
         if int(values[3]) == 1:
            wts += 1
         elif int(values[3]) == 0:
            rds += 1
         else:
            print values

         sizes[values[4]] = sizes.get(values[4], 0) + 1
         #bundle[values[2]] = bundle.get(values[2], 0) + 1

   print ("Total Operations: ", cnt)
   rd_per = (rds * 100.0) / cnt
   wt_per = (wts * 100.0) / cnt

   print("Reads {} ({}%), Writes {} ({}%)".format(rds, rd_per, wts, wt_per))
   print("")
   print("Data sizes:")
   print sizes
   print("")
   #print("SVE Bundle:")
   #print bundle
