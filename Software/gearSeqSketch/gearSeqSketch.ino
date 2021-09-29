/* G.E.A.R. sequencer Arduino sketch

   Rich Holmes 2021 CC0 1.0

   Portions drawn from 
   https://github.com/katspaugh/arduino-modular-synth/blob/master/arduino-euclid/arduino-euclid.ino
   (No stated license)

 */

#define SEQ_OUT_PIN       4
#define PER_OUT_PIN       5
#define RESET_IN_PIN      3
#define CLOCK_PIN         2

#define ALG_POT  A2
#define CV1_POT  A6
#define CV2_POT  A3
#define CV3_POT  A1
#define CV4_POT  A0

int potlist[5] = {ALG_POT, CV1_POT, CV2_POT, CV3_POT, CV4_POT}; // alg first, then params pots

#define MAX_PERIOD          32

// Parameters for sequence: requested and actual
int parReq[4] = {0, 0, 0, 0};
int parAct[4] = {0, 0, 0, 0};
int parMin[4] = {0, 0, 0, 0};
int parMax[4] = {1, 1, 1, 1};  // maximum value + 1
String parNames[4] = {"", "", "", ""};

bool changed = false;  // algo or param 1–4 changed
bool changedNotOffset = false; // algo or param 1–3 changed 

int algo = 0;        
String algNames[5] = {"None", "Gap", "Euclidean", "ADC", "Random"};
bool states[MAX_PERIOD];          // List of on/off values
int counter = -1;                 // Current step #
int period = 0;
int offset = 0;

bool send_tick = false;
bool clock_state = false;
bool last_clock_state = false;
bool last_reset_state = false;

// Pots handling

int last_pot_state[5] = {9999, 9999, 9999, 9999, 9999};
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

int actTrigs ()
{
  // For gap sequence, actual #trigs may be less than requested
  // Return actual #trigs
  
  mxt = parAct[2] > 0 ? parAct[0] / gcf(parAct[0], parAct[2]) : parAct[0];
  return constrain (parAct[1], 0, mxt);
}

int potToParam(int potvalue, int parmin, int parmax)
{
  // Map pot value to parmin..parmax

  return int(float(potvalue)/(1024/(parmax-parmin)))+parmin;
}

void disPot (int ipot, int pot_state)
{
  // Print pot status to serial monitor

  if (ipot == 0)
    {
      Serial.print ("Alg pot = ");
      Serial.print (pot_state);
      Serial.print (" -> ");
      Serial.print (potToParam(pot_state, 0, 5));
    }
  else
    {
      Serial.print ("Pot ");
      Serial.print (ipot);
      Serial.print (" = ");
      Serial.print (pot_state);
      Serial.print (" -> ");
      Serial.print (parName[ipot]);
      Serial.print (potToParam(pot_state, parMin[ipot], parMax[ipot]));
    }
}  

bool checkPots()
{
  // Read pots and store new values if changed enough
  
  bool something_changed = false;
  for (int ipot = 0; ipot < 5; ++ipot)
    {
      int pot_state = analogRead (potlist[ipot]);
      if (abs(pot_state - last_pot_state[ipot]) < 10)
	continue;
      last_pot_state[ipot] = pot_state;
      something_changed = true;
      disPot (ipot, pot_state);
    }
  return something_changed;
}

void disParams ()
{
  // Print parameters and sequence to serial monitor

  for (ip = 0; ip < 4; ++ip)
    {
      if (parNames[ip] == "")
	continue;
      Serial.print (parNames[ip]);
      Serail.print (" ");
      Serial.print (parAct[ip]);
      if (parAct[ip] != parReq[ip])
	{
	  Serial.print (" [");
	  Serial.print (parReq[ip]);
	  Serial.print ("]");
	}
      Serial.print (" ");
    }
  Serial.println();
  for (int i = 0; i < param1; ++i)
    Serial.print (getState (i) ? "." : "O");
  Serial.println();
}

void updateParams()
{
  // Get new values for parameters; display them if changed
  
  // First check for new algorithm
  
  new_algo = potToParam(last_pot_state[0], 0, 5);
  if (new_algo != algo)
    {
      algo = new_algo;
      changed = true;
      changedNotOffset = true;
      
      if (algo == 0)
	{
	  for (ipar = 0; ipar < 4; ++ipar)
	    {
	      parNames[ipar] = "";
	      parMin[ipar] = 0;
	      parMax[ipar] = 1;
	    }
	}
      else if (algo == 1)
	{
	  parNames[0] = "Per";
	  parMin[0] = 2;
	  parMax[0] = MAX_PERIOD+1;
	  parNames[1] = "Trig";
	  parMin[1] = 0;
	  parMax[1] = MAX_PERIOD+1;
	  parNames[2] = "Gen";
	  parMin[2] = 1;
	  parMax[2] = MAX_PERIOD / 2 + 1;
	  parNames[3] = "Off";
	  parMin[3] = 0;
	  parMax[3] = MAX_PERIOD;
	}
      else if (algo == 2)
	{
	  parNames[0] = "Per";
	  parMin[0] = 2;
	  parMax[0] = MAX_PERIOD+1;
	  parNames[1] = "Trig";
	  parMin[1] = 0;
	  parMax[1] = MAX_PERIOD+1;
	  parNames[2] = "";
	  parMin[2] = 1;
	  parMax[2] = 0;
	  parNames[3] = "Off";
	  parMin[3] = 0;
	  parMax[3] = MAX_PERIOD;
	}
      else if (algo == 3)
	{
	  parNames[0] = "Per";
	  parMin[0] = 2;
	  parMax[0] = MAX_PERIOD+1;
	  parNames[1] = "Val";
	  parMin[1] = 0;
	  parMax[1] = 1024;
	  parNames[2] = "D/G";
	  parMin[2] = 0;
	  parMax[2] = 2;
	  parNames[3] = "Off";
	  parMin[3] = 0;
	  parMax[3] = MAX_PERIOD;
	}
      else if (algo == 4)
	{
	  parNames[0] = "Per";
	  parMin[0] = 2;
	  parMax[0] = MAX_PERIOD+1;
	  parNames[1] = "Trig";
	  parMin[1] = 0;
	  parMax[1] = MAX_PERIOD+1;
	  parNames[2] = "";
	  parMin[2] = 0;
	  parMax[2] = 32;
	  parNames[3] = "Off";
	  parMin[3] = 0;
	  parMax[3] = MAX_PERIOD;
	}
      
    }

  // Now check for new parameters and record requests
  
  for (ip = 1; ip < 5; ++ip)
    {
      int new_par = potToParam(last_pot_state[ip],
			       parMin[ip-1],
			       parMax[ip-1]);
      if (new_par != parReq[ip-1])
      {
	parReq[ip-1] = new_par;
	changed = true;
	if (ip != 4)
	  changedNotOffset = true;
      }
    }
}


