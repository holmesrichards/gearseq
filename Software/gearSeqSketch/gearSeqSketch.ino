/* Gap sequencer Arduino sketch

   Rich Holmes 2021 CC0 1.0

   Portions drawn from 
   https://github.com/katspaugh/arduino-modular-synth/blob/master/arduino-euclid/arduino-euclid.ino
   (No stated license)

 */

#define TRIG1_OUT_PIN       2
#define TRIG2_OUT_PIN       3
#define PER1_OUT_PIN        4
#define PER2_OUT_PIN        5
#define RESET1_IN_PIN       6
#define RESET2_IN_PIN       7
#define CLOCK_PIN           8

#define PER1_POT  0
#define PER2_POT  1
#define TRIG1_POT 2
#define TRIG2_POT 3
#define GEN1_POT  4
#define GEN2_POT  5
#define OFF1_POT  6
#define OFF2_POT  7

#define MAX_PERIOD          32

int trig_out_pin[2] = {TRIG1_OUT_PIN, TRIG2_OUT_PIN};
int per_out_pin[2] = {PER1_OUT_PIN, PER2_OUT_PIN};
int reset_out_pin[2] = {RESET1_OUT_PIN, RESET2_OUT_PIN};

// Parameters for each sequence
int period[2] = {MAX_PERIOD, MAX_PERIOD};  // Period (# of steps)
int rtriggers[2] = {1, 1};                 // Number of triggers requested
int triggers[2] = {1, 1};                  // Number of triggers actual
int generator[2] = {1, 1};                 // Generator (-1 for Euclidean,
                                           // -2 for random)
int offset[2] = {0, 0};                    // Rotational offset
bool states[2][MAX_PERIOD];                // List of on/off values
int counter[2] = {-1, -1};                 // Current step #

bool send_tick = false;
bool clock_state = false;
bool last_clock_state = false;
bool last_reset_state[2] = {false, false};

// Pots handling

int per_pot[2] = {PER1_POT, PER2_POT};
int trig_pot[2] = {TRIG1_POT, TRIG2_POT};
int gen_pot[2] = {GEN1_POT, GEN2_POT};
int off_pot[2] = {OFF1_POT, OFF2_POT};

int last_pot_state[8] = {9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999};
int last_change_time = 0;
bool change_pending = false;

int gcd (int a,int b)
{
  int R;
  while ((a % b) > 0)
    {
      R = a % b;
      a = b;
      b = R;
    }
  return b;
}

int getPot (int ip2)
{
  // Return pot value (directly or through multiplexer as appropriate)
  
  return analogRead (A0+ip2);
}

int actTriggers (int iseq)
{
  // For gap sequence, actual triggers may be less than requested
  // Return actual triggers
  
  mxt = generator[iseq] > 0 ? period[iseq] / gcf(period[iseq], generator[iseq]) : period[iseq];
  return constrain (rtriggers[iseq], 0, mxt);
}

int potToPeriod(int potvalue)
{
  // Map period pot to 1..MAX_PERIOD

  return int(float(potvalue)/(1024/MAX_PERIOD))+1;
}

int potToTriggers(int potvalue, int iseq)
{
  // Map trig pot to 0..period

  return int(float(potvalue)/(1024/(period[iseq]+1)));
}

int potToGenerator(int potvalue, int iseq)
{
  // Map gen pot to -2, -1, or 1..period/2

  max_gen = period[iseq]/2;
  new_gen = int(float(gen_pot[iseq])/(1024/(max_gen+2)));
  if (new_gen == max_gen)
    new_gen == -1;
  else if (new_gen == max_gen + 1)
    new_gen == -2;
  else
    new_gen += 1;
  return new_gen;
}

int potToOffset(int potvalue, int iseq)
{
  // Map off pot to 0..period-1

  return int(float(off_pot[iseq])/(1024/(period[iseq])));
}

