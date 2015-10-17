

const int signalPin = 7;
const int clockPin = 6;
const int outputEnablePin = 5;

// ---> == bottom

const unsigned char pattern[] = { 
  0b01111110,
  0b10000001,
  0b10001001,
  0b10100101,
  0b10000101,
  0b10100101,
  0b10001001,
  0b10000001,
  0b01111110,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
};

/*
 const unsigned char pattern[] = { 
  0b00000000,
  0b00000000,
  0b01111111,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b01111111,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b01011111,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000001,
  0b01000010,
  0b01000110,
  0b01001010,
  0b01010010,
  0b01100010,
  0b01000010,
  0b00000000 };
*/ 

const int patternLen = sizeof(pattern)/sizeof(pattern[0]);

int currCol = -1;
int dir = 0;

int patternPeriod = 1000; // ms
//int colDuration = patternPeriod / patternLen;
int colDuration = 4;
unsigned long nextColTime = 0;

void writeColPattern(unsigned char col)
{
  shiftOut(signalPin, clockPin, LSBFIRST, col);

  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
}


void setup() {
  // put your setup code here, to run once:
  pinMode(signalPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if(currentMillis > nextColTime)
  {
    currCol += dir;
    if(currCol >= patternLen)
    {
      currCol = patternLen-1;
      dir = -1;
    }
    else if(currCol < 0)
    {
      currCol = 0;
      dir = 1;      
    }

    digitalWrite(outputEnablePin, HIGH);
    writeColPattern(pattern[currCol]);
    digitalWrite(outputEnablePin, LOW);

    nextColTime += colDuration;
  }
}
