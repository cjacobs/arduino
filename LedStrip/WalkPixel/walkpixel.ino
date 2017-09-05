
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

const int PIN = 11;
const int NUM_PIXELS = 100;

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

int offset = 0;

void loop()
{
    auto evenColor = strip.Color(39, 0, 255);  // Purple
    auto oddColor = strip.Color(0, 128, 192);  // Aqua
    int numPixels = strip.numPixels();

    gradient(0, numPixels, 2);
    gradient(1, numPixels, 2);
    delay(10000);
    return;
    
    walkPixel(0, numPixels, 2, evenColor, 50);
    walkPixel(1, numPixels, 2, evenColor, 50);
}

void walkPixel(int start, int end, int increment, uint32_t color, int t)
{
    auto offColor = strip.Color(0, 0, 0);
    int prevIndex = start;
    for (int index = start; index < end; index += increment)
    {
        strip.setPixelColor(index, color);
        strip.show();
        delay(t/4);
        strip.setPixelColor(prevIndex, offColor);
        prevIndex = index;
        delay(3*t/4);
        strip.show();
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
        strip.show();
    }
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
