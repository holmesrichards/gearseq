/* G.E.A.R. sequencer Arduino sketch

   Rich Holmes 2021 CC0 1.0

   Portions drawn from 
   https://github.com/katspaugh/arduino-modular-synth/blob/master/arduino-euclid/arduino-euclid.ino
   (No stated license)

 */

#include <assert.h>

enum Alg { NoneAlg, Gap, Euclid, ADeeCee, Rand };
#define MAX_PERIOD          32

int nalgo = 0;
#define USEGAP 1
#define USEEUC 1
#define USEADC 1
#define USERAN 1

#define OLED 0 // nonzero to use OLED, else serial monitor

typedef unsigned short int param;
typedef char stateindex;

#define CLOCK_PIN         2
#define CLOCK_LED_PIN     3
#define RESET_LED_PIN     4
#define RESET_IN_PIN      5
#define SEQ_OUT_PIN       6
#define PER_OUT_PIN       7

#define ALG_POT  A2
#define CV1_POT  A6
#define CV2_POT  A3
#define CV3_POT  A1
#define CV4_POT  A0

char potlist[5] = {ALG_POT, CV1_POT, CV2_POT, CV3_POT, CV4_POT}; // alg first, then params pots

int counter = -1;                 // Current step #

volatile bool sendTick = false;
bool clockState = false;
bool lastClockState = false;
bool lastResetState = false;

// Pots handling

param lastPotState[5] = {9999, 9999, 9999, 9999, 9999};
int lastChangeTime = 0;
bool changePending = false;

Alg alg;
String name;
String parName[4];

// parReq is requested values for next parameters
// These may be modified by constraints, and do not go into use until
// start of next period.
// parAct is actual parameter values in use.

param parReq[4];    // requested values
param parMin[4];
param parMax[4];
param parAct[4]; // actual values
bool states[MAX_PERIOD];
bool changed; // if any par different than last time
bool changedNotOffset; // if any of pars 0-3 different

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


class Algo
{
 public:
  Algo(Alg alg = NoneAlg) {Setup();}
  virtual void Setup();
  void DisPot (int ipot, int potState);
  void DisParams();
  void UpdatePars();
  virtual param paramConstrain (int i, param p0, param p1, param p2, param p3);
  virtual void SetParAct();
  void SetupNextPeriod();
  virtual void SetStates() {assert (parAct[0] <= MAX_PERIOD); for (stateindex i = 0; i < parAct[0]; ++i) states[i] = false;}
  bool GetState(stateindex i) const;
  param PotToParam(int i) const;
  param Period() const {return parAct[0];}
  param Offset() const {return parAct[3];}
};

#if USEGAP
class GapAlgo: public Algo
{
 public:
  GapAlgo() {Setup();}
  void Setup();
  void SetStates();
  virtual param paramConstrain (int i, param p0, param p1, param p2, param p3);
  void SetParAct();
};
#endif

#if USEEUC
class EuclidAlgo: public Algo
{
 public:
  EuclidAlgo() {Setup();}
  void Setup();
  void SetStates();
};
#endif

#if USEADC
class ADCAlgo: public Algo
{
 public:
  ADCAlgo() {Setup();}
  void Setup();
  void SetStates();
  virtual param paramConstrain (int i, param p0, param p1, param p2, param p3);
};
#endif

#if USERAN
class RandAlgo: public Algo
{
 public:
  RandAlgo() {Setup();}
  void Setup();
  void SetStates();
};
#endif

Algo* algoList[5];
unsigned short int curAlgo = 0;




void Algo::Setup()
{
  Serial.println ("Algo::Setup");
  alg = NoneAlg;
  name = "None";
  parName[0] = "Period";
  parName[1] = "Triggers";
  parName[2] = "";
  parName[3] = "Offset";
  parReq[0] = 9999;
  parReq[1] = 9999;
  parReq[2] = 9999;
  parReq[3] = 9999;
  parMin[0] = 2;
  parMax[0] = MAX_PERIOD;
  parMin[1] = 0;
  parMax[1] = MAX_PERIOD;
  parMin[2] = 0;
  parMax[2] = 31;
  parMin[3] = 0;
  parMax[3] = MAX_PERIOD-1;
  changed = false;  // algo or param 1–4 changed
  changedNotOffset = false; // algo or param 1–3 changed
}

void Algo::DisPot (int ipot, int potState)
{
#if OLED
  return;
#else
  // Print pot status to serial monitor

  if (ipot == 0)
    {
      Serial.print ("Alg pot = ");
      Serial.print (potState);
      Serial.print (" -> ");
      Serial.println (PotToParam(-1));
    }
  else
    {
      Serial.print ("Pot ");
      Serial.print (ipot);
      Serial.print (" = ");
      Serial.print (potState);
      Serial.print (" -> ");
      Serial.print (parName[ipot-1]);
      Serial.print (" = ");
      Serial.println (PotToParam(ipot-1));
    }
#endif
}

