// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Preferences.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    4

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 21



// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------
Preferences configuracao;

int animacao = configuracao.getInt("_animacao", 0);

void IRAM_ATTR selecao() {
  animacao++;
  configuracao.putInt("_animacao", animacao);
  Serial.println(animacao);
}

void setup() {

  Serial.begin(9600);
  pinMode(9, INPUT);
  pinMode(2, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), selecao, FALLING);
  attachInterrupt(digitalPinToInterrupt(9), selecao, FALLING);

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(150); // Set BRIGHTNESS to about 1/5 (max = 255)

  configuracao.begin("_animacao", false);
  animacao = configuracao.getInt("_animacao", 0);


}


// loop() function -- runs repeatedly as long as board is on ---------------


void loop() {

  Serial.println(configuracao.getInt("_animacao", 10));

  switch (animacao) {
    case 0:
      rainbow(10);
      break;
    case 1:
      twinkleChristmas(strip.Color(255, 0, 0), strip.Color(0, 255, 0), 150);
      break;

    case 2:
      chaseLights(strip.Color(255, 255, 0), 2);
      break;

    case 3:
      pulsingStar(strip.Color(0, 0, 255), 30);
      break;

    case 4:
      expandir(strip.Color(random(255), 100, 0), 60);
      break;

    case 5:
      colorWipe(strip.Color(0, 50, 130), 50);
      colorWipeReversa(strip.Color(0,   0,   0), 50);
      break;
    case 6:
      theaterChase(strip.Color(127,   50,   0), 50); // Red, half brightness
      break;
    case 7:
      theaterChase2(strip.Color(127,   50,   0), strip.Color(10,   150,   0), 50); // Red, half brightness
      break;
    case 8:
      theaterChase2rand(strip.Color(180,   20,   0), strip.Color(10,   150,   40), 50); // Red, half brightness
      break;
    case 9:
      pulsingStar2(strip.Color(180,   20,   0), strip.Color(10,   150,   40), 50); // Red, half brightness
      break;

    case 10:
      pulseRand(200, 100, 1, 20);
      break;
    case 11:
      pulseRand(10, 150, 50, 100);
      break;
    case 12:
      chaseLights2(50, 0, 0, 2);
      break;

    default:
      animacao = 0;
      break;
  }


  // Fill along the length of the strip in various colors...


  /*  colorWipe(strip.Color(  0, 255,   0), 50); // Green
    colorWipe(strip.Color(  0,   0, 255), 50); // Blue

    // Do a theater marquee effect in various colors...
    theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
    theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
    theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

    rainbow(10);             // Flowing rainbow cycle along the whole strip
    theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant*/
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  int animacao_ant = animacao;
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
    if (animacao != animacao_ant) {
      break;
    }
  }
}


void colorWipeReversa(uint32_t color, int wait) {
  int animacao_ant = animacao;
  for (int i = strip.numPixels() - 1; i > -1; i--) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
    if (animacao != animacao_ant) {
      break;
    }
  }
}

void pulseRand(int r, int g, int b, int wait) {
  int animacao_ant = animacao;
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    //strip.setPixelColor(i, strip.Color((r+random(255))%255,   (g+random(255))%255,   (b+random(255))%255));         //  Set pixel's color (in RAM)
    strip.setPixelColor(i, strip.Color(random(255) % r,   random(255) % g,   random(255) % b));

    strip.show();                          //  Update strip to match
    if (animacao != animacao_ant) {
      break;
    }
  }
  delay(wait);                           //  Pause for a moment
}


void colorWipeReversaRand( int wait) {
  int animacao_ant = animacao;
  for (int i = strip.numPixels() - 1; i > -1; i--) { // For each pixel in strip...
    strip.setPixelColor(i, strip.Color(random(255),   random(255),   random(255)));         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
    if (animacao != animacao_ant) {
      break;
    }
  }
}


// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  int animacao_ant = animacao;
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        if (animacao != animacao_ant) {
          break;
        }
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
      if (animacao != animacao_ant) {
        break;
      }
    }
  }
}



