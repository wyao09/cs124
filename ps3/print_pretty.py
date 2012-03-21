import sys

s = "Buffy the Vampire Slayer fans are sure to get their fix with the DVD release of the show's first season. The three-disc collection includes all 12 episodes as well as many extras. There is a collection of interviews by the show's creator Joss Whedon in which he explains his inspiration for the show as well as comments on the various cast members. Much of the same material is covered in more depth with Whedon's commentary track for the show's first two episodes that make up the Buffy the Vampire Slayer pilot. The most interesting points of Whedon's commentary come from his explanation of the learning curve he encountered shifting from blockbuster films like Toy Story to a much lower-budget television series. The first disc also includes a short interview with David Boreanaz who plays the role of Angel. Other features include the script for the pilot episodes, a trailer, a large photo gallery of publicity shots and in-depth biographies of Whedon and several of the show's stars, including Sarah Michelle Gellar, Alyson Hannigan and Nicholas Brendon."

l = s.split()
words = len(l)

if len(sys.argv) != 2:
    print "ussage: print_pretty <characters per line>"
    sys.exit()

m = int(sys.argv[1])

max_fit = []
cost = [] #list of lists

# populate max_fit
for i in range (0, words):
    sum = -1
    fit = 0
    while sum <= m and i + fit < words:
        sum += len(l[i+fit]) + 1
        fit += 1
    max_fit.append(fit - 1)

# populate cost
for i in range (0, words):
    c = []
    n = m + 1
    upper = max_fit[i] + i
    for j in range (i, upper):
        n -= len(l[j]) + 1
        c.append(n*n*n) #optimize
    cost.append(c)

optimize = [0]*words
answer = [0]*words # number of words to print - 1

def dp(n):
    min = m*m*m
    for i in range(0, max_fit[n]):
        t = cost[n][i]
        if i+1 < words:
            t += optimize[n+i+1]
        if t < min:
            min = t
            answer[n] = i
    return min

#final line invariant (base cases)
t = -1
for i in range(words-1, -1, -1):
    if len(l[i]) + t + 1 <= m:
        answer[i] = words
        t += len(l[i]) + 1
    else:
        break

for i in range (i, -1, -1):
    optimize[i] = dp(i)

skip = answer[0]

print "minimumal penalty: " + str(optimize[0])
