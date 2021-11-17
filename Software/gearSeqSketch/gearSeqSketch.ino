/* G.E.A.R. sequencer Arduino sketch

   Rich Holmes 2021 CC0 1.0

   Portions drawn from 
   https://github.com/katspaugh/arduino-modular-synth/blob/master/arduino-euclid/arduino-euclid.ino
   (No stated license)

 */

#define OLED    1 // nonzero to use OLED
#define FLIPPED 1 // nonzero to flip display vertically (yellow stripe at bottom)
#define SERIAL  0 // nonzero to use serial monitor
#define MAX_PERIOD  32 // can be reduced; increasing would require significant changes
#define NALGO 4   // number of algorithms (other than None)

#include <assert.h>   // for assert macro

#if defined __AVR__   // use DirectIO library for fast digital read/write
#include <DirectIO.h> // --> https://github.com/mmarchetti/DirectIO.git
#endif

#if OLED
#include <U8g2lib.h>  // library for drawing to OLED
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8;  // object for OLED control
#endif

typedef unsigned short int param;
typedef char stateindex;

#define CLOCK_PIN         2
#define CLOCK_LED_PIN     3
#define RESET_LED_PIN     4
#define RESET_IN_PIN      5
#define SEQ_OUT_PIN       6
#define PER_OUT_PIN       7
#define DISP_BLANK_PIN    8

#define ALG_POT  A2
#define CV1_POT  A6
#define CV2_POT  A3
#define CV3_POT  A1
#define CV4_POT  A0

// Set up DirectIO

volatile InputLow<CLOCK_PIN>      i_clock (false);
InputLow<RESET_IN_PIN>            i_reset (false);
InputLow<DISP_BLANK_PIN>          i_disp_blank (true); // internal pullup on
volatile Output<CLOCK_LED_PIN>    o_clock;
Output<RESET_LED_PIN>             o_reset;
volatile Output<SEQ_OUT_PIN>      o_seq;
volatile Output<PER_OUT_PIN>      o_per;

// current algorithm in use and requested next algorithm
volatile unsigned short int curAlgo = 0;
volatile unsigned short int reqAlgo = 0;

// Potentiometers

char potlist[5] = {ALG_POT, CV1_POT, CV2_POT, CV3_POT, CV4_POT}; // alg first, then params pots

enum change {noChange, someChange, specialChange};

param lastPotState[5] = {9999, 9999, 9999, 9999, 9999};
long lastChangeTime = -999999;

// Sequence control
volatile int counter = -1;            // Current step #
volatile bool clockState = false;     // True if clock pulse high
volatile bool lastClockState = false; // old clockState value
bool lastResetState = false;          // old reset value
volatile bool changeParAct = false;   // need to change actual parameters or algorithm

// parReq is requested values for next parameters.
// parPend is requested values modified by constraints, to go into use at
// start of next period.
// parAct is actual parameter values in use.

param parReq[4] = {9999, 9999, 9999, 9999};           // requested values
volatile param parPend[4] = {9999, 9999, 9999, 9999}; // constrained values pending
volatile param parAct[4];                             // actual current values
volatile long statesPend;                             // bits for the pending pattern
volatile long statesAct;                              // bits for the current pattern

// Display control
bool displayPend = false;        // display pending, not actual, parameters
bool displayBlank = false;       // don't display anything
volatile bool doDisplay = false; // update the display when you get a chance

// Forward declarations

int gcd (int a,int b);
void checkReset();
void SetupThisPeriod();
void onClockOn();
void onClockOff();
void tick();
void setup();
void loop();

//============================================================
//
// Main algorithm class and derivatives
//
//============================================================

class Algo
{
  // Implements an sequence creating algorithm, its parameters, and
  // methods relating to it
  
