/* G.E.A.R. sequencer Arduino sketch

   Rich Holmes 2021 CC0 1.0

   Portions drawn from 
   https://github.com/katspaugh/arduino-modular-synth/blob/master/arduino-euclid/arduino-euclid.ino
   (No stated license)

 */

#define OLED 1 // nonzero to use OLED
#define SERIAL 1 // nonzero to use serial monitor

#include <assert.h>
#if OLED
#include <U8g2lib.h>
#endif

#define MAX_PERIOD          32 // can be reduced; increasing would require significant changes

#define NALGO 4

#if OLED
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;
#endif

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
bool changePending = false; // need to change pending parameters
bool changeParAct = false; // need to change actual parameters or algorithm
bool doDisplay = false; // update the display when you get a chance

char* blank = "                ";

// parReq is requested values for next parameters.
// parPend is requested values modified by constraints, to go into use at
// start of next period.
// parAct is actual parameter values in use.

param parReq[4] = {9999, 9999, 9999, 9999};    // requested values
param parPend[4] = {9999, 9999, 9999, 9999};    // requested values
param parAct[4]; // actual values
long statesPend; // bits for the pattern
long statesAct;

// current algorithm in use and requested next algorithm
unsigned short int curAlgo = 0;
unsigned short int reqAlgo = 0;

int gcd (int a,int b);
void checkReset();
void SetupThisPeriod();
void onClockOn();
void onClockOff();
void tick();
void setup();
void loop();


//============================================================

class Algo
{
 public:
  Algo() {Setup();}
  virtual void Setup();
  bool checkPots();
  param PotToParam(int i, int v=-1);
  virtual param paramConstrain (int i, param p0, param p1, param p2, param p3);
  void DisParams();
  void UpdatePars();
  virtual void SetParPend();
  virtual void SetStates() {statesPend = 0;}
  bool GetState(stateindex i);
  virtual param Period() const {return parAct[0];}
  virtual param PeriodPend() const {return parPend[0];}
  virtual param Offset() const {return parAct[3];}
  virtual param OffsetPend() const {return parPend[3];}
  String algName;
  String parName[4];
  param parMin[4];
  param parMax[4];
  short unsigned int delta[5]; // pot range corresponding to change
  // of algo or parameter
};

//============================================================

class GapAlgo: public Algo
{
 public:
  GapAlgo() {Setup();}
  void Setup();
  void SetStates();
  param paramConstrain (int i, param p0, param p1, param p2, param p3);
  void SetParPend();
};

//============================================================

class EuclidAlgo: public Algo
{
 public:
  EuclidAlgo() {Setup();}
  void Setup();
  void SetStates();
};

//============================================================

class ADCAlgo: public Algo
{
 public:
  ADCAlgo() {Setup();}
  void Setup();
  void SetStates();
  param paramConstrain (int i, param p0, param p1, param p2, param p3);
};

//============================================================

class RandAlgo: public Algo
{
 public:
  RandAlgo() {Setup();}
  void Setup();
  void SetStates();
};

//============================================================

Algo* algoList[5];

//============================================================

void Algo::Setup()
{
  algName = "None"; 
  parName[0] = "Per  "; // 5 chars
  parName[1] = "-N/A-";
  parName[2] = "-N/A-";
  parName[3] = "-N/A-";
  parMin[0] = 2;
  parMax[0] = MAX_PERIOD;
  parMin[1] = 0;
  parMax[1] = MAX_PERIOD;
  parMin[2] = 0;
  parMax[2] = 31;
  parMin[3] = 0;
  parMax[3] = MAX_PERIOD-1;
  delta[0] = int(1024./(NALGO+1));
  for (int i = 0; i < 4; ++i)
    delta[i+1] = int(1024./(parMax[i]-parMin[i]+1));
}

bool Algo::checkPots()
{
  // Read pots and store new values if changed enough
  
  bool somethingChanged = false;
  for (int ipot = 0; ipot < 5; ++ipot)
    {
      int potState = analogRead (potlist[ipot]);
      if (abs(int(potState)-int(lastPotState[ipot])) < delta[ipot])
	continue;
      lastPotState[ipot] = potState;
      somethingChanged = true;
    }
  return somethingChanged;
}


