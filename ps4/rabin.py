import random
 
trials = 100
 
def test_prime(n):
    # write n-1 as 2^s * d
    s = 0
    d = n-1
    while True:
        q, r = divmod(d, 2)
        if r == 1:
            break
        s += 1
        d = q
    assert(2**s * d == n-1)
 
    # test for witness
    def composite(a):
        if pow(a, d, n) == 1:
            return False
        for i in range(s):
            if pow(a, 2**i * d, n) == n-1:
                return False
            print str(a) + "^(2^" + str(i) + ")*" + str(d) + " mod " + str(n) + " = " + str(pow(a, 2**i * d, n))
        print str(a) + " is witness of " + str(n)
        return True
 
    for i in range(trials):
        if 100 > n:
            u = n
        else:
            u = 100
        a = random.randrange(2, u)
        if composite(a):
            return False
 
    return True

print ""
test_prime(636127)
print ""
test_prime(294409)
print ""