void setupNextPeriod()
{
  // Set things for upcoming period

  if (changed)
    {
      for (ip = 0; ip < 4; ++ip)
	{
	  parAct[ip] = parReq[ip];
	  // For Gap, actual trig may be lower
	  if (algo == 1 && ip == 1)
	    parAct[ip] = actTrigs();
	}
    }  
  
  // New states if change other than offset
  if (changedNotOffset)
    {
      period = parAct[0];
      offset = parAct[3];
      setStates ();
    }
  disParams ();

  changed = false;  // algo or param 1–4 changed
  changedNotOffset = false; // algo or param 1–3 changed 
}

bool getState(uint8_t index)
{
  // Get state value with offset
  
  int step_idx = (index + offset) % period;
  return states[step_idx];
}

void checkReset()
{
  // See if we need to reset this sequence
  
  bool reset = analogRead(RESET_IN_PIN) > 500;
  if (reset == last_reset_state) return;
  last_reset_state = reset;
  if (reset) counter = -1; // will be zero next clock pulse
}

void setStatesGap()
{
  // Set states for a gap sequence

  int index = 0;
  states[index] = true;
    
  for (int var = 1; var < parAct[1]; ++var)
    {
      index = (index + parAct[2]) % parAct[0];
      states[index] = true;
    }
}

void setStatesEuclidean()
{
  // Set states for a Euclidean sequence
  // Algorithm source:
  // https://www.computermusicdesign.com/simplest-euclidean-rhythm-algorithm-explained/

  int bucket = 0;

  for (int var = 0; var < parAct[0]; ++var)
    {
      bucket += parAct[1];
      if (bucket >= parAct[0])
	{
	  bucket -= parAct[0];
	  states[var] = true;
	}
      else
	states[var] = false;
    }
}

void setStatesADC()
{
  // Set states for an ADC sequence
  int bit = 1 << 9;
  for (int var = 0; var < parAct[0]; ++var)
    {
      states[var] = (bit & parAct[1] != 0);
      bit >>= 1;
    }
  // Gray code if requested
  if (parAct[2] > 0)
    for (int var = 0; var < parAct[0]-1; ++var)
      states[var] = states[var]^states[var+1];
}

void setStatesRandom()
{
  // Set states for a random sequence

  // Make a list of all step numbers, pick one at random and add it to
  // state; move it to the end of the list and repeat with that
  // last entry excluded; et cetera

  int ldeck = parAct[0]
  int deck[ldeck];
  for (int id = 0; id < ldeck; ++id)
    deck[id] = id;
  
  for (int var = 0; var < parAct[1]; ++var)
    {
      int id = random(ldeck);
      states[deck[id]] = true;
      ldeck -= 1;
      int tmp = deck[ldeck];
      deck[ldeck] = deck[id];
      deck[id] = tmp;
    }
}

void setStates()
{
  // See which kind of sequence we need and call the appropriate method
  
  for (int i = 0; i < period; i++)
    states[i] = false;
  if (algo == 1)
    setStatesGap();				 
  else if (algo == 2)
    setStatesEuclidean();
  else if (algo == 3)
    setStatesADC();
  else
    setStatesRandom();
}

void onClockOn()
{
  // Write the next state, and turn on period output if counter is 0
  
  counter += 1;
  if (counter >= param1)
    {
      counter = 0;
      digitalWrite(PER_OUT_PIN, HIGH);
    }

  digitalWrite(TRIG_OUT_PIN, getState(counter));
}

void onClockOff()
{
  // Turn off state and param1 output
  
  digitalWrite(TRIG_OUT_PIN, LOW);
  digitalWrite(PER_OUT_PIN, LOW);
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
  
  pinMode(TRIG_OUT_PIN, OUTPUT);
  pinMode(PER_OUT_PIN, OUTPUT);
  pinMode(RESET_IN_PIN, INPUT);
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
  if (counter == -1 || counter == period-1)
    setupNextPeriod();
      
  if (!send_tick)
    return;
  send_tick = false;

  clock_state = digitalRead(CLOCK_PIN);
  if (clock_state == last_clock_state)
    return;
  last_clock_state = clock_state;

  clock_state ? onClockOn() : onClockOff();
}