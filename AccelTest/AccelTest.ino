#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_9DOF                dof   = Adafruit_9DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;


int rgb[3] = {0,0,0};
const int rPin = 6;
const int gPin = 5;
const int bPin = 3;

void setColor(int rgb[])
{
  analogWrite(rPin, 255-rgb[0]);
  analogWrite(gPin, 255-rgb[1]);
  analogWrite(bPin, 255-rgb[2]);
}

/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
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

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Adafruit 9 DOF Pitch/Roll/Heading Example")); Serial.println("");
  
  /* Initialise the sensors */
  initSensors();

  // init LED pins
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
}

/**************************************************************************/
/*!
    @brief  Constantly check the roll/pitch/heading/altitude/temperature
*/
/**************************************************************************/
void loop(void)
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  /* Read the accelerometer and magnetometer */
  accel.getEvent(&accel_event);
  mag.getEvent(&mag_event);

  /* Use the new fusionGetOrientation function to merge accel/mag data */  
  if (dof.fusionGetOrientation(&accel_event, &mag_event, &orientation))
  {
    // roll in -90-90
    // pitch in -180-180
    // heading in 0-360 according to docs, but really -180-180

    rgb[0] = constrain(((orientation.roll+90)/180)*255, 0, 255);
    rgb[1] = constrain(((orientation.pitch+180)/360)*255, 0, 255);
    rgb[2] = constrain(((orientation.heading+180)/360)*255, 0, 255);
    setColor(rgb);


    Serial.print(rgb[0]);
    Serial.print(F(" "));
    Serial.print(rgb[1]);
    Serial.print(F(" "));
    Serial.print(rgb[2]);
    Serial.println();
    
    // 'orientation' should have valid .roll and .pitch fields 
    Serial.print(F("Orientation: "));
    Serial.print(orientation.roll);
    Serial.print(F(" "));
    Serial.print(orientation.pitch);
    Serial.print(F(" "));
    Serial.print(orientation.heading);
    Serial.println(F(""));
  }
  
  delay(100);
}
