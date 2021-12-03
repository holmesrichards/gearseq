# G.E.A.R. Sequncer implementation details

The algorithm pot is used to select the algorithm. Turn it hard left for "None" (no output), right for random, and in between for gap, Euclidean, or ADC.

The parameters — up to four of them — can be selected using the other pots and the associated control voltage inputs. With no cable plugged in a pot can be used to select a parameter manually; with a cable plugged in, the CV attenuated by the pot (in the range 0 to 5 V) determines the parameter.

If a parameter value is changed, or a different algorithm is selected, the change takes effect at the start of the next period. If you can't wait, use the Reset button.

Algorithm choice, parameter values, and the trigger pattern are shown on an OLED display. Normally it displays the values currently in use, and if a parameter has a different value requested than the current one (either because the current one has not been updated yet, or because the requested value is outside the current valid range) it is shown in brackets. While adjusting a pot, the display switches to showing the values that will go into effect at the next period if no further changes occur. As visual hints you are looking at future instead of present values, there is an asterisk in the upper left corner, and the trigger pattern is shown at the bottom with Xs and dots (representing trigger and no trigger) instead of the normal Os and dashes.

Other inputs are a clock, which steps the sequencer, and a reset pulse, which sends the sequence back to its start on the next clock. There are buttons for manual stepping and resetting too. Outputs are the sequence itself and a pulse at the start of each period. (If you don't need a trigger sequence but are hurting for clock dividers, just use the period output for any division up to 32!)

Internally, an Arduino Nano (clone) collects the clock, reset, and CV inputs, reads the pots, updates the OLED, calculates the sequence, and sends the sequence and period pulses out. It works pretty hard. Be nice to it.

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

## Dependencies

The following Arduino libraries must be installed:

* DirectIO [https://github.com/mmarchetti/DirectIO](https://github.com/mmarchetti/DirectIO)
* u8g2 [https://github.com/olikraus/u8g2/wiki](https://github.com/olikraus/u8g2/wiki)