 public:
  Algo() {Setup();}
  virtual void Setup();                        // Set up a new Algo object
  change checkPots();                          // Read the pots and note changes
  param PotToParam(int i);                     // Convert pot to parameter
  virtual param paramConstrain (int i, param p0, param p1, param p2, param p3);
  //                                           // Constrain parameter values
  void DisParams();                            // OLED display
  void UpdatePars();                           // Set requesteed params
  virtual void SetParPend();                   // Set constrained params
  virtual void SetStates() {statesPend = 0;}   // Set the trigger pattern
  bool GetState(stateindex i, long theStates = -999, param thePeriod = 0, param theOffset = 0);
  //                                           // Read the trigger pattern
  virtual param Period() const {return parAct[0];}
  virtual param PeriodPend() const {return parPend[0];}
  virtual param Offset() const {return parAct[3];}
  virtual param OffsetPend() const {return parPend[3];}
  String algName;               // Name of this algorithm
  String parName[4];            // Names of four parameters
  param parMin[4];              // Minimum for requested parameter value
  param parMax[4];              // Maximum for requested parameter value
  short unsigned int delta[5];  // pot range corresponding to change
  //                            // of algo or parameter
};

//============================================================

class GapAlgo: public Algo
{
  // Gap algorithm
  
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
  // Euclidean algorithm
  
 public:
  EuclidAlgo() {Setup();}
  void Setup();
  void SetStates();
};

//============================================================

class ADCAlgo: public Algo
{
  // ADC algorithm
  
 public:
  ADCAlgo() {Setup();}
  void Setup();
  void SetStates();
  param paramConstrain (int i, param p0, param p1, param p2, param p3);
};

//============================================================

class RandAlgo: public Algo
{
  // Random algorithm
  
 public:
  RandAlgo() {Setup();}
  void Setup();
  void SetStates();
};

//============================================================

volatile Algo* algoList[5];  // Vector of pointers to singleton algorithms:
//                           // None, Gap, Euclidean, ADC, Random

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
  // Significant pot change is width corresponding to change of parameter
  delta[0] = int(1024./(NALGO+1));
  for (int i = 0; i < 4; ++i)
    delta[i+1] = int(1024./(parMax[i]-parMin[i]+1));
}

//==============================

change Algo::checkPots()
{
  // Read pots and store new values if changed enough
  // If anything changed return someChange, otherwise noChange
  // unless only change was smallish change in ADC algo param 2
  // then return specialChange
  
  change whatChanged = noChange;
  bool onlySmallADCValChange = true;
  for (int ipot = 0; ipot < 5; ++ipot)
    {
      int potState = analogRead (potlist[ipot]);
      int ch = abs(int(potState)-int(lastPotState[ipot]));
      if (ch < delta[ipot])
	continue;
      lastPotState[ipot] = potState;
      if (reqAlgo != 3 || ipot != 2 || ch > 10)
	onlySmallADCValChange = false;
      whatChanged = someChange;
    }
  if (whatChanged == someChange && onlySmallADCValChange)
    return specialChange;
  else
    return whatChanged;
}

//==============================

param Algo::PotToParam(int i)
{
  // Map pot value to parMin..parMax
  // i == -1 for algorithm pot, 0-3 for parameter pots

  assert (i >= -1 && i < 4);
  v = lastPotState[i+1];
  if (i == -1) // algorithm pot
    return param(float(v)/(1024./(NALGO+1))); // 0 to NALGO
  else
    return param(float(v)/(1024./(parMax[i]-parMin[i]+1))+parMin[i]);
}

//==============================

param Algo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  // Constrain the ith requested parameter value, given value of it
  // and other three; this is used as the pending value of that parameter
  
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

//==============================

void Algo::DisParams ()
{
  // Display algorithm, parameters, and trigger pattern on OLED
  // and/or serial monitor
  
  String d_algName;
  String d_parName[4];
  param d_par[4];
  long d_states;
  int d_period;
  int d_offset;
  char* xchar = "X";
  char* dotchar = ".";
  char* ochar = "O";
  char* dashchar = "-";
  char* d_on;
  char* d_off;

  // collect the data to be shown
  
  if (displayPend)
    {
      // For pending parameters display (while a pot is being changed)
      
      d_algName = algoList[reqAlgo]->algName;
      for (int ip = 0; ip < 4; ++ip)
	{
	  d_parName[ip] = algoList[reqAlgo]->parName[ip];
	  d_par[ip] = parPend[ip];
	}
      d_states = statesPend;
      d_period = PeriodPend();
      d_offset = OffsetPend();
      d_on = xchar;
      d_off = dotchar;
    }
  else
    {
      // For actual parameters display (while things are static)
      
      d_algName = algoList[curAlgo]->algName;
      for (int ip = 0; ip < 4; ++ip)
	{
	  d_parName[ip] = algoList[curAlgo]->parName[ip];
	  d_par[ip] = parAct[ip];
	}
      d_states = statesAct;
      d_period = Period();
      d_offset = Offset();
      d_on = ochar;
      d_off = dashchar;
    }

#if OLED

  // OLED update

  // Top line, algorithm name and "*" for pending display
  String str;
  char line[32];
  String top = d_algName + (algoList[reqAlgo]->algName == d_algName ? "" :
			  String ("[")+
			  algoList[reqAlgo]->algName.substring(0,3)+String("]"));
  int c = (16-top.length())/2;
  top.toCharArray(line, 17);
  u8x8.clearLine (0);
  if (displayPend)
    u8x8.drawString (0, 0, "*");
  u8x8.drawString (c, 0, line);

  // Next four lines, parameters — actual or pending — and requested
  // values if different
  char l = 1;
  for (int ip = 0; ip < 4; ++ip)
    {
      u8x8.clearLine (l);
      d_parName[ip].toCharArray(line, 6);
      u8x8.drawString (0, l, line);
      if (d_parName[ip] != "-N/A-")
	{
	  str = String(d_par[ip]);
	  while (str.length() < 4)
	    str = String(" ")+str;
	  str.toCharArray(line, 5);
	  u8x8.drawString (6, l, line);
	  
	  if (d_par[ip] != parReq[ip])
	    {
	      str = String(parReq[ip]);
	      str = String("[") + str + String("]");
	      str.toCharArray(line, 7);
	      u8x8.drawString (10, l, line);
	    }
	}
      ++l;
    }

  // Clear the following lines
  while (l < 8)
    u8x8.clearLine (l++);

  // Last one or two lines are the trigger pattern
  l = d_period < 17 ? 7 : 6;
  c = 0;
  for (int i = 0; i < d_period; ++i)
    {
      u8x8.drawString (c++, l, GetState (i, d_states, d_period, d_offset) ? d_on : d_off);
      if (c == 16)
	{
	  c = 0;
	  l++;
	}
    }

#endif
#if SERIAL
  // Print parameters and sequence to serial monitor

  // Name and "*" for pending display
  if (displayPend)
    Serial.print ("* ");
  Serial.print (d_algName);
  if (algoList[reqAlgo]->algName != d_algName)
    Serial.print (String ("[")+algoList[reqAlgo]->algName.substring(0,3)+String("]"));
  Serial.print (" ");

  // Parameters — actual or pending — and requested values if
  // different
  for (int ip = 0; ip < 4; ++ip)
    {
      Serial.print (d_parName[ip]);
      Serial.print (" ");
      if (d_parName[ip] == "-N/A-")
      	continue;
      Serial.print (d_par[ip]);
      if (d_par[ip] != parReq[ip])
      	{
      	  Serial.print (" [");
      	  Serial.print (parReq[ip]);
      	  Serial.print ("]");
      	}
      Serial.print (" ");
    }
  Serial.println();

  // Trigger pattern
  for (int i = 0; i < d_period; ++i)
    Serial.print (GetState (i, d_states, d_period, d_offset) ? d_on : d_off);
  Serial.println();
#endif

  doDisplay = false;
}

//==============================

void Algo::UpdatePars()
{
  // Get values for requested parameters, and if any have changed,
  // update the pending parameters (and algorithm) and trigger pattern

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

//==============================

void Algo::SetParPend()
{
  // Set pending parameters from constrained requested parameters
  
  parPend[0] = paramConstrain (0, parReq[0], parReq[1], parReq[2], parReq[3]);
  // Other params may be constrained by constrained period
  parPend[1] = paramConstrain (1, parPend[0], parReq[1], parReq[2], parReq[3]);
  parPend[2] = paramConstrain (2, parPend[0], parReq[1], parReq[2], parReq[3]);
  parPend[3] = paramConstrain (3, parPend[0], parReq[1], parReq[2], parReq[3]);
}

//==============================

bool Algo::GetState(stateindex index, long theStates, param thePeriod, param theOffset)
{
  // Get state value with offset. Use specified theStates, thePeriod, and
  // theOffset, unless theStates is -999, then use actual current ones.

  // Pettern is stored as a 32 bit word, one bit per sequencer step
  
  if (theStates == -999)
    {
      theStates = statesAct;
      thePeriod = Period();
      theOffset = Offset();
    }
  
  assert (thePeriod <= MAX_PERIOD);
  stateindex stepIdx = (index + (thePeriod-theOffset)) % thePeriod;
  return ((theStates & (0x80000000 >> stepIdx)) != 0);
}

//============================================================

void GapAlgo::Setup()
{
  Algo::Setup();
  algName = "Gap";
  parName[1] = "Trigs"; // 5 chars
  parName[2] = "Gener";
  parName[3] = "Offs ";
  parMin[2] = 1;
  parMax[2] = MAX_PERIOD/2;
  delta[3] = int(1024./(parMax[2]-parMin[2]+1));
}

//==============================

param GapAlgo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  // Constrain the ith requested parameter value, given value of it
  // and other three; this is used as the pending value of that parameter
  
  if (i == 1)
    return constrain (p1, parMin[1], p0+p2 > 0 ? p0/gcd(p0,p2) : p0);
  else if (i == 2)
    return constrain (p2, parMin[2], p0/2);
  else
    return Algo::paramConstrain (i, p0, p1, p2, p3);
}

//==============================

void GapAlgo::SetParPend()
{
  // Set pending parameters from constrained requested parameters
  
  parPend[0] = paramConstrain (0, parReq[0], parReq[1], parReq[2], parReq[3]);
  // Other params may be constrained by constrained period
  parPend[2] = paramConstrain (2, parPend[0], parReq[1], parReq[2], parReq[3]);
  // Triggers may be further constrained by constrained generator
  parPend[1] = paramConstrain (1, parPend[0], parReq[1], parPend[2], parReq[3]);
  parPend[3] = paramConstrain (3, parPend[0], parReq[1], parPend[2], parReq[3]);
}

//==============================

