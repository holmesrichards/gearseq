# Gap sequence algorithm

The gap sequence algorithm generates a sequence based on four input parameters: period *P*, triggers *T*, generator *G*, and offset *O*.

In terse and abstract terms, there is a trigger at step *m* (in range 1 to *P*) if and only if *m* = (*O* + *k* * *G*) mod *P* + 1 for some *k* where 0 <= *k* < *T*. (Any positive value could be used for *G* but any *G* > *P*/2 will give the same pattern as one of the *G* values ≤ *P*/2. For instance, with *P* = 12, *G* = 7 gives the same pattern as *G* = 5 (but with an offset)). 

Less abstractly, imagine you have an analog clock with a chime. But instead of chiming from one to twelve times at each hour, it always chimes either once or not at all. There are twelve hours so the period is 12. Suppose the trigger number is 6. Now suppose the offset is 3; that means it will chime at 3:00. And now if the generator is 5, it will also chime five hours later, at 8:00; and five hours after that, at 1:00; and five hours after that, at 6:00; and likewise at 11:00 and 4:00. So in each twelve hour time period there will be chimes at 1:00, 3:00, 4:00, 6:00, 8:00, and 11:00. Time gaps are 1, 2, and 3 hours.

A gap sequencer is like that, except the period can be varied; and it doesn't chime, it makes triggers; and instead of one step per hour, the step rate is governed by a clock.

Note that if, for instance, we requested a generator of 4 instead of 5, we'd get chimes at 3:00, four hours later at 7:00, and four hours later at 11:00. But four hours after that is 3:00 again. So the maximum number of triggers with *P* = 12 and *G* = 4 is 3. In general, if *P* and *G* have a common factor, then the maximum number of triggers is reduced by that factor. For example, if *P* = 12:

|Generator|Maximum triggers|
|----|----|
|1|12|
|2|6|
|3|4|
|4|3|
|5|12|
|6|2|

By the three-gap theorem, the number of steps between consecutive triggers will have one of no more than three values, and if there are three values, the largest will be the sum of the other two. Not that this is necessarily important for trigger sequencing purposes, but there it is.
