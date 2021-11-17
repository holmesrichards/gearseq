<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**  *generated with [DocToc](https://github.com/thlorenz/doctoc)*

- [G.E.A.R. Sequencer](#gear-sequencer)
  - [Algorithms](#algorithms)
  - [Implementation](#implementation)
  - [Pot assignments](#pot-assignments)
    - [Pot+CV 1 and 4](#potcv-1-and-4)
    - [Pot+CV 2](#potcv-2)
    - [Pot+CV 3](#potcv-3)
    - [Parameter ranges](#parameter-ranges)
  - [Usage notes](#usage-notes)
  - [Dependencies](#dependencies)
  - [Submodules](#submodules)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# G.E.A.R. Sequencer

This is a voltage controlled trigger sequencer or, perhaps a better term, trigger sequence generator. Rather than sending triggers in a sequence that is completely specified by the user, it creates sequences based on user specified parameters.

It offers a choice of four algorithms:

* Gap sequence
* Euclidean sequence
* ADC sequence
* Random sequence

(Hence the name G.E.A.R. Sequencer. Within limits of program memory and programmer sanity, there is no reason more algorithms could not be added.)

Each algorithm is governed by up to four parameters whose values the user can dial in on potentiometers, or they can be set using control voltages.

## Algorithms

**Euclidean sequencing** is familiar and I won't go into the details here — see [http://cgm.cs.mcgill.ca/~godfried/publications/banff-extended.pdf](http://cgm.cs.mcgill.ca/~godfried/publications/banff-extended.pdf) if you need them. The sequence pattern is determined by two parameters: Repetition period (*P*) and number of triggers (*T*). The algorithm distributes the *T* triggers as uniformly as possible among the *P* sequence steps. A third parameter, the offset (*O*), governs not the pattern itself but where the pattern starts/stops relative to where the period starts/stops.

A more novel algorithm is the **gap sequence,** based on the procedure underlying linear scales in tuning theory and the [Three-gap Theorem](https://en.wikipedia.org/wiki/Three-gap_theorem) in mathematics — not that you need to know that. I'm not aware of any prior use of this idea for trigger sequencing although I would be quite surprised if no one else has done it before. In a gap sequence the pattern is defined by three parameters: *P*, *T*, and the generator (*G*). For a given *P* and *T* there is exactly one Euclidean sequence, but there are up to *P*/2 gap sequences corresponding to different generators. Again, the position of the sequence start relative to the period can be specified by *O*. You can find details of the algorithm [here](Docs/gap_sequence.md).

An **ADC sequence** uses a pattern based on digitization of an analog control voltage. You specify *P* and a value to digitize, *V*, as well as, again, *O*. The sequence pattern is the first *P* bits in the digitization of *V*. This is similar to something the Barton Musical Circuits BMC006 Voltage To Rhythm Converter does (though the BMC006 does a lot more with the idea). 

A **random sequence** is fairly self explanatory. For a given *P* and *T*, the algorithm distributes *T* triggers randomly within *P* steps. The random pattern is then repeated indefinitely, offset by *O* as with the other types, until a new sequence is requested. Another "parameter" is the throw (*Th*). The value of the throw is not used to determine the pattern, but if it changes, a new random pattern will be chosen. 

## Implementation

The algorithm pot* is used to select the algorithm. Turn it hard left for "None" (no output), right for random, and in between for gap, Euclidean, or ADC.

The parameters — up to four of them — can be selected using the other pots and the associated control voltage inputs. With no cable plugged in a pot can be used to select a parameter manually; with a cable plugged in, the CV attenuated by the pot (in the range 0 to 5 V) determines the parameter. 

If a parameter value is changed, or a different algorithm is selected, the change takes effect at the start of the next period. If you can't wait, use the Reset button.

Algorithm choice, parameter values, and the trigger pattern are shown on an OLED display. Normally it displays the values currently in use, and if a parameter has a different value requested than the current one (either because the current one has not been updated yet, or because the requested value is outside the current valid range) it is shown in brackets. While adjusting a pot, the display switches to showing the values that will go into effect at the next period if no further changes occur. As visual hints you are looking at future instead of present values, there is an asterisk in the upper left corner, and the trigger pattern is shown at the bottom with Xs and dots (representing trigger and no trigger) instead of the normal Os and dashes.

Other inputs are a clock, which steps the sequencer, and a reset pulse, which sends the sequence back to its start on the next clock. There are buttons for manual stepping and resetting too. Outputs are the sequence itself and a pulse at the start of each period. (If you don't need a trigger sequence but are hurting for clock dividers, just use the period output for any division up to 32!)

Internally, an Arduino Nano (clone) collects the clock, reset, and CV inputs, reads the pots, updates the OLED, calculates the sequence, and sends the sequence and period pulses out. It works pretty hard. Be nice to it.

\* Pots are used instead of rotary switches to allow larger and variable, software defineable numbers of choices. Rotary encoders would have been more complicated and expensive, using more Arduino pins to read, and harder to integrate with control voltages.

## Pot assignments

### Pot+CV 1 and 4

Pot+CV 1 always determines the period *P* and pot+CV 4 determines the offset *O*. 

### Pot+CV 2

For all but the ADC algorithm, pot+CV 2 determines the number of triggers *T*. For ADC, it is the value to be digitized, *V*. 

### Pot+CV 3

Pot+CV 3's function varies between algorithms:

* Gap: Generator *G*.
* Euclidean or ADC: Not used.
* Random: Throw *Th*. Not used in determining the pattern, but a change in this parameter causes a new sequence to be generated.

### Parameter ranges

Parameters may be requested within ranges as follows. The (dynamic) valid range for a parameter may be smaller than the (static) range of values that can be requested. If the requested value is outside the valid range, it will be adjusted to a valid value.

|Pot|Parameter|Algorithm|Requested range|Valid range|
|----|----|----|----|----|
|1|*P*|G,E,A,R|2–32|ADC: 2–10; others: 2–32|
|2|*T*|G,E,R|0–32|0–*P* *|
|2|*V*|A|0–1023|0–1023|
|3|*G*|G|1–16|1–*P*/2|
|3|*Th*|R|0–31|0–31|
|4|*O*|G,E,A,R|0–31|0–*P*-1

The limit of 32 for *P*, and the related upper limits for *T*, *G*, and *O*, can be changed in software. Reducing them should be trivial; increasing them would involve some coding. For ADC the limits of 10 for *P* and 1023 for *V* can't be increased; you could decrease them, though. (They're related: *V_max* = 2^*P_max* - 1.)

\* **Note:** For the gap algorithm, the actual maximum number of triggers may be some fraction of *P* (depending on *G*), in which case the requested value of *T* will be truncated if it is larger.

## Usage notes

-  Unlike with the Euclidean and random algorithms, where changing *T* by one can give a completely different pattern, incrementing *T* in the gap algorithm simply converts one non-trigger step in the pattern to a trigger step. This is something that can be usefully exploited with a slowly varying control voltage, gradually adding or removing triggers from the sequence pattern.

- When *G* is an even divisor of *P* in the gap algorithm, the maximum number of triggers is only a fraction of *P*. (Specifically, *P* divided by the greatest common divisor of *P* and *G*.) So periods with lots of divisors, such as 12, offer fewer possibilities than prime periods, like 11 or 13. To get more possibilities for 12, you can "cheat": set *P* to 13. Now you can get as many triggers as you want with *G* being any number from 1 through 6. Then use an external clock divider to pulse the reset input every 12 clock pulses.

## Dependencies

The following Arduino libraries must be installed:

* DirectIO [https://github.com/mmarchetti/DirectIO](https://github.com/mmarchetti/DirectIO)
* u8g2 [https://github.com/olikraus/u8g2/wiki](https://github.com/olikraus/u8g2/wiki)


## Submodules

This repo uses submodules aoKicad and Kosmo_panel, providing libraries needed for KiCad. To clone:

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
