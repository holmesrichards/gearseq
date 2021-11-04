/* G.E.A.R. sequencer Arduino sketch

   Rich Holmes 2021 CC0 1.0

   Portions drawn from 
   https://github.com/katspaugh/arduino-modular-synth/blob/master/arduino-euclid/arduino-euclid.ino
   (No stated license)

 */

enum Alg { NoneAlg, Gap, Euclid, ADC, Rand };
String algNames[5] = {"None", "Gap", "Euclidean", "ADC", "Random"};
#define MAX_PERIOD          32

typedef unsigned short int param;
typedef char stateindex;

class Algo
{
 public:
  Algo(enum alg) {Setup(alg);}
  void Setup(enum alg);
  void DisPot (int ipot, int potState);
  void DisParams();
  void UpdatePars();
  param ActPar(const int i) const {return par[i];} // value parAct will be set to
  void SetupNextPeriod();
  void SetStates() {return;}
  bool GetState(stateindex i) const;
  param PotToParam(int i) const;
  
  Alg alg;
  String name;
  String parName[4];
  param parMin[4];
  param parMax[4];
  param par[4];    // requested values
  param parAct[4]; // actual values
  bool states[MAX_PERIOD];
  bool changed; // if any par different than last time
  bool changeNotOffset; // if any of pars 0-3 different
};

class GapAlgo: public Algo
{
  GapAlgo()
    {
      Algo::Setup(Gap);
      parName[2] = "Generator";
      parMin[2] = 1;
      parMax[2] = MAX_PERIOD/2;
      par[2] = 1;
    }
};

class EuclidAlgo: public Algo
{
  EuclidAlgo()
    {
      Algo::Setup(Euclid);
    }
};

class ADCAlgo: public Algo
{
  ADCAlgo()
    {
      Algo::Setup(ADC);
      parMax[0] = 10;
      parName[2] = "Gray";
      parMax[2] = 1;
    }
};

class RandAlgo: public Algo
{
  RandAlgo()
    {
      Algo::Setup(Rand);
      parName[2] = "Throw";
    }
};

Algo noneAlgo(NoneAlg);
GapAlgo gapAlgo;
EuclidAlgo euclidAlgo;
ADCAlgo adcAlgo;
RandAlgo randAlgo;
Algo algoList[5] = {noneAlgo, gapAlgo, euclidAlgo, adcAlgo, randAlgo};
unsigned short int curAlgo = 0;

void Algo::Setup(enum alg)
{
  alg = alg;
  name = algNames[alg];
  parName[0] = "Period";
  parName[1] = "Triggers";
  parName[2] = "";
  parName[3] = "Offset";
  parMin[0] = 2;
  parMax[0] = MAX_PERIOD;
  parMin[1] = 0;
  parMax[1] = MAX_PERIOD;
  parMin[2] = 0;
  parMax[2] = 31;
  parMin[3] = 0;
  parMax[3] = MAX_PERIOD-1;
  par[0] = 2;
  par[1] = 0;
  par[2] = 0;
  par[3] = 0;
  for (unsigned i = 0; i < 4; ++i)
    parAct[i] = par[i];
  for (stateindex i = 0; i < MAX_PERIOD; ++i)
    states[i] = false;
  changed = true;
  changedNotOffset = true;
}

void Algo:DisPot (int ipot, int potState)
{
  // Print pot status to serial monitor

  if (ipot == 0)
    {
      Serial.print ("Alg pot = ");
      Serial.print (potState);
      Serial.print (" -> ");
      Serial.print (PotToParam(-1));
    }
  else
    {
      Serial.print ("Pot ");
      Serial.print (ipot);
      Serial.print (" = ");
      Serial.print (potState);
      Serial.print (" -> ");
      Serial.print (parName[ipot-1]);
      Serial.print (PotToParam(ipot-1));
    }
}  

void Algo::DisParams ()
{
  // Print parameters and sequence to serial monitor

  for (ip = 0; ip < 4; ++ip)
    {
      if (parNames[ip] == "")
	continue;
      Serial.print (parName[ip]);
      Serail.print (" ");
      Serial.print (parAct[ip]);
      if (parAct[ip] != par[ip])
	{
	  Serial.print (" [");
	  Serial.print (par[ip]);
	  Serial.print ("]");
	}
      Serial.print (" ");
    }
  Serial.println();
  for (int i = 0; i < par[0]; ++i)
    Serial.print (GetState (i) ? "." : "O");
  Serial.println();
}

void Algo::UpdatePars()
{
  // Get new values for parameters; display them if changed

  for (ip = 1; ip < 5; ++ip)
    {
      param newPar = PotToParam(ip);
      if (newPar != par[ip-1])
      {
	par[ip-1] = newPar;
	changed = true;
	if (ip != 4)
	  changedNotOffset = true;
      }
    }
}

