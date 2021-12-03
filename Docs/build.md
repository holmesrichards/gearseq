# G.E.A.R. Sequencer build and software notes

The build is pretty straightforward. Note there are no socketed ICs, but you should socket the Nano. Use a large DIP socket (normally you'd find 32 pin sockets, not 30, but you can bend or cut away two pins) or two 1x15 pin sockets. In the latter case put a Nano in the sockets (use a dead one if you have one) before soldering to make sure they stay aligned.

Also note this module makes no use of -12 V, and +12 V is used only to feed 5 V regulators — the one on the Nano, and an external one. So this design should be usable with any kind of power supply that can power a 5 V regulator, including a 9 V battery. Presumably. I haven't tried it!

## Errata

One sort-of error on the first run PCB: C4 (330 µF) has the wrong footprint, with the holes too close together. Not really a problem, just bend the leads toward each other on your capacitor. You can use an electrolytic here if you'd rather, just make sure the negative (-) lead is in the hole closer to D1.

## Construction

The two circuit boards are connected with a 12-pin header/socket pair, and with two 10 mm M3 spacers, secured with 6 mm M3 screws through the holes in the PCBs.

Pay attention to what goes on what side. On the main PCB, the jacks, LEDs, and 12-pin header go on one side, everything else on the other. On the pots board the pots and toggle switch go on one side, the 12-pin socket goes on the other. That should be evident from the silkscreen.

## Panel components

There are footprints provided for both 9 mm and 16 mm vertical board mount pots. Buy and mount only one or the other! (You can mix and match between pot positions.)

Three of the LEDs are marked as "green" and one "red"; of course you can use whatever colors you prefer. Associated with them are four resistors with value shown as "RL". Use whatever value works well with your LEDs. Minimum value should be (5-Vf)/Imax where Vf and Imax are the forward voltage and maximum current from the LED datasheet, but you probably will want a larger value for a less blinding brightness. I used regular green LEDs (D9, D16, and D17 for the clock, sequence, and period, respectively) and a superbright red (D8 for reset) from Tayda with 470 Ω resistors for the greens (R10, R14, R16) and 2 kΩ for the red (R9).

As usual with the pots, jacks, LEDs, and toggle switch, put them in position; place the front panel over them and fasten it down with nuts on some of the panel components; then and only then solder them.

The OLED can be MC096VX or MC096GX or other compatible OLED. Note these have VCC and GND in the opposite order on their header, so reverse their order with the jumpers if you need to. You can get these displays in white, blue, or blue and yellow versions; any of these will work. The reason it is mounted upside down is to have the yellow stripe at the bottom, if you use a blue and yellow version, to highlight the trigger pattern on the display. The OLED should be mounted to the front panel using M2 screws. I secured four screws to the panel with nuts, then put the OLED on and secured it with four more nuts. 

The OLED and the push button switches connect to the PCB via wires. You can solder the wires directly to the PCB, but for the push buttons I recommend fitting Molex connectors to the wires and Molex headers on the board. Route the wires through the slot in the PCB. For the OLED I mounted a 1x4 pin header on the PCB and used short female-female Dupont jumpers to connect it up. 

## Software

Upload the gearSeqSketch.ino sketch (in the folder Software/gearSeqSketch). The software requires you install two libraries:

* DirectIO [https://github.com/mmarchetti/DirectIO](https://github.com/mmarchetti/DirectIO)
* u8g2 [https://github.com/olikraus/u8g2/wiki](https://github.com/olikraus/u8g2/wiki)

Upload the software to the Nano and then unplug the USB cable before connecting to +12 V power.

Also in the Software folder is a Python script, gearEuclid.py. This is not needed for anything, and does not work with the module, it just demonstrates the Gap and Euclidean algorithms in a probably more readable form than the Arduino sketch. You can run it on the command line of your computer with four parameters as arguments and see what each algorithm produces for a trigger pattern. For instance, try: `python3 gearEuclid.py  16 5 7 2`.