#include <avr/sleep.h>
#include <Time.h>
#include "U8glib.h"


// U8GLIB constructor
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9, 12);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 12
//U8GLIB_SSD1306_128X64 u8g(10, 9, 12);		// HW SPI Com: CS = 10, A0 = 9, RST = 12 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_MINI12864 u8g(13, 11, 10, 9 ,12);
//U8GLIB_MINI12864 u8g(10, 14 ,12);
U8GLIB_NHD_C12864 u8g(13, 11, 10, 14, 12);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8


// settings for I/O pins
#define BUZZER_PIN 5              // pin for buzzer (need pwm)
#define DISPLAY_BACKLIGHT_PIN 6   // pin for lcd backlight (need pwm)
#define KEY_PIN_PREV 4            // pin for previous button
#define KEY_PIN_NEXT 3            // pin for next button
#define KEY_PIN_SELECT 2          // pin for select button

#define RTC4534_SCK 15            // pin for RTC4534 SCK
#define RTC4534_DATA 16           // pin for RTC4534 DATA
#define RTC4534_WR 17             // pin for RTC4534 RW
#define RTC4534_CE 9              // pin for RTC4534 CE

// settings for buzzer
const int buzzer_volumes[4] = { 0, 1, 5, 15 };  // 4 steps volume (0=silence)
int buzzer_volume = 3;                         // initial volume step

// settings for animation
#define ANIMATION_STEP 2      // animation smoothness (1=smooth, 10=no animation)
#define ANIMATION_MAXSTEP 6  // animation max step

// variables for animation
boolean animation_required = true;
uint8_t animation_progress = 0;    // animation progress (0=start, 10=end)

// variables for transition
boolean transition_required = true;

// settings for display
//const int display_contrasts[4] = { 150, 175, 200, 255 };  // 4 steps contrast
const int display_contrasts[4] = { 0, 10, 20, 30 };  // 4 steps contrast
int display_contrast = 0;
boolean display_flip = false;
const int display_brightnesses[4] = { 0, 80, 160, 255 };   // 4 steps brightness (0=none, 255=max)
int display_brightness = 3;

// variables for watch
time_t last_time;

// settings for power saving
const float power_voltage_max = 3.7;
const float power_voltage_drop = 0.6;
#define power_sleepdelay power_lcdoffdelays[power_lcdoffdelay] * 2    // powerdown (millisec, 0=never powerdown)
const unsigned long power_lcdoffdelays[4] = {0, 5000, 10000, 30000};  // lcd off (millisec, 0=always on)
int power_lcdoffdelay = 2;
unsigned long last_millis = 0;  // mills for power save mode
int powerstate = 0;  // (0=normal, 1=backlight off, 2=powerdown)

// mode
#define MODE_TIME 0        // Watch
#define MODE_MENU 1        // Menu
#define MODE_SETTINGS 2    // Settings
#define MODE_SETTIME 3     // Date&Time
#define MODE_SETSOUND 4    // Sound
#define MODE_STOPWATCH 5   // Stopwatch
#define MODE_SETDISPLAY 6  // Display
uint8_t mode_current = MODE_TIME;
uint8_t mode_prev = MODE_TIME;

void setup(void) {
//  Serial.begin(19200);
  pinMode(BUZZER_PIN,OUTPUT);
  // set contrast
  u8g.setContrast(display_contrasts[display_contrast]);
  analogWrite(DISPLAY_BACKLIGHT_PIN, display_brightnesses[display_brightness]);
  
  // setup rtc4534
  setupRtc();
  last_time = now();
  last_millis = millis();

  keySetup();  // setup key detection and debounce algorithm
}

void animationLoop(void (*draw)(void), void (*keyhandler)(void)) {
    // animation loop
    animation_progress = 0;
    while (checkAnimationRequired()) {      
      // picture loop
      u8g.firstPage();
      do  {
        draw();
      } while( u8g.nextPage() ); 
    } 
    
    // update menu
    keyhandler();
} 


void loop(void) {
  getKeyPress();
  
  if (mode_current == MODE_MENU) {
    animationLoop(drawMenu, updateMenu);
  }
  else if (mode_current == MODE_SETTINGS) {
    animationLoop(drawSettings, updateSettings);
  }
  else if (mode_current == MODE_SETTIME) {
    animationLoop(drawSettime, updateSettime);
  }
  else if (mode_current == MODE_SETDISPLAY) {
    animationLoop(drawSetdisplay, updateSetdisplay);
  }
  else if (mode_current == MODE_SETSOUND) {
    animationLoop(drawSetsound, updateSetsound);
  }
  else if (mode_current == MODE_STOPWATCH) {
    if (animation_required) {
      while (checkAnimationRequired()) {      
        u8g.firstPage(); 
        do {
          drawStopwatch();
        } while( u8g.nextPage() );
      }
    }
    else {
        u8g.firstPage(); 
        do {
          drawStopwatch();
        } while( u8g.nextPage() );
    }
    
    updateStopwatch();
  }
  else {
    if ( last_time < now() ) {
      setTimeFromRtc();
      if (animation_required) {
        while (checkAnimationRequired()) {      
          u8g.firstPage(); 
          do {
            drawWatch();
          } while( u8g.nextPage() );
        }
      }
      else {
          u8g.firstPage(); 
          do {
            drawWatch();
          } while( u8g.nextPage() );
      }

      last_time = now();
    }
    
    // show main menu 
    updateWatch();
  }
  
  // check transition
  checkTransitionRequired();
  
  // check power down
  checkPowerDownRequired();
  
  if (mode_current != MODE_STOPWATCH) {
    delay(50);
  }
}

