import math
import sys
import random
from mpmath import mp

if len(sys.argv) != 3:
    print "usage: composite [odd integer to be tested] [parameter for accuracy]"
    print "output with n-1 as 2^s * d: (witness, s, d)"
    sys.exit()

n = int(sys.argv[1]) # odd integer to be tested for primality
"""
s = int(sys.argv[2]) # n-1 as 2^s*d
d = mp.mpf(sys.argv[3])
"""
k = int(sys.argv[2]) # parameter that determines the accuracy of the test
s = 0
d = 0

# find s and d first
for i in range(0, 30):
    x = mp.fdiv((n-1), mp.power(2,i))
    if x - long(x) == 0:
        s = i
        d = int(x)

def composite():
    for i in range(0, k):
        a = random.randint(2,100)#n-2)
        x = mp.fmod(mp.power(a,d),n)
        if x != 1 and x != n-1:#
            comp = True
            for r in range(1, s):
                x = mp.fmod(mp.power(x,2),n)
                if x == 1:#
                    return a, s, d, r
                if x == n-1:#
                    comp = False
                    break
                """
            if comp:    
                return a, s, d
            else:
                print 'ha'
                """
    return -1, s, d
    
print composite()
