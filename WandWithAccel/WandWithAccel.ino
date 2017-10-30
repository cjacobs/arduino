#include <Wire.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_Sensor.h>

const bool useAccel = true;

const int signalPin = 7;
const int outputEnablePin = 6;
const int clockPin = 5;

int currCol = -1;
int dir = 0;

int patternPeriod = 1000; // ms
//int colDuration = patternPeriod / patternLen;
int colDuration = 5;
unsigned long nextColTime = 0;
unsigned long testOffTime = 0;

float filterAlpha = 0.75;
float peakThresh = 10.0;
int minPeakDist = 30;

// ---> == bottom

const unsigned char blankPattern = 0x0;

const unsigned char arrowPattern[] = {
  0,0,0,0,0,0,
  0b00010000,
  0b00010000,
  0b10010001,
  0b01010010,
  0b01010100,
  0b00111000,
  0b00010000,
};
  
const unsigned char pattern2[] = { 
  0,
  0,
  0,
  0,
  0,
  0,
  0b01111110,
  0b10000001,
  0b10000101,
  0b10100101,
  0b10000101,
  0b10100101,
  0b10001001,
  0b10000001,
  0b01111110
};

 const unsigned char pattern[] = { 
//  0,0,0,0,0,0,0,0,
  0,0,
  0b00001101,
  0b00010101,
  0b00001110,
  0b00000000,
  0b00000000,
  0b00001110,
  0b00010001,
  0b00010001,
  0b00001110,
  0b00000000,
  0b00000000,
  0b01000010,
  0b01100010,
  0b01010010,
  0b01001010,
  0b01000110,
  0b01000010,  
  0,0,0,0,0,0,0,0,0,0,0,
  0b00000000 };

const int patternLen = sizeof(pattern)/sizeof(pattern[0]);


/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;


void writeColPattern(unsigned char col)
{
  digitalWrite(outputEnablePin, HIGH);
  
  shiftOut(signalPin, clockPin, LSBFIRST, col);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);

  digitalWrite(outputEnablePin, LOW);
}

void initSensors()
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(signalPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);

  /* Initialise the sensors */
  initSensors();

  Serial.begin(115200);
}


int curr_out_pos = -1;
const int out_buf_len = 3;
float filtered_out[out_buf_len] = {0,0,0};
void filter_signal(float x, float alpha)
{
  curr_out_pos = (curr_out_pos+1) % out_buf_len;
  float prev_y = filtered_out[(curr_out_pos+out_buf_len-1) % out_buf_len];
  filtered_out[curr_out_pos] = prev_y + (x-prev_y)*alpha;
}

float noise_gate(float x, float thresh)
{
  if(x >-thresh && x < thresh)
  {
    return 0.0;
  }
  else
  {
    return x;
  }
}

unsigned long prev_peak_time = 0;
int find_peak(float thresh, int min_peak_dist)
{
  unsigned long curr_time = millis();
    
  if(curr_time-prev_peak_time < min_peak_dist && curr_time > prev_peak_time)
  {
    return 0;
  }
  
  float y[3];
  int peak_val = 0;
  y[0] = noise_gate(filtered_out[(curr_out_pos+out_buf_len-2)%out_buf_len], thresh);
  y[1] = noise_gate(filtered_out[(curr_out_pos+out_buf_len-1)%out_buf_len], thresh);
  y[2] = noise_gate(filtered_out[curr_out_pos+out_buf_len], thresh);
  if((y[1] < y[0] && y[1] <= y[2]))
  {
    peak_val = -1;  
  }
  else if((y[1] > y[0] && y[1] >= y[2]))
  {
    peak_val = 1;
  }

  if(peak_val != 0)
  {
    prev_peak_time = curr_time;
  }
  
  return peak_val;
}


int baseFreq = 10; // only operate at 100hz
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();


//  if(currentMillis % baseFreq != 0)
//  {
//    return;
//  }

  // about 2600 us to read both
  // about 1600 us to read mag
  // about 1000 us to read accel
  
  sensors_event_t accel_event;
//  sensors_event_t mag_event;
//  sensors_vec_t   orientation;

  // Read the sensor(s)
  accel.getEvent(&accel_event);
  //mag.getEvent(&mag_event);

  Serial.print(accel_event.acceleration.x); Serial.print("\t");
  Serial.print(accel_event.acceleration.y); Serial.print("\t");
  Serial.print(accel_event.acceleration.z); Serial.print("\t");
  Serial.println();

  // y value is what we want for detecting want motion
  // TODO: mean-subtract over long period (IIR with very small alpha?), in case wand is tipped so y includes gravity
  filter_signal(accel_event.acceleration.y, filterAlpha);
  int peak_dir = find_peak(peakThresh, minPeakDist);

  if(useAccel)
  {
    if(peak_dir == 1)
    {
      dir = 1;
      currCol = -1;
      nextColTime = 0;
    }
    else if(peak_dir == -1)
    {
      dir = 1;
   //   nextColTime = 0;
    }
  }

  // maybe just set dir to direction of y accel? -- no, need direction of y velocity
  // or maybe double-integrate y accel to get position
  // and reset on peaks?
  // or just integrate accel to get velocity = dir, and reset on peaks
  
  if(currentMillis > nextColTime)
  {
    currCol += dir;

    if(!useAccel)
    {
      if(currCol >= patternLen)
      {
        currCol = patternLen;
        dir = -1;
      }
      else if(currCol < 0)
      {
        currCol = -1;
        dir = 1;      
      }
    }

    const bool test = false;
    if(test)
    {
      if(peak_dir == -1)
      {
        writeColPattern(0xff);
        testOffTime = currentMillis+2;
      }
      if(currentMillis > testOffTime)
      {
        writeColPattern(0x00);
      }
    }
    else
    {
      if(currCol >= 0 && currCol < patternLen && dir == 1)
      {
        writeColPattern(pattern[currCol]);      
      }
      else
      {
        writeColPattern(blankPattern);
      }   
    }

    nextColTime += colDuration;
  }
}