param Algo::PotToParam(int i, int v)
{
  // Map pot value to parMin..parMax
  // i == -1 for algorithm pot, 0-3 for parameter pots
  // If v < 0 use lastPotState[i+i] else use v for pot value

  if (v < 0)
    v = lastPotState[i+1];
  if (i == -1) // algorithm pot
    return param(float(v)/(1024./(NALGO+1))); // 0 to NALGO
  else
    return param(float(v)/(1024./(parMax[i]-parMin[i]+1))+parMin[i]);
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

void Algo::DisParams ()
{
#if OLED
  String str;
  char line[17];

  String top = algName + (algoList[reqAlgo]->algName == algName ? "" :
			  String ("[")+
			  algoList[reqAlgo]->algName.substring(0,3)+String("]"));
  int c = (16-top.length())/2;
  top.toCharArray(line, 17);
  u8x8.drawString (0, 0, blank);
  u8x8.drawString (c, 0, line);
  char l = 1;
  for (int ip = 0; ip < 4; ++ip)
    {
      u8x8.drawString (0, l, blank);
      parName[ip].toCharArray(line, 6);
      u8x8.drawString (0, l, line);
      if (parName[ip] != "-N/A-")
	{
	  str = String(parAct[ip]);
	  while (str.length() < 4)
	    str = String(" ")+str;
	  str.toCharArray(line, 5);
	  u8x8.drawString (6, l, line);
	  
	  if (parAct[ip] != parReq[ip])
	    {
	      str = String(parReq[ip]);
	      while (str.length() < 4)
		str = String(" ")+str;
	      str = String("[") + str + String("]");
	      str.toCharArray(line, 7);
	      u8x8.drawString (10, l, line);
	    }
	}
      ++l;
    }

  while (l < 8)
    u8x8.drawString (0, l++, blank);

  l = Period() < 17 ? 7 : 6;
  c = 0;
  for (int i = 0; i < Period(); ++i)
    {
      u8x8.drawString (c++, l, GetState (i) ? "O" : "-");
      if (c == 16)
	{
	  c = 0;
	  l++;
	}
    }
#endif
#if SERIAL
  // Print parameters and sequence to serial monitor

  Serial.print (algName);
  if (algoList[reqAlgo]->algName != algName)
    Serial.print (String ("[")+algoList[reqAlgo]->algName.substring(0,3)+String("]"));
  Serial.print (" ");
  
  for (int ip = 0; ip < 4; ++ip)
    {
      Serial.print (parName[ip]);
      Serial.print (" ");
      if (parName[ip] == "-N/A-")
      	continue;
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
    Serial.print (GetState (i) ? "O" : "-");
  Serial.println();
#endif

  doDisplay = false;
}

void Algo::UpdatePars()
{
  // Get new values for requested parameters

  bool changed = false; // if any par different than last time
  bool changedNotOffset = false; // if any of pars 0-3 different

  for (int ip = 0; ip < 4; ++ip)
    {
      param newPar = PotToParam(ip);
      if (newPar != parReq[ip])
      {
	parReq[ip] = newPar;
	changed = true;
	if (ip != 3)
	  changedNotOffset = true;
      }
    }
  if (changed)
    {
      SetParPend();
      // New states if change other than offset
      if (changedNotOffset)
	SetStates ();
    }
}

void Algo::SetParPend()
{
  parPend[0] = paramConstrain (0, parReq[0], parReq[1], parReq[2], parReq[3]);
  // Other params may be constrained by constrained period
  parPend[1] = paramConstrain (1, parPend[0], parReq[1], parReq[2], parReq[3]);
  parPend[2] = paramConstrain (2, parPend[0], parReq[1], parReq[2], parReq[3]);
  parPend[3] = paramConstrain (3, parPend[0], parReq[1], parReq[2], parReq[3]);
}

bool Algo::GetState(stateindex index)
{
  // Get (actual) state value with offset

  assert (Period() <= MAX_PERIOD);
  stateindex stepIdx = (index + (Period()-Offset())) % Period();
  return ((statesAct & (0x80000000 >> stepIdx)) != 0);
}

//============================================================

void GapAlgo::Setup()
{
  Algo::Setup();
  algName = "Gap";
  parName[1] = "Trigs"; // 5 chars
  parName[2] = "Gener";
  parName[3] = "Offs ";
  parMax[2] = MAX_PERIOD/2;
  delta[3] = int(1024./(parMax[2]-parMin[2]+1));
}

param GapAlgo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  if (i == 1)
    return constrain (p1, parMin[1], p0+p2 > 0 ? p0/gcd(p0,p2) : p0);
  else if (i == 2)
    return constrain (p2, parMin[2], p0/2);
  else
    return Algo::paramConstrain (i, p0, p1, p2, p3);
}

void GapAlgo::SetParPend()
{
  parPend[0] = paramConstrain (0, parReq[0], parReq[1], parReq[2], parReq[3]);
  // Other params may be constrained by constrained period
  parPend[2] = paramConstrain (2, parPend[0], parReq[1], parReq[2], parReq[3]);
  // Triggers may be further constrained by constrained generator
  parPend[1] = paramConstrain (1, parPend[0], parReq[1], parPend[2], parReq[3]);
  parPend[3] = paramConstrain (3, parPend[0], parReq[1], parPend[2], parReq[3]);
}

void GapAlgo::SetStates()
{
  assert (parPend[0] <= MAX_PERIOD);

  stateindex index = 0;
  statesPend = 1;
    
  for (int var = 1; var < parPend[1]; ++var)
    {
      index = (index + parPend[2]) % parPend[0];
      statesPend |= 0x80000000 >> index;
    }
}

//============================================================

void EuclidAlgo::Setup()
{
  Algo::Setup();
  algName = "Euclidean";
  parName[1] = "Trigs"; // 5 chars
  parName[3] = "Offs ";
}

void EuclidAlgo::SetStates()
{
  // Algorithm source:
  // https://www.computermusicdesign.com/simplest-euclidean-rhythm-algorithm-explained/

  assert (parPend[0] <= MAX_PERIOD);
  int bucket = 0;
  statesPend = 1;

  for (stateindex var = 0; var < parPend[0]; ++var)
    {
      bucket += parPend[1];
      if (bucket >= parPend[0])
	{
	  bucket -= parPend[0];
	  statesPend |= 0x80000000 >> var;
	}
    }
}

//============================================================

void ADCAlgo::Setup()
{
  Algo::Setup();
  algName = "ADC";
  parName[1] = "Value"; // 5 chars
  parName[3] = "Offs ";
  parMin[1] = 0;
  parMax[1] = 1023;
  delta[2] = int(1024./(parMax[1]-parMin[1]+1))+2; /// Inflate to stop churn for now
}

param ADCAlgo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  if (i == 0)
    return constrain (p0, parMin[1], 10);
  else if (i == 1)
    return constrain (p1, parMin[1], parMax[1]);
  else
    return Algo::paramConstrain (i, p0, p1, p2, p3);
}

