const int rPin = 6;
const int gPin = 5;
const int bPin = 3;


void setup() {
  // put your setup code here, to run once:
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);

}

void setColor(int rgb[])
{
  analogWrite(rPin, 255-rgb[0]);
  analogWrite(gPin, 255-rgb[1]);
  analogWrite(bPin, 255-rgb[2]);
}

int period = 10;
unsigned int nextT = 0;

int rgb[3] = {0, 0, 0};
int currIndex = 0;
int dir = 1;

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long t = millis();
  
  if (t >= nextT)
  {
    rgb[currIndex] += dir;
    if(rgb[currIndex] > 255)
    {
      rgb[currIndex] = 254;
      currIndex += 1;
    }
    else if(rgb[currIndex] < 0)
    {
      rgb[currIndex] = 1;
      currIndex += 1;
    }
    if(currIndex > 2)
    {
      currIndex = 0;
      dir = -1*dir;
    }
    
    setColor(rgb);
    nextT += period;
  }
  
}
