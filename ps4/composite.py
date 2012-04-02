import math
import sys
import random
from mpmath import mp

n = int(sys.argv[1]) # odd integer to be tested for primality
"""
s = int(sys.argv[2]) # n-1 as 2s*d
d = mp.mpf(sys.argv[3])
"""
k = int(sys.argv[2]) # parameter that determines the accuracy of the test
s = 0
d = 0

# find s and d first
for i in range(0, 20):
    x = mp.fdiv((n-1), mp.power(2,i))
    if x - long(x) == 0:
        s = i
        d = int(x)

def composite():
    for i in range(0, k):
        a = random.randint(2,n-2)
        x = mp.fmod(mp.power(a,d),n)
        if x != 1 and x != n-1:#
            for r in range(i, s-1):
                x = mp.fmod(mp.power(x,2),n)
                if x == 1:#
                    return a, s, d
                if x != n-1:#
                    return a, s, d
    return -1, s, d
    
print composite()
