/*
 */

#define NUM_LEDS 5
const int NUM_COLS = 4;
const int NUM_ROWS = 3;

// constants won't change. Used here to set a pin number :
const int ledPins[NUM_LEDS] = {13,12,11,10,9};

const int colLedPins[] = {8,7,6,5};
const int rowLedPins[] = {4,3,2};

const long interval = 100;           // interval at which to blink (milliseconds)
unsigned long nextBlinkTime = 0;        // will store next time to update LED

bool ledValues[NUM_LEDS];

int darkLed = 0;
int inc = 1;

void setup() {
  // set the digital pins as output:
  for(int index = 0; index < NUM_LEDS; index++)
  {
    pinMode(ledPins[index], OUTPUT);
  }

  for(int index = 0; index < NUM_COLS; index++)
    pinMode(colLedPins[index], OUTPUT);
  for(int index = 0; index < NUM_ROWS; index++)
    pinMode(rowLedPins[index], OUTPUT);
}

void setLeds(bool values[NUM_LEDS])
{
    for(int index = 0; index < NUM_LEDS; index++)
    {
      digitalWrite(ledPins[index], values[index] ? LOW : HIGH);
    } 
}

void setRow(int rowIndex, unsigned char bitmask)
{
  // first turn off all rows
  for(int index = 0; index < NUM_COLS; index++)
  {
    digitalWrite(colLedPins[index], HIGH);
  }

  for(int colIndex = 0; colIndex < NUM_ROWS; colIndex++)
  {
    digitalWrite(rowLedPins[colIndex], bitmask & 0x01 ? HIGH : LOW);
    bitmask >>= 1;
  }

  // now activate the row
  digitalWrite(colLedPins[rowIndex], LOW);
  
}

const int nextRowInterval = 2;
unsigned long nextRowTime = 0;
int currRow = 0;
unsigned char rowVals[] = { 0b101, 0b000, 0b101, 0b111 };


const int updatePatternInterval = 1000;
unsigned long nextPatternTime = 0;

void updatePattern()
{
  for(int rowIndex = 0; rowIndex < NUM_COLS; rowIndex++)
  {
      rowVals[rowIndex] = ~rowVals[rowIndex];
  }
}

void loop()
{  
  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();

  if(currentMillis > nextPatternTime)
  {
    updatePattern();
    nextPatternTime += updatePatternInterval;
  }

  if(currentMillis > nextRowTime)
  {
    currRow = (currRow+1) % NUM_COLS;
    setRow(currRow, rowVals[currRow]);
    nextRowTime = currentMillis + nextRowInterval;
  }

 
  if(currentMillis > nextBlinkTime) 
  {
    nextBlinkTime += interval;

    for(int index = 0; index < NUM_LEDS; index++)
    {
      digitalWrite(ledPins[index], (index==darkLed)?LOW:HIGH);
    }

    for(int index = 0; index < NUM_LEDS; index++)
    {
      ledValues[index] = index == darkLed;
    }
    setLeds(ledValues);

    darkLed += inc;
    if(darkLed < 0)
    {
      darkLed = 1;
      inc = 1;
    }
    else if(darkLed >= NUM_LEDS)
    {
      darkLed = NUM_LEDS-2;
      inc = -1;
    }
  }
}

