
// NeoPixel library:
#include <Adafruit_NeoPixel.h>

// FastLED library:
/*
#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>
*/

#define RED_LED 11
#define WHITE_LED 12

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
}

void loop() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(WHITE_LED, LOW);
  delay(500);

  digitalWrite(RED_LED, LOW);
  digitalWrite(WHITE_LED, HIGH);    
  delay(500);
}