void GapAlgo::SetStates()
{
  // Set the trigger pattern using the gap algorithm
  // Pettern is stored as a 32 bit word, one bit per sequencer step
  
  if (parPend[1] == 0)
    statesPend = 0x0;
  else
    {
      stateindex index = 0;
      statesPend = 0x80000000;

      for (int var = 1; var < parPend[1]; ++var)
	{
	  index = (index + parPend[2]) % parPend[0];
	  statesPend |= 0x80000000 >> index;
	}
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

//==============================

void EuclidAlgo::SetStates()
{
  // Set the trigger pattern using the Euclidean algorithm
  // Pettern is stored as a 32 bit word, one bit per sequencer step

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

//==============================

param ADCAlgo::paramConstrain (int i, param p0, param p1, param p2, param p3)
{
  // Constrain the ith requested parameter value, given value of it
  // and other three; this is used as the pending value of that parameter
  
  if (i == 0)
    return constrain (p0, parMin[1], 10);
  else if (i == 1)
    return constrain (p1, parMin[1], parMax[1]);
  else
    return Algo::paramConstrain (i, p0, p1, p2, p3);
}

//==============================

void ADCAlgo::SetStates()
{
  // Set the trigger pattern using the gap algorithm
  // Pettern is stored as a 32 bit word, one bit per sequencer step

  statesPend = long(parPend[1]) << 22;  // OK, that was easy
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

//==============================

void RandAlgo::SetStates()
{
  // Set the trigger pattern using the ramdom algorithm
  // Pettern is stored as a 32 bit word, one bit per sequencer step

  // Make a list of all step numbers, pick one at random and set it to
  // 1 in state; move it to the end of the list and repeat with that
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
  // Greatest common divisor (the REAL Euclidean algorithm!) for
  // Gap sequence trigger parameter constraints
  
  int R;
  while ((a % b) > 0)
    {
      R = a % b;
      a = b;
      b = R;
    }
  return b;
}

//==============================

void checkReset()
{
  // See if we need to reset this sequence

  bool reset = i_reset.read();
  if (reset == lastResetState) return;
  lastResetState = reset;
  if (reset)
    counter = -1; // will be zero next clock pulse
  o_reset.write(reset);
}

//==============================

void SetupThisPeriod()
{
  // Set things for this period — copying pending values to actual
  curAlgo = reqAlgo;
  for (int i = 0; i < 4; ++i)
    parAct[i] = parPend[i];
  statesAct = statesPend;
}

//==============================

void onClockOn()
{
  // Write the next state, and turn on period output and set up new
  // period if counter is 0
  
  counter += 1;
  if (counter >= algoList[curAlgo]->Period())
    counter = 0;
  if (counter == 0)
    {
      if (changeParAct)
	{
	  SetupThisPeriod();
	  doDisplay = true;     // update the display when there's time 
	  changeParAct = false; // no changes are pending any more
	}
      if (curAlgo > 0) // no period output if algo is None
	o_per.write(HIGH);
    }

  o_seq.write(algoList[curAlgo]->GetState(counter));
}

//==============================

void onClockOff()
{
  // Turn off state and param1 output

  o_seq.write(LOW);
  o_per.write(LOW);
}

//==============================

void tick() // ISR function
{
  clockState = i_clock.read();
  if (clockState != lastClockState)
    {
      lastClockState = clockState;
      o_clock.write(clockState);
      
      clockState ? onClockOn() : onClockOff();
    }
}

//============================================================

void setup()
{
#if OLED
  u8x8.begin();
  u8x8.setFont(u8x8_font_victoriamedium8_r);
  u8x8.setFlipMode(FLIPPED);
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

  // Set up array of singleton algorithm objects
  algoList[0] = new Algo();
  algoList[1] = new GapAlgo();
  algoList[2] = new EuclidAlgo();
  algoList[3] = new ADCAlgo();
  algoList[4] = new RandAlgo();

  // Get initial pot values
  for (int ipot = 0; ipot < 5; ++ipot)
    lastPotState[ipot] = analogRead (potlist[ipot]);

  // Initial algo value
  reqAlgo = constrain (algoList[0]->PotToParam(-1), 0, NALGO);
  curAlgo = reqAlgo;

  // Update pending and actual parameters and states
  algoList[reqAlgo]->UpdatePars();
  SetupThisPeriod();

  // Display starting parameters
  assert (curAlgo >= 0 && curAlgo < 5);
  algoList[curAlgo]->DisParams();
  changeParAct = false;

  // Ready to start interrupts now
  attachInterrupt (digitalPinToInterrupt(CLOCK_PIN), tick, CHANGE);

#if SERIAL
  Serial.println("setup done ");
#endif
}

//==============================

void loop()
{
  // Examine the pots and see if there's been a significant change
  change whatChanged = algoList[reqAlgo]->checkPots();
  if (whatChanged != noChange)
    {
      if (whatChanged == someChange)
	{
	  // Turn on pending values display and update change time only
	  // if change was NOT just ADC algo channel 2 by a small amount
	  // (because that one has 10 significant bits and they can
	  // fluctuate if there's noise)
	  displayPend = true;
	  lastChangeTime = millis();
	}
      doDisplay = true;
      // Check for new algorithm
      unsigned short int newAlgo = constrain (algoList[curAlgo]->PotToParam(-1), 0, NALGO);
      if (newAlgo != reqAlgo)
	reqAlgo = newAlgo;

      // Update the requested parameters, and remind ourself to change'
      // actual parameters next sequencer period
      algoList[reqAlgo]->UpdatePars();
      changeParAct = true;
    }

  // Did someone say RESET?
  checkReset();

  // Stop displaying pending values after changes stop for a while
  if (displayPend && (millis() - lastChangeTime > 2000))
    {
      displayPend = false;
      doDisplay = true;
    }

  // If the display blanking switch has changed, change the boolean
  // and if it's now on, blank the display; if it's off, trigger display.
  // (This affects serial output too.)
  bool dbValue = i_disp_blank.read();
  if (dbValue != displayBlank)
    {
      displayBlank = dbValue;
      if (displayBlank)
#if OLED
	u8x8.setPowerSave(true);
#endif
      else
	{
	  doDisplay = true;
#if OLED
	  u8x8.setPowerSave(false);
#endif
	}
    }

  // Update the display if needed
  if (doDisplay && !displayBlank)
      algoList[curAlgo]->DisParams();      
}