void ADCAlgo::SetStates()
{
  statesPend = long(parPend[1]) << 22;
}

//============================================================

void RandAlgo::Setup()
{
  Algo::Setup();
  algName = "Random";
  parName[1] = "Trigs"; // 5 chars
  parName[2] = "Throw";
  parName[3] = "Offs ";
}

void RandAlgo::SetStates()
{
  // Set states for a random sequence

  // Make a list of all step numbers, pick one at random and add it to
  // state; move it to the end of the list and repeat with that
  // last entry excluded; et cetera

  assert (parPend[0] <= MAX_PERIOD);
  statesPend = 0;
    
  int ldeck = parPend[0];  
  stateindex deck[ldeck];
  
  for (int id = 0; id < ldeck; ++id)
    deck[id] = id;

  for (int var = 0; var < parPend[1]; ++var)
    {
      int id = random(ldeck);
      statesPend |= 0x80000000 >> deck[id];
      ldeck -= 1;
      int tmp = deck[ldeck];
      deck[ldeck] = deck[id];
      deck[id] = tmp;
    }
}

//============================================================

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

void SetupThisPeriod()
{
  // Set things for this period
  curAlgo = reqAlgo;
  for (int i = 0; i < 4; ++i)
    parAct[i] = parPend[i];
  statesAct = statesPend;
}

void onClockOn()
{
  // Write the next state, and turn on period output and set up new
  // period if counter is 0
  
  counter += 1;
  if (counter >= algoList[curAlgo]->Period())
    {
      counter = 0;
      digitalWrite(PER_OUT_PIN, HIGH);
      if (changeParAct)
	{
	  SetupThisPeriod();
	  doDisplay = true;
	  changeParAct = false;
	}
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


//============================================================

void setup()
{
#if OLED
  u8x8.begin();
  u8x8.setFont(u8x8_font_victoriamedium8_r);
#endif  
#if SERIAL
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

  algoList[0] = new Algo();
  algoList[1] = new GapAlgo();
  algoList[2] = new EuclidAlgo();
  algoList[3] = new ADCAlgo();
  algoList[4] = new RandAlgo();
  lastPotState[0] = analogRead (potlist[0]);
  reqAlgo = constrain (algoList[curAlgo]->PotToParam(-1), 0, NALGO);
  algoList[reqAlgo]->checkPots();
  algoList[reqAlgo]->UpdatePars();
  SetupThisPeriod();
  changePending = false;
  changeParAct = false;
  algoList[curAlgo]->DisParams();
#if SERIAL
  Serial.println("setup done ");
#endif
}

void loop()
{
  if (algoList[reqAlgo]->checkPots())
    {
      lastChangeTime = millis();
      changePending = true;
    }
  else if (changePending && millis - lastChangeTime > 250)
    {
      // First check for new algorithm
      unsigned short int newAlgo = constrain (algoList[curAlgo]->PotToParam(-1), 0, NALGO);
      if (newAlgo != reqAlgo)
	reqAlgo = newAlgo;
      algoList[reqAlgo]->UpdatePars();
      doDisplay = true;
      changePending = false;
      changeParAct = true;
    }
  
  checkReset();
  
  if (sendTick)
    {
      sendTick = false;
  
      clockState = !digitalRead(CLOCK_PIN); // inverted input
      if (clockState != lastClockState)
	{
	  lastClockState = clockState;
	  digitalWrite(CLOCK_LED_PIN, clockState);
      
	  clockState ? onClockOn() : onClockOff();
	}
    }
  
  if (doDisplay)
      algoList[curAlgo]->DisParams();      
}