void disPot (int ipot, int iseq, int pot_state)
{
  // Print pot status to serial monitor
  
  Serial.print ("Sequence ");
  Serial.print (iseq+1);
  Serial.print (" Pot ");
  Serial.print (ipot);
  Serial.print (" = ");
  Serial.print (pot_state);
  Serial.print (" -> ");
  if (ipot == PER1_POT || ipot == PER2_POT)
    {
      Serial.print ("Period ");
      Serial.println (potToPeriod(pot_state));
    }
  else if (ipot == TRIG1_POT || ipot == TRIG2_POT)
    {
      Serial.print ("Triggers ");
      Serial.println (potToTriggers(pot_state, iseq));
    }
  else if (ipot == GEN1_POT || ipot == GEN2_POT)
    {
      Serial.print ("Generator ");
      int g = potToGenerator(pot_state, iseq);
      if (g == -1)
	Serial.println ("[Euclidean]");
      else if (g == -2)
	Serial.println ("[Random]");
      else
	Serial.println (potToGenerator(pot_state, iseq));
    }
  else if (ipot == OFF1_POT || ipot == OFF2_POT)
    {
      Serial.print ("Offset ");
      Serial.println (potToOffset(pot_state, iseq));
    }
}  

bool checkPots()
{
  // Read pots and store new values if changed enough
  
  bool something_changed = false;
  for (int ipot = 0; ipot < 4; ++ipot)
    {
      for (int iseq = 0; iseq < 2; ++iseq)
	{	
	  int ip2 = 2 * ipot + iseq;
	  int pot_state = getPot (ip2);	
	  if (abs(pot_state - last_pot_state[ip2]) < 10)
	    continue;
	  last_pot_state[ip2] = pot_state;
	  something_changed = true;
	  disPot (ipot, iseq, pot_state);
	}
    }
  return something_changed;
}

void disParams (int iseq)
{
  // Print parameters to serial monitor
  
  Serial.print ("Sequence ");
  Serial.println (iseq+1);
  Serial.print ("  Period ");
  Serial.print (period[iseq]);
  Serial.print ("  Triggers ");
  Serial.print (triggers[iseq]);
  if (triggers[iseq] != rtriggers[iseq])
    {
      Serial.print (" [");
      Serial.print (rtriggers[iseq]);
      Serial.print ("]");
    }
  Serial.print ("  Generator ");
  if (generator[iseq] == -1)
    Serial.print ("[Euclidean]");
  else if (generator[iseq] == -2)
    Serial.print ("[Random]");
  else
    Serial.print (generator[iseq]);
  Serial.print ("  Offset ");
  Serial.println (offset[iseq]);
  for (int i = 0; i < period[iseq]; ++i)
    Serial.print (getState (i, iseq) ? "." : "O");
  Serial.println();
}

void updateParams()
{
  // Get new values for parameters; display them if changed
  
  for (int iseq = 0; iseq < 2; ++iseq)
    {
      bool something_changed = false;

      new_per = potToPer (last_pot_state[per_pot[iseq]])
      if (new_per != period[iseq])
	{
	  period[iseq] = new_per;
	  something_changed = true;
	}

      new_trig = potToTrig (last_pot_state[trig_pot[iseq]])
      if (new_trig != rtriggers[iseq])
	{
	  rtriggers[iseq] = new_trig;
	  something_changed = true;
	}

      new_gen = potToGen (last_pot_state[gen_pot[iseq]])
      if (new_gen != generator[iseq])
	{
	  generator[iseq] = new_gen;
	  something_changed = true;
	}

      new_off = potToOff (last_pot_state[off_pot[iseq]])
      if (new_off != offset[iseq])
	{
	  offset[iseq] = new_off;
	  something_changed = true;
	}

      if (something_changed)
	{
	  triggers[iseq] = actTriggers(iseq);
	  counter[iseq] = -1;
	  setStates (iseq);
	  disParams (iseq);
	}
    }
}

bool getState(uint8_t index, int iseq)
{
  // Get state value with offset
  
  int step_idx = (index + offset[iseq]) % period[iseq];
  return states[iseq][step_idx];
}

void checkReset(int iseq)
{
  // See if we need to reset this sequence
  
  bool reset = analogRead(reset_out_pin[iseq]) > 500;
  if (reset == last_reset_state[iseq]) return;
  last_reset_state[iseq] = reset;
  if (reset) counter[iseq] = -1; // will be zero next clock pulse
}

void setStatesGap(int iseq)
{
  // Set states for a gap sequence

  int index = 0;
  states[iseq][index] = true;
    
  for (int var = 1; var < triggers[iseq]; ++var)
    {
      index = (index + generator[iseq]) % period[iseq];
      states[iseq][index] = true;
    }
}

void setStatesEuclidean(int iseq)
{
  // Set states for a Euclidean sequence
  // Algorithm source:
  // https://www.computermusicdesign.com/simplest-euclidean-rhythm-algorithm-explained/

  int bucket = 0;

  for (int var = 0; var < period[iseq]; ++var)
    {
      bucket += triggers[iseq];
      if (bucket >= period[iseq])
	{
	  bucket -= period[iseq];
	  states[iseq][var] = true;
	}
      else
	states[iseq][var] = false;
    }
}

void setStatesRandom(int iseq)
{
  // Set states for a random sequence

  // Make a list of all step numbers, pick one at random and add it to
  // state; move it to the end of the list and repeat with that
  // last entry excluded; et cetera

  int ldeck = period[iseq]
  int deck[ldeck];
  for (int id = 0; id < ldeck; ++id)
    deck[id] = id;
  
  for (int var = 0; var < triggers[iseq]; ++var)
    {
      int id = random(ldeck);
      states[iseq][deck[id]] = true;
      ldeck -= 1;
      int tmp = deck[ldeck];
      deck[ldeck] = deck[id];
      deck[id] = tmp;
    }
}

void setStates(int iseq)
{
  // See which kind of sequence we need and call the appropriate method
  
  for (int i = 0; i < period[iseq]; i++)
    states[iseq][i] = false;
  if (generator[iseq] == -1)
    setStatesEuclidean(iseq);
  else if (generator[iseq] == -2)
    setStatesRandom(iseq);
  else
    setStatesGap(iseq);				 
}

void onClockOn()
{
  // For each sequence, write the next state, and turn on period
  // output if counter is 0
  
  for (iseq = 0; iseq < 2; ++iseq)
    {
      counter[iseq] += 1;
      if (counter[iseq] >= period[iseq])
	{
	  counter[iseq] = 0;
	  digitalWrite(per_out_pin[iseq], HIGH);
	}

      digitalWrite(trig_out_pin[iseq], getState(counter[iseq], iseq));
    }
}

void onClockOff()
{
  // For each sequence, turn off state and period output
  
  for (iseq = 0; iseq < 2; ++iseq)
    {
      digitalWrite(trig_out_pin[iseq], LOW);
      digitalWrite(per_out_pin[iseq], LOW);
    }
}

void pciSetup(byte pin)
{
  // Set up clock interrupt
  
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}
 
ISR(PCINT0_vect)
{
  send_tick = true;
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(TRIG1_OUT_PIN, OUTPUT);
  pinMode(TRIG2_OUT_PIN, OUTPUT);
  pinMode(PER1_OUT_PIN, OUTPUT);
  pinMode(PER2_OUT_PIN, OUTPUT);
  pinMode(RESET1_IN_PIN, INPUT);
  pinMode(RESET2_IN_PIN, INPUT);
  pinMode(CLOCK_PIN, INPUT);

  pciSetup(CLOCK_PIN);
}

void loop()
{
  if (checkPots())
    {
      last_change_time = millis();
      change_pending = false;
    }
  else if (change_pending && millis - last_change_time > 250)
    update_params();
  
  checkReset();

  if (!send_tick)
    return;
  send_tick = false;

  clock_state = digitalRead(CLOCK_PIN);
  if (clock_state == last_clock_state)
    return;
  last_clock_state = clock_state;

  clock_state ? onClockOn() : onClockOff();
}