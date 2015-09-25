
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

unsigned char zChar[] = { 
  0b01111100,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00100000,
  0b01000000,
  0b01000000,
  0b01111100
};

unsigned char oChar[] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b00111000,
  0b01000100,
  0b01000100,
  0b01000100,
  0b00111000
};

unsigned char eChar[] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b00111000,
  0b01000100,
  0b01111100,
  0b01000000,
  0b00111100
};

unsigned char spaceChar[] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

unsigned char heartChar[] = {
  0b00000000,
  0b01101100,
  0b11111110,
  0b11111110,
  0b11111110,
  0b01111100,
  0b00111000,
  0b00010000
};

unsigned char heartChar2[] = {
  0b00000000,
  0b01101100,
  0b10010010,
  0b10000010,
  0b10000010,
  0b01000100,
  0b00101000,
  0b00010000
};

unsigned char smileCharA[] = {
  0b01111110,
  0b10000001,
  0b10100101,
  0b10000001,
  0b10000001,
  0b10111101,
  0b10000001,
  0b01111110
};

unsigned char smileCharB[] = {
  0b01111110,
  0b10000001,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b10000001,
  0b01111110
};

unsigned char faceChar[] = {
  0b00000000,
  0b00111100,
  0b01011010,
  0b11111111,
  0b11000011,
  0b11000011,
  0b01100110,
  0b00111100
};


unsigned char eyeChar[] = {
  0b00000000,
  0b00000000,
  0b00111100,
  0b01000010,
  0b10010001,
  0b01000010,
  0b00111100,
  0b00000000
};

unsigned char uChar[] = {
  0b00000000,
  0b00000000,
  0b01000100,
  0b01000100,
  0b01000100,
  0b01000100,
  0b01000100,
  0b00111000
};

unsigned char* message[] = {zChar, oChar, eChar, spaceChar, eyeChar, heartChar, uChar, spaceChar, smileCharA, smileCharB, spaceChar};
unsigned char* message2[] = {zChar, oChar, eChar, spaceChar, eyeChar, heartChar2, uChar, spaceChar, smileCharA, smileCharB, spaceChar};
const int NUM_LETTERS = sizeof(message) / sizeof(message[0]);
int currLetter = 0;

const int updateLetterInterval = 800;
unsigned long nextLetterTime = 0;

void reset()
{
  // disable outputs and fill shift registers with "off" value
  digitalWrite(outputEnablePin, HIGH); // HIGH disables output
  digitalWrite(rowSignalPin, HIGH); // HIGH is off for rows
  digitalWrite(colSignalPin, LOW); // LOW is off for cols
  for(int index = 0; index < NUM_ROWS; index++)
  {
    digitalWrite(rowClockPin, HIGH);
    digitalWrite(colClockPin, HIGH);
    digitalWrite(rowClockPin, LOW);
    digitalWrite(colClockPin, LOW);  
  }

  // init to first column on:
  currCol = 0;
  digitalWrite(colSignalPin, HIGH);
  digitalWrite(colClockPin, HIGH);
  digitalWrite(colClockPin, LOW);
}

void setColumn(int col)
{
  digitalWrite(colSignalPin, LOW);
  for(int index = 0; index < 9; index++)
  {
    digitalWrite(colClockPin, HIGH);
    digitalWrite(colClockPin, LOW);
  }  

  digitalWrite(colSignalPin, HIGH);
  digitalWrite(colClockPin, HIGH);
  digitalWrite(colClockPin, LOW);

  digitalWrite(colSignalPin, LOW);
  for(int index = 0; index <= col; index++)
  {
    digitalWrite(colClockPin, HIGH);
    digitalWrite(colClockPin, LOW);
  }
}

void advanceColumn()
{
  currCol = (currCol+1) % 8;

  digitalWrite(colSignalPin, currCol == 0 ? HIGH : LOW);

  // toggle the column clock
  digitalWrite(colClockPin, HIGH);
  digitalWrite(colClockPin, LOW);
}

void setRow(unsigned char bitmask)
{
  for(int index = 0; index < NUM_COLS; index++)
  {
    digitalWrite(rowSignalPin, bitmask & 0x01 ? LOW : HIGH);

    // toggle clock
    digitalWrite(rowClockPin, HIGH);
    digitalWrite(rowClockPin, LOW);

    bitmask >>= 1;
  }
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
    setRow(message[currLetter][currCol]);    
    }
    else
    {
    setRow(message2[currLetter][currCol]);          
    }
    currGlyphLevel = (currGlyphLevel+1) % 3;
//    advanceColumn();
    digitalWrite(outputEnablePin, LOW);

    currCol = (currCol+1) % NUM_COLS;
    nextRowTime = currentMillis + nextRowInterval;
  }

  
}