void Algo::DisParams ()
{
#if OLED
  return;
#else
  // Print parameters and sequence to serial monitor

  for (int ip = 0; ip < 4; ++ip)
    {
      if (parName[ip] == "")
      	continue;
      Serial.print (parName[ip]);
      Serial.print (" ");
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
  for (int i = 0; i < Period(); ++i)
    Serial.print (GetState (i) ? "O" : ".");
  Serial.println();
#endif
}

void Algo::UpdatePars()
{
  // Get new values for requested parameters

  for (int ip = 0; ip < 4; ++ip)
    {
      param newPar = PotToParam(ip);
      if (newPar != parReq[ip])
      {
	parReq[ip] = newPar;
	changed = true;
	if (ip != 4)
	  changedNotOffset = true;
      }
    }
}

param Algo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  switch (i)
    {
    case 0:
      return constrain (p0, parMin[0], parMax[0]);
      break;
    case 1:
      return constrain (p1, parMin[1], p0);
      break;
    case 2:
      return constrain (p2, parMin[2], parMax[2]);
      break;
    case 3:
      return constrain (p3, parMin[3], p0-1);
      break;
    }
}

void Algo::SetParAct()
{
  parAct[0] = paramConstrain (0, parReq[0], parReq[1], parReq[2], parReq[3]);
  // Other params may be constrained by constrained period
  parAct[1] = paramConstrain (1, parAct[0], parReq[1], parReq[2], parReq[3]);
  parAct[2] = paramConstrain (2, parAct[0], parReq[1], parReq[2], parReq[3]);
  parAct[3] = paramConstrain (3, parAct[0], parReq[1], parReq[2], parReq[3]);
}

void Algo::SetupNextPeriod()
{
  // Set things for upcoming period
  if (changed)
    {
      SetParAct();
      // New states if change other than offset
      if (changedNotOffset)
      	SetStates ();
      DisParams();
    }
  
  changed = false;  // algo or param 1–4 changed
  changedNotOffset = false; // algo or param 1–3 changed
}

bool Algo::GetState(stateindex index) const
{
  // Get state value with offset

  assert (parAct[0] <= MAX_PERIOD);
  stateindex stepIdx = (index + (parAct[0]-parAct[3])) % parAct[0];
  return states[stepIdx];
}

param Algo::PotToParam(int i) const
{
  // Map pot value to parMin..parMax
  // i == -1 for algorithm pot, 0-3 for parameter pots

  if (i == -1) // algorithm pot
    return param(float(lastPotState[0])/(1024./(nalgo+1))); // 0 to nalgo
  else
    return param(float(lastPotState[i+1])/(1024./(parMax[i]-parMin[i]+1))+parMin[i]);
}

#if USEGAP
void GapAlgo::Setup()
{
  Serial.println ("GapAlgo::Setup");
  Algo::Setup();
  alg = Gap;
  name = "Gap";
  parName[2] = "Generator";
  parMax[2] = MAX_PERIOD/2;
}

param GapAlgo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  if (i == 1)
    return constrain (p1, parMin[1], p0+p2 > 0 ? p0/gcd(p0,p2) : p0);
  else
    return Algo::paramConstrain (i, p0, p1, p2, p3);
}

void GapAlgo::SetParAct()
{
  parAct[0] = paramConstrain (0, parReq[0], parReq[1], parReq[2], parReq[3]);
  // Other params may be constrained by constrained period
  parAct[2] = paramConstrain (2, parAct[0], parReq[1], parReq[2], parReq[3]);
  // Triggers may be further constrained by constrained generator
  parAct[1] = paramConstrain (1, parAct[0], parReq[1], parAct[2], parReq[3]);
  parAct[3] = paramConstrain (3, parAct[0], parReq[1], parReq[2], parReq[3]);
}

void GapAlgo::SetStates()
{
  assert (parAct[0] <= MAX_PERIOD);
  for (stateindex i = 0; i < parAct[0]; ++i)
    states[i] = false;

  stateindex index = 0;
  states[index] = true;
    
  for (int var = 1; var < parAct[1]; ++var)
    {
      index = (index + parAct[2]) % parAct[0];
      states[index] = true;
    }
}
#endif

#if USEEUC
void EuclidAlgo::Setup()
{
  Algo::Setup();
  alg = Euclid;
  name = "Euclid";
}

void EuclidAlgo::SetStates()
{
  // Algorithm source:
  // https://www.computermusicdesign.com/simplest-euclidean-rhythm-algorithm-explained/

  assert (parAct[0] <= MAX_PERIOD);
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

#endif

#if USEADC
void ADCAlgo::Setup()
{
  Algo::Setup();
  alg = ADeeCee;
  name = "ADC";
  parName[1] = "Value";
  parName[2] = "";
  parMin[0] = 2;
  parMax[0] = 10;
  parMin[1] = 0;
  parMax[1] = 1023;
  parMin[2] = 0;
  parMax[2] = 31;
  parMin[3] = 0;
  parMax[3] = 9;
}

param ADCAlgo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  if (i == 1)
    return constrain (p1, parMin[1], parMax[1]);
  else
    return Algo::paramConstrain (i, p0, p1, p2, p3);
}

