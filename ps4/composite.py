import math
import sys
import random
from mpmath import mp

n = mp.mpf(sys.argv[1]) # odd integer to be tested for primality
s = int(sys.argv[2]) # n-1 as 2s*d
d = mp.mpf(sys.argv[3])
k = int(sys.argv[4]) # parameter that determines the accuracy of the test


# find s and d first


def composite():
    for i in range(0, k):
        a = mp.mpf(random.randint(2,n-2))
        x = mp.power(a,d)
        x = mp.fmod(x,n)
        if x != 1 and x != n-1:#
            for r in range(i, s-1):
                x = mp.fmod(mp.power(x,2),n)
                if x == 1:#
                    return a
                if x != n-1:#
                    return a
    return -1
    
print composite()