void Algo::SetupNextPeriod()
{
  // Set things for upcoming period

  if (changed)
    {
      for (ip = 0; ip < 4; ++ip)
	parAct[ip] = ActPar(ip);
      DisParams();
    }
  
  // New states if change other than offset
  if (changedNotOffset)
    setStates ();

  changed = false;  // algo or param 1–4 changed
  changedNotOffset = false; // algo or param 1–3 changed 
}

bool Algo:GetState(stateindex index) const
{
  // Get state value with offset
  
  stateindex stepIdx = (index + parAct[3]) % parAct[0];
  return algoList[curAlgo].states[stepIdx];
}

param Algo::PotToParam(int i) const
{
  // Map pot value to parMin..parMax
  // i == -1 for algorithm pot, 0-3 for parameter pots

  if (i == -1) // algorithm pot
    return param(float(lastPotState[0])/(1024/5)); // 0 to 4
  else
    return param(float(lastPotState[i+1])/(1024/(parMax[i]-parMin[i]+1))+parMin[i]);
}

void GapAlgo::setStates()
{
  stateindex index = 0;
  states[index] = true;
    
  for (int var = 1; var < parAct[1]; ++var)
    {
      index = (index + parAct[2]) % parAct[0];
      states[index] = true;
    }
}

param GapAlgo::ActPar(const int i) const
{
  // Actual maximum number of trigs may be a fraction of period
  if (i == 1)
    {
      mxt = par[2] > 0 ? par[0] / gcf(par[0], par[2]) : par[0];
      return constrain (par[1], 0, mxt);
    }
  else
    return par[i];
}
      

void EuclidAlgo::SetStates()
{
  // Algorithm source:
  // https://www.computermusicdesign.com/simplest-euclidean-rhythm-algorithm-explained/

  int bucket = 0;

  for (stateindex var = 0; var < parAct[0]; ++var)
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

void ADCAlgo::SetStates()
{
  int bit = 1 << 9;
  for (stateindex var = 0; var < parAct[0]; ++var)
    {
      states[var] = (bit & parAct[1] != 0);
      bit >>= 1;
    }
  // Gray code if requested
  if (parAct[2] > 0)
    for (int var = 0; var < parAct[0]-1; ++var)
      states[var] = states[var]^states[var+1];
}

void RandAlgo::SetStates()
{
  // Set states for a random sequence

  // Make a list of all step numbers, pick one at random and add it to
  // state; move it to the end of the list and repeat with that
  // last entry excluded; et cetera

  int ldeck = parAct[0]
  stateindex deck[ldeck];
  for (stateindex id = 0; id < ldeck; ++id)
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

int counter = -1;                 // Current step #

bool sendTick = false;
bool clockState = false;
bool lastClockState = false;
bool lastResetState = false;

// Pots handling

int lastPotState[5] = {9999, 9999, 9999, 9999, 9999};
int lastChangeTime = 0;
bool changePending = false;

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


bool checkPots()
{
  // Read pots and store new values if changed enough
  
  bool somethingChanged = false;
  for (int ipot = 0; ipot < 5; ++ipot)
    {
      int potState = analogRead (potlist[ipot]);
      if (abs(potState - lastPotState[ipot]) < 10)
	continue;
      lastPotState[ipot] = potState;
      somethingChanged = true;
      disPot (ipot, potState);
    }
  return somethingChanged;
}


void checkReset()
{
  // See if we need to reset this sequence
  
  bool reset = analogRead(RESET_IN_PIN) > 500;
  if (reset == lastResetState) return;
  lastResetState = reset;
  if (reset) counter = -1; // will be zero next clock pulse
}

void onClockOn()
{
  // Write the next state, and turn on period output if counter is 0
  
  counter += 1;
  if (counter >= algoList[curAlgo].par[0])
    {
      counter = 0;
      digitalWrite(PER_OUT_PIN, HIGH);
    }

  digitalWrite(TRIG_OUT_PIN, algoList[curAlgo].GetState(counter));
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
  sendTick = true;
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
      lastChangeTime = millis();
      changePending = false;
    }
  else if (changePending && millis - lastChangeTime > 250)
    {
      // First check for new algorithm
      newAlgo = algoList[curAlgo].potToParam(-1);
      if (newAlgo != algo)
	curAlgo = newAlgo;
      algoList[curAlgo].UpdateParams();
    }
  
  checkReset();
  if (counter == -1 || counter == period-1)
    algoList[curAlgo].SetupNextPeriod();
      
  if (!sendTick)
    return;
  sendTick = false;

  clockState = digitalRead(CLOCK_PIN);
  if (clockState == lastClockState)
    return;
  lastClockState = clockState;

  clockState ? onClockOn() : onClockOff();
}