void ADCAlgo::SetStates()
{
  assert (parAct[0] <= MAX_PERIOD);
  int bit = 1 << parAct[0];
  for (stateindex var = 0; var < parAct[0]; ++var)
    {
      states[var] = (bit & parAct[1]) != 0;
      bit >>= 1;
    }
}

#endif

#if USERAN
void RandAlgo::Setup()
{
  Algo::Setup();
  alg = Rand;
  name = "Rand";
  parName[2] = "Throw";
}

void RandAlgo::SetStates()
{
  // Set states for a random sequence

  // Make a list of all step numbers, pick one at random and add it to
  // state; move it to the end of the list and repeat with that
  // last entry excluded; et cetera

  assert (parAct[0] <= MAX_PERIOD);
  for (stateindex i = 0; i < parAct[0]; ++i)
    states[i] = false;

  int ldeck = parAct[0];
  assert (ldeck < 64);
  
  stateindex deck[ldeck];
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
#endif

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
      algoList[curAlgo]->DisPot (ipot, potState);
    }
  return somethingChanged;
}


void checkReset()
{
  // See if we need to reset this sequence
  
  bool reset = !digitalRead(RESET_IN_PIN); // inverted input
  if (reset == lastResetState) return;
  lastResetState = reset;
  if (reset)
    counter = -1; // will be zero next clock pulse
  digitalWrite(RESET_LED_PIN, reset);
}

void onClockOn()
{
  // Write the next state, and turn on period output if counter is 0
  
  counter += 1;
  if (counter >= algoList[curAlgo]->Period())
    {
      counter = 0;
      digitalWrite(PER_OUT_PIN, HIGH);
    }

  digitalWrite(SEQ_OUT_PIN, algoList[curAlgo]->GetState(counter));
}

void onClockOff()
{
  // Turn off state and param1 output
  
  digitalWrite(SEQ_OUT_PIN, LOW);
  digitalWrite(PER_OUT_PIN, LOW);
}

void tick() // ISR function
{
  sendTick = true;
}


void setup()
{
#if !OLED
  Serial.begin(9600);
  Serial.println("setup ");
#endif
  
  pinMode(CLOCK_LED_PIN, OUTPUT);
  pinMode(RESET_LED_PIN, OUTPUT);
  pinMode(SEQ_OUT_PIN, OUTPUT);
  pinMode(PER_OUT_PIN, OUTPUT);
  pinMode(RESET_IN_PIN, INPUT);
  pinMode(CLOCK_PIN, INPUT);

  attachInterrupt (digitalPinToInterrupt(CLOCK_PIN), tick, CHANGE);

  nalgo = 0;
  algoList[nalgo++] = new Algo();
#if USEGAP
  algoList[nalgo++] = new GapAlgo();
#endif
#if USEEUC
  algoList[nalgo++] = new EuclidAlgo();
#endif
#if USEADC
  algoList[nalgo++] = new ADCAlgo();
#endif
#if USERAN
  algoList[nalgo++] = new RandAlgo();
#endif
  lastPotState[0] = analogRead (potlist[0]);
  curAlgo = constrain (algoList[0]->PotToParam(-1), 0, nalgo-1);
  Serial.print("algo "); Serial.print(algoList[0]->PotToParam(-1)); Serial.print ("->"); Serial.println(curAlgo);
  algoList[curAlgo]->Setup();
  Serial.println("setup2 ");
  checkPots();
  Serial.println("setup3 ");
  algoList[curAlgo]->UpdatePars();
  algoList[curAlgo]->SetupNextPeriod();
  changePending = false;
}

void loop()
{
  if (checkPots())
    {
      lastChangeTime = millis();
      changePending = true;
    }
  else if (changePending && millis - lastChangeTime > 250)
    {
      // First check for new algorithm
      unsigned short int newAlgo = constrain (algoList[curAlgo]->PotToParam(-1), 0, nalgo-1);
      if (newAlgo != curAlgo)
	{
	  curAlgo = newAlgo;
	  algoList[curAlgo]->Setup();
	}
      algoList[curAlgo]->UpdatePars();
      changePending = false;
    }
  
  checkReset();
  if (counter == algoList[curAlgo]->Period()-1)
    algoList[curAlgo]->SetupNextPeriod();

  if (!sendTick)
    {
      return;
    }
  sendTick = false;
  
  clockState = !digitalRead(CLOCK_PIN); // inverted input
  if (clockState == lastClockState)
    return;
  lastClockState = clockState;
  digitalWrite(CLOCK_LED_PIN, clockState);

  clockState ? onClockOn() : onClockOff();
}