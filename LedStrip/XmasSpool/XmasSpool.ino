
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const int BUTTON = 10;
const int PIN = 11;
const int PIN2 = 12;
const int NUM_PIXELS = 49; // Spool comes with 50x3 LEDs, but we have to cut off the first set of 3, and they're only addressable in blocks of 3.


// Typical color correction for pixels on a string:
// 255, 224, 140


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN2, NEO_RBG | NEO_KHZ400);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

const auto red = strip.Color(64, 0, 0);  
const auto green = strip.Color(0, 64, 0);  
const auto blue = strip.Color(0, 0, 64);  
const auto purple = strip.Color(64, 0, 192);  // Purple
const auto aqua = strip.Color(0, 128, 192);  // Aqua
const auto white = strip.Color(255, 224, 140); // 
const auto grey = strip.Color(128, 128, 128);
const auto black = strip.Color(0, 0, 0);


// Set all the lights to a color
void colorFill(uint32_t c)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
    }
    strip.show();
}

void alternate(int start)
{
  uint32_t colors[3] = {red, green, blue};
  int numPixels = strip.numPixels();
  for(int i = 0; i < numPixels; ++i)
  {
    strip.setPixelColor(i, colors[(start+i)%3]);
  }
  strip.show();
}



void setup()
{
  pinMode(PIN, OUTPUT);
  pinMode(PIN2, OUTPUT);
// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined(__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

const int minDel = 20;
const int maxDel = 160;
int del = 100;
int currentPos = 0;
int dir = 1;
void loop()
{
    int numPixels = strip.numPixels();

    for(int i = 0; i < numPixels; ++i)
    {
      strip.setPixelColor(i, currentPos == i ? white : black);
    }
    strip.show();
    
    // update position
    currentPos += dir;
    if(currentPos >= NUM_PIXELS)
    {
      currentPos = NUM_PIXELS-1;
      dir = -1;
    }
    else if(currentPos < 0)
    {
      currentPos = 0;
      dir = 1;
    }

    if(random(10) == 0)
    {
    int delta = random(0, 50) - 25;
    del += delta;
    if(del < minDel)
    {
      del = minDel;
    }
    else if(del >= maxDel)
    {
      del = maxDel;
    }
      
    }
    delay(del);
}

