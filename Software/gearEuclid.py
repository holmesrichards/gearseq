#!/usr/bin/python3

from math import gcd

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

def seqstr (a):
    ret = ""
    for ai in a:
        ret += "O" if ai == 1 else "-"
    return ret

def main():
    steps = 26
    events = 14


    for steps in range (8, 17):
        for events in range (3, steps-2): 
            print ("\nSteps", steps, "events", events)
            print ("Euclid")
            sse = seqstr(euclid(steps, events))
            print ("      ", sse)
            sser = [sse]
            for i in range (1, steps):
                sser.append (sser[-1][1:]+sser[-1][0:1])
            print ("Gap")
            for gen in range (1, int(steps/2)+1):
                if events <= steps / gcd(steps, gen):
                    ss3 = seqstr(gap(steps, events, gen))
                    print ("gen {:2d}".format(gen), ss3, "= Euclid" if ss3 in sser else "")

main()

