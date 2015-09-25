
const int NUM_ROWS = 8;
const int NUM_COLS = 8;
 
const int rowSignalPin = 12;
const int rowClockPin = 11;
const int colSignalPin = 10;
const int colClockPin = 9;
const int outputEnablePin = 8;

const int nextRowInterval = 0;
unsigned long nextRowTime = 0;

int currCol = 0;
int currGlyphLevel = 0;

const unsigned char zChar[] = { 
  0b01111100,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00100000,
  0b01000000,
  0b01000000,
  0b01111100
};

const unsigned char oChar[] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b00111000,
  0b01000100,
  0b01000100,
  0b01000100,
  0b00111000
};

const unsigned char eChar[] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b00111000,
  0b01000100,
  0b01111100,
  0b01000000,
  0b00111100
};

const unsigned char spaceChar[] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const unsigned char fullHeartChar[] = {
  0b00000000,
  0b01101100,
  0b11111110,
  0b11111110,
  0b11111110,
  0b01111100,
  0b00111000,
  0b00010000
};

const unsigned char hollowHeartChar[] = {
  0b00000000,
  0b01101100,
  0b10010010,
  0b10000010,
  0b10000010,
  0b01000100,
  0b00101000,
  0b00010000
};

const unsigned char insideHeartChar[] = {
  0b00000000,
  0b00000000,
  0b01101100,
  0b01111100,
  0b01111100,
  0b00111000,
  0b00010000,
  0b00000000
};

const unsigned char smileCharA1[] = {
  0b01111110,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10111101,
  0b10000001,
  0b01111110
};

const unsigned char smileCharB1[] = {
  0b01111110,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10100101,
  0b10011001,
  0b10000001,
  0b01111110
};

const unsigned char smileCharA2[] = {
  0b01111110,
  0b10000001,
  0b10100101,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b01111110
};

const unsigned char smileCharB2[] = {
  0b01111110,
  0b10000001,
  0b10100101,
  0b10000001,
  0b10000001,
  0b10000001,
  0b10000001,
  0b01111110
};

const unsigned char eyeChar[] = {
  0b00000000,
  0b00000000,
  0b00111100,
  0b01000010,
  0b10010001,
  0b01000010,
  0b00111100,
  0b00000000
};

const unsigned char uChar[] = {
  0b00000000,
  0b00000000,
  0b01000100,
  0b01000100,
  0b01000100,
  0b01000100,
  0b01000100,
  0b00111000
};

unsigned char check1[] = {
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010
};

unsigned char check2[] = {
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101,
  0b10101010,
  0b01010101
};

const unsigned char blankChar[] = {0,0,0,0,0,0,0,0};

const unsigned char* rMessage[] =  {zChar, oChar, eChar, spaceChar, eyeChar, fullHeartChar, uChar, spaceChar, smileCharA1, smileCharB1, spaceChar, check1};
const unsigned char* rMessage2[] = {zChar, oChar, eChar, spaceChar, eyeChar, hollowHeartChar, uChar, spaceChar, smileCharA1, smileCharB1, spaceChar, check1};

const unsigned char* gMessage[] =  {zChar, oChar, eChar, spaceChar, eyeChar, insideHeartChar, uChar, spaceChar, smileCharA2, smileCharB2, spaceChar, check2};
const unsigned char* gMessage2[] = {zChar, oChar, eChar, spaceChar, eyeChar, spaceChar, uChar, spaceChar, smileCharA2, smileCharB2, spaceChar, check2};

const int NUM_LETTERS = sizeof(rMessage) / sizeof(rMessage[0]);
int currLetter = 0;

const int updateLetterInterval = 800;
unsigned long nextLetterTime = 0;

void reset()
{
  // disable outputs and fill shift registers with "off" value
  digitalWrite(outputEnablePin, HIGH); // HIGH disables output
  currCol = 0;
}

void writeByte(unsigned char val, int signalPin, int clockPin)
{
  for(int index = 0; index < 8; index++)
  {
    digitalWrite(signalPin, val & 0x01 ? LOW : HIGH);

    // toggle clock
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);

    val >>= 1;
  }  
}

void setColumn(int col)
{
  shiftOut(colSignalPin, colClockPin, MSBFIRST, 0x01 << col);

  digitalWrite(colClockPin, HIGH);
  digitalWrite(colClockPin, LOW);
}

void setRow(unsigned char rByte, unsigned char gByte)
{
//  writeByte(gByte, rowSignalPin, rowClockPin);    
//  writeByte(rByte, rowSignalPin, rowClockPin);    
  shiftOut(rowSignalPin, rowClockPin, LSBFIRST, ~gByte);
  shiftOut(rowSignalPin, rowClockPin, LSBFIRST, ~rByte);

  digitalWrite(rowClockPin, HIGH);
  digitalWrite(rowClockPin, LOW);
}

void setup() {
  pinMode(rowSignalPin, OUTPUT);
  pinMode(rowClockPin, OUTPUT);
  pinMode(colSignalPin, OUTPUT);
  pinMode(colClockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);

  unsigned long currentMillis = millis();
  nextLetterTime = currentMillis + updateLetterInterval;
  
  reset();
}

void loop()
{  
  unsigned long currentMillis = millis();

  if(currentMillis > nextLetterTime)
  {
    currLetter = (currLetter+1) % NUM_LETTERS;
    nextLetterTime += updateLetterInterval;
  }

  if(currentMillis > nextRowTime)
  {
    digitalWrite(outputEnablePin, HIGH);
    setColumn(currCol);
    if(currGlyphLevel == 0)
    {
      setRow(rMessage[currLetter][currCol], gMessage[currLetter][currCol]);    
    }
    else
    {
      setRow(rMessage2[currLetter][currCol], gMessage2[currLetter][currCol]);
    }
    currGlyphLevel = (currGlyphLevel+1) % 3;
    digitalWrite(outputEnablePin, LOW);

    currCol = (currCol+1) % NUM_COLS;
    nextRowTime = currentMillis + nextRowInterval;
  }

  
}

