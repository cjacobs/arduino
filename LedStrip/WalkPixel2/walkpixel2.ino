
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const int PIN = 11;
const int NUM_PIXELS = 100;


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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_RGB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


void walkPixel(int start, int end, int increment, uint32_t color, int t, int t2=10)
{
    auto offColor = strip.Color(0, 0, 0);
    int prevIndex = start;
    for (int index = start; index < end; index += increment)
    {
        strip.setPixelColor(index, color);
        strip.show();
        delay(t2);
        if(prevIndex != index)
        {
          strip.setPixelColor(prevIndex, offColor);
          strip.show();
        }
        delay(t-t2);
        prevIndex = index;
    }
    strip.setPixelColor(prevIndex, offColor);
}



void gradient(int start, int end, int increment)
{
    float r = 64;
    float g = 64;
    float b = 64;
    int numIterations = (end-start)/increment;
    for(int index = start; index < end; index += increment)
    {
      auto i = (index-start) / increment;
      auto c = strip.Color(i*r/numIterations, i*g/numIterations, i*b/numIterations);
      strip.setPixelColor(index, c);
    }
    strip.show();
}

void fillEvenOdd(uint32_t evenColor, uint32_t oddColor)
{
    for (int index = 0; index < strip.numPixels(); ++index)
    {
        if (index % 2 == 0)
        {
            strip.setPixelColor(index, evenColor);
        }
        else
        {
            strip.setPixelColor(index, oddColor);
        }
    }
    strip.show();
}

// Set all the lights to a color
void colorFill(uint32_t c)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
    }
    strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void setup()
{
// This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined(__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
    // End of trinket special code

    //  Serial.begin(9600);
    //  Serial.println("Hello");

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

bool done = false;

void loop()
{
  if(done) return;
  
    auto grey = strip.Color(128, 128, 128);
    auto white = strip.Color(255, 224, 140);
    auto black = strip.Color(0, 0, 0);
    int numPixels = strip.numPixels();

    // 1 second all on
    colorFill(grey);
    delay(1000);

    // 1/2 second all off
    colorFill(black);
    delay(500);
    
    for (int index = 0; index < numPixels; ++index)
    {
        strip.setPixelColor(index, white);
        strip.show();
        delay(500);

        colorFill(black);
        delay(500);
    }

    // 1 second all on
    colorFill(grey);
    delay(1000);

    colorFill(black);
    delay(10000);

    done = true;
}

