# G.E.A.R. Sequencer

A trigger sequencer or, perhaps a better term, trigger sequence generator. Rather than sending triggers in a sequence that is completely specified by the user, it creates sequences based on user specified parameters.

It offers a choice of four algorithms:

* Gap sequence
* Euclidean sequence
* ADC sequence
* Random sequence

(Hence the name G.E.A.R. Sequencer. Within limits of program memory, there is no reason more algorithms could not be added.)

## Algorithms

Euclidean sequencing is familiar and need not be described here other than to say the sequence pattern is determined by two parameters: Repetition period (*P*) and number of triggers (*N*). The algorithm distributes the *N* triggers as uniformly as possible among the *P* sequence steps. A third parameter, the rotational offset (*O*), governs not the pattern itself but where the pattern starts/stops relative to where the period starts/stops.

More novel is the gap sequence, based on the procedure underlying linear scales in tuning theory and the [Three-gap Theorem](https://en.wikipedia.org/wiki/Three-gap_theorem) in mathematics. I'm not aware of any prior use of this idea for trigger sequencing although I would be quite surprised if no one else has done it before. In a gap sequence the pattern is defined by three parameters: *P*, *N*, and the generator (*G*). Whereas for a given *P* and *N* there is exactly one Euclidean sequence, there are up to *P*/2 gap sequences corresponding to different generators. Again, the position of the sequence start relative to the period can be specified by *O*. Details of the algorithm are given below.

An ADC sequence uses a pattern based on digitization of an analog control voltage. The Barton Musical Circuits BMC006 Voltage To Rhythm Converter does this. In the G.E.A.R. sequencer there are two variants, Direct, in which the bits in the digitized value of the CV are the sequence pattern, and Gray, in which each bit is XORed with the next one to give the sequence pattern. The attractive thing about the Gray variant is that if one slowly ramps the CV up or down, the resulting consecutive patterns differ in only one bit. Again *P* and *O* (but not *N*) can be specified.

A random sequence is fairly self explanatory. For a given *P* and *N*, the algorithm distributes *N* triggers randomly within *P* steps. The random pattern is then repeated indefinitely, offset by *O* as with the other types, until a new sequence is requested. Another "parameter" is the change (*C*). While the value of the change is not used to generate the pattern, if it changes, a new random pattern will be chosen. 

## Implementation

The algorithm and its (up to four) parameter values can be selected using five potentiometers and four CV inputs. With no cable plugged in a pot can be used to select a parameter manually; with a cable plugged in, the CV attenuated by the pot determines the parameter. Parameter values are shown on an OLED display. 

If a parameter value is changed, or a different algorithm is selected, the change takes effect at the start of the next period.

Other inputs are a clock and a reset pulse which sends the sequence back to its start on the next clock. Outputs are the sequence itself and a pulse at the start of each period. (One could ignore the sequence output and just use the period output as a clock divider.)

Internally, an Arduino Nano collects the clock, reset, and CV inputs, reads the pots, updates the OLED, calculates the sequence, and sends the sequence and period pulses out.

Pots are used instead of rotary switches to allow larger and variable, software defineable numbers of choices. (Rotary encoders would have been more complicated and expensive, and harder to integrate with control voltages.) One pot is for algorithm selection (or none, to disable the outputs). The other four, and the four CVs, are labeled 1 through 4. 

### Pot+CV 1 and 4

For the four G.E.A.R. algorithms (not necessarily for any future added algorithms), pot+CV 1 always determines the period *P* and pot+CV 4 determines the rotational offset *O*. 

### Pot+CV 2

For all but ADC, pot+CV 2 determines the number of triggers *N*. For ADC, it is the value to be digitized. 

### Pot+CV 3

Pot+CV 3's function varies between algorithms:

* Gap: Generator *G*.
* Euclidean: Not used.
* ADC: Choice of Direct or Gray variant.
* Random: 

### Parameter ranges:

Pot and CV ranges are as follows. The valid range for a parameter may be smaller than the range of values that can be requested. If the requested value is outside the valid range, it will be adjusted to a valid value.

|Pot|Parameter|Requested range|Valid range|
|----|----|----|----|
|1|*P*|2–32|ADC: 2–10; others: 2–32|
|2|*N*|0–32|0–*P* *|
|3|*G*|0–16|0–*P*/2|
|3|*C*|0–16|0–16|
|4|*O*|0–31|0–*P*-1

The limit of 32 for *P* can be changed in software.

\* For the Gap algorithm, the actual maximum number of triggers may be some fraction of *P* (depending on *G*), in which case the requested value of *N* will be truncated if it is larger.

## Gap sequence algorithm

Given period *P*, number *N*, generator *G*, and offset *O*, there is a trigger at step *m* (in range 1 to *P*) if and only if *m* = (*O* + *k* * *G*) mod *P* + 1 for some *k* where 0 <= *k* < *N*. (Any positive value could be used for *G* but any *G* > *P*/2 will give the same pattern as one of the *G* values ≤ *P*/2. For instance, with *P* = 12, *G* = 7 gives the same pattern as *G* = 5 (but with a rotational offset)). 

By the three-gap theorem, the number of steps between consecutive triggers will have one of no more than three values, and if there are three values, the largest will be the sum of the other two. Not that this is necessarily important for trigger sequencing purposes, but there it is.

Less abstractly, imagine you have an analog clock with a chime. But instead of chiming from one to twelve times at each hour, it always chimes either once or not at all. There are twelve hours so the period is 12. Suppose the number is 6. Now suppose the offset is 3; that means it will chime at 3:00. And now if the generator is 5, it will also chime five hours later, at 8:00; and five hours after that, at 1:00; and five hours after that, at 6:00; and likewise at 11:00 and 4:00. So in each twelve hour time period there will be chimes at 1:00, 3:00, 4:00, 6:00, 8:00, and 11:00. Time gaps are 1, 2, and 3 hours.

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

## Submodules

This repo uses submodules aoKicad and Kosmo_panel, needed for KiCad. To clone:

```
git clone git@github.com:holmesrichards/gapseq.git
git submodule init
git submodule update
```


Alternatively do

```
git clone --recurse-submodules git@github.com:holmesrichards/gapseq.git
```

Or if you download the repository as a zip file, you must also click on the "aoKicad" and "Kosmo\_panel" links on the GitHub page (they'll have "@ something" after them) and download them as separate zip files which you can unzip into this repo's aoKicad and Kosmo\_panel directories.

If desired, copy the files from aoKicad and Kosmo\_panel to wherever you prefer (your KiCad user library directory, for instance, if you have one). Then in KiCad, add symbol libraries 

```
aoKicad/ao_symbols
Kosmo_panel/Kosmo
```
and footprint libraries 
```
aoKicad/ao_tht
Kosmo_panel/Kosmo_panel.
```
