#!/usr/bin/python3

# This just demonstrates the Euclidean and Gap algorithms

# invoke with e.g.
#
# python3 gearEuclid.py 16 5 7 2
#
# to see Euclidean pattern with period 16, # triggers 5, and offset 2 (7 is ignored)
# and Gap pattern with period 16, # triggers 5, generator 7, and offset 2

from math import gcd
from sys import argv

def gap (steps,  events, gen):
    storedRhythm = [1] + [0] * (steps-1)

    index = 0
    
    for var in range (1, events):
        index = (index + gen) % steps
        storedRhythm[index] = 1

    return storedRhythm

def euclid (steps,  events):
    storedRhythm = []

    bucket = 0

    for var in range (steps):
        bucket += events
        if bucket >= steps:
            bucket -= steps
            storedRhythm.append(1) #'1' indicates a event on this beat
        else:
            storedRhythm.append(0) #'0' indicates no event on this beat

    return storedRhythm

def seqstr (a, offs):
    ret = ""
    for ai in a[offs:]+a[:offs]:
        ret += "O" if ai == 1 else "-"
    return ret

def main():
    par = [16, 5, 7, 0]

    for i in range (1, 5):
        if len(argv) > i:
            par[i-1] = int(argv[i])

    print ("Period:", par[0], "| Triggers:", par[1], "| Generator:", par[2], "| Offset:", par[3])
    sse = seqstr(euclid(par[0], par[1]), par[3])
    print ("Euclidean", sse)
    sse = seqstr(gap(par[0], par[1], par[2]), par[3])
    print ("Gap      ", sse)

main()