void theaterChase2rand(uint32_t color1, uint32_t color2, int wait) {
  int animacao_ant = animacao;
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        if (random(0, 2) == 0) {
          strip.setPixelColor(c, color1); // Set pixel 'c' to value 'color'
        }
        else {
          strip.setPixelColor(c, color2); // Set pixel 'c' to value 'color'
        }
        if (animacao != animacao_ant) {
          break;
        }
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
      if (animacao != animacao_ant) {
        break;
      }
    }
  }
}

void theaterChase2(uint32_t color1, uint32_t color2, int wait) {
  int animacao_ant = animacao;
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        if (c % 2 == 0) {
          strip.setPixelColor(c, color1); // Set pixel 'c' to value 'color'
        }
        else {
          strip.setPixelColor(c, color2); // Set pixel 'c' to value 'color'
        }
        if (animacao != animacao_ant) {
          break;
        }
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
      if (animacao != animacao_ant) {
        break;
      }
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  int animacao_ant = animacao;
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    strip.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
    if (animacao != animacao_ant) {
      break;
    }
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int animacao_ant = animacao;
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        if (animacao != animacao_ant) {
          break;
        }
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
      if (animacao != animacao_ant) {
        break;
      }
    }
  }
}


void twinkleChristmas(uint32_t color1, uint32_t color2, int delayMs) {
  int animacao_ant = animacao;
  for (int i = 0; i < strip.numPixels(); i++) {
    if (random(2) == 0) {
      strip.setPixelColor(i, color1);
    } else {
      strip.setPixelColor(i, color2);
    }
    if (animacao != animacao_ant) {
      break;
    }
  }
  strip.show();
  delay(delayMs);
}

void expandir(uint32_t color, int delayMs) {
  int animacao_ant = animacao;
  for (int j = 0; j < 8; j++) {
    switch (j) {
      case 0:
        strip.setPixelColor(21-1, color);
        break;
      case 1:
        strip.setPixelColor(1-1, color);
        strip.setPixelColor(17-1, color);
        strip.setPixelColor(13-1, color);
        strip.setPixelColor(9-1, color);
        strip.setPixelColor(5-1, color);

        break;
      case 2:
        strip.setPixelColor(2-1, color);
        strip.setPixelColor(4-1, color);
        strip.setPixelColor(6-1, color);
        strip.setPixelColor(8-1, color);
        strip.setPixelColor(10-1, color);
        strip.setPixelColor(12-1, color);
        strip.setPixelColor(14-1, color);
        strip.setPixelColor(16-1, color);
        strip.setPixelColor(18-1, color);
        strip.setPixelColor(20-1, color);

        break;
      case 3:
        strip.setPixelColor(3-1, color);
        strip.setPixelColor(7-1, color);
        strip.setPixelColor(11-1, color);
        strip.setPixelColor(15-1, color);
        strip.setPixelColor(19-1, color);

        break;
      case 4:
        strip.setPixelColor(3-1, strip.Color(0, 0, 0));
        strip.setPixelColor(7-1, strip.Color(0, 0, 0));
        strip.setPixelColor(11-1, strip.Color(0, 0, 0));
        strip.setPixelColor(15-1, strip.Color(0, 0, 0));
        strip.setPixelColor(19-1, strip.Color(0, 0, 0));

        break;
      case 5:
        strip.setPixelColor(2-1, strip.Color(0, 0, 0));
        strip.setPixelColor(4-1, strip.Color(0, 0, 0));
        strip.setPixelColor(6-1, strip.Color(0, 0, 0));
        strip.setPixelColor(8-1, strip.Color(0, 0, 0));
        strip.setPixelColor(10-1, strip.Color(0, 0, 0));
        strip.setPixelColor(12-1, strip.Color(0, 0, 0));
        strip.setPixelColor(14-1, strip.Color(0, 0, 0));
        strip.setPixelColor(16-1, strip.Color(0, 0, 0));
        strip.setPixelColor(18-1, strip.Color(0, 0, 0));
        strip.setPixelColor(20-1, strip.Color(0, 0, 0));

        break;
      case 6:
        strip.setPixelColor(1-1, strip.Color(0, 0, 0));
        strip.setPixelColor(17-1, strip.Color(0, 0, 0));
        strip.setPixelColor(13-1, strip.Color(0, 0, 0));
        strip.setPixelColor(9-1, strip.Color(0, 0, 0));
        strip.setPixelColor(5-1, strip.Color(0, 0, 0));

        break;
      case 7:
        strip.setPixelColor(21-1, strip.Color(0, 0, 0));
        break;
    }
    
    strip.show();
    delay(delayMs);
    if (animacao != animacao_ant) {
      break;
    }
  }
}

void chaseLights(uint32_t color, int delayMs) {
  int animacao_ant = animacao;
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(random(strip.numPixels()), color);
    if (i > 0) strip.setPixelColor(random(strip.numPixels()), 0); // Apaga o LED anterior
    strip.show();
    delay(delayMs);
    if (animacao != animacao_ant) {
      break;
    }
  }
  //strip.setPixelColor(strip.numPixels() - 1, 0);
}


void chaseLights2(int r, int g, int b, int delayMs) {
  int animacao_ant = animacao;
  for (int i = 0; i < strip.numPixels(); i++) {

    switch (random(3)) {
      case 0:
        strip.setPixelColor(random(strip.numPixels()), strip.Color(255 - r, g, b));
        break;

      case 1:
        strip.setPixelColor(random(strip.numPixels()), strip.Color(r, 255 - g, b));
        break;

      case 2:
        strip.setPixelColor(random(strip.numPixels()), strip.Color(r, g, 255 - b));
        break;
    }

    if (i > 0) strip.setPixelColor(random(strip.numPixels()), 0); // Apaga o LED anterior
    strip.show();
    delay(delayMs);
    if (animacao != animacao_ant) {
      break;
    }
  }
  //strip.setPixelColor(strip.numPixels() - 1, 0);
}


void pulsingStar(uint32_t color, int delayMs) {
  int animacao_ant = animacao;
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(
                            (color >> 16 & 0xFF) * brightness / 255,
                            (color >> 8 & 0xFF) * brightness / 255,
                            (color & 0xFF) * brightness / 255
                          ));
      if (animacao != animacao_ant) {
        break;
      }
    }
    strip.show();
    delay(delayMs);
    if (animacao != animacao_ant) {
      break;
    }
  }
  for (int brightness = 255; brightness >= 0; brightness -= 5) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(
                            (color >> 16 & 0xFF) * brightness / 255,
                            (color >> 8 & 0xFF) * brightness / 255,
                            (color & 0xFF) * brightness / 255
                          ));
      if (animacao != animacao_ant) {
        break;
      }
    }
    strip.show();
    delay(delayMs);
    if (animacao != animacao_ant) {
      break;
    }
  }
}

void pulsingStar2(uint32_t color1, uint32_t color2, int delayMs) {
  int animacao_ant = animacao;
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if (random(2) == 0) {
        strip.setPixelColor(i, strip.Color(
                              (color1 >> 16 & 0xFF) * brightness / 255,
                              (color1 >> 8 & 0xFF) * brightness / 255,
                              (color1 & 0xFF) * brightness / 255
                            ));
      } else {
        strip.setPixelColor(i, strip.Color(
                              (color2 >> 16 & 0xFF) * brightness / 255,
                              (color2 >> 8 & 0xFF) * brightness / 255,
                              (color2 & 0xFF) * brightness / 255
                            ));

      }
      if (animacao != animacao_ant) {
        break;
      }
    }
    strip.show();
    delay(delayMs);
    if (animacao != animacao_ant) {
      break;
    }
  }
  for (int brightness = 255; brightness >= 0; brightness -= 5) {
    for (int i = 0; i < strip.numPixels(); i++) {
      if (random(2) == 0) {
        strip.setPixelColor(i, strip.Color(
                              (color1 >> 16 & 0xFF) * brightness / 255,
                              (color1 >> 8 & 0xFF) * brightness / 255,
                              (color1 & 0xFF) * brightness / 255
                            ));
      } else {
        strip.setPixelColor(i, strip.Color(
                              (color2 >> 16 & 0xFF) * brightness / 255,
                              (color2 >> 8 & 0xFF) * brightness / 255,
                              (color2 & 0xFF) * brightness / 255
                            ));

      }



      if (animacao != animacao_ant) {
        break;
      }
    }
    strip.show();
    delay(delayMs);
    if (animacao != animacao_ant) {
      break;
    }
  }
}
