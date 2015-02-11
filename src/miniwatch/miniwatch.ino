#include <avr/sleep.h>
#include <Time.h>
#include "U8glib.h"

// U8GLIB constructor
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_MINI12864 u8g(9, 10, 13, 11, 12);

// settings for I/O pins
// *key pins are high input. (on -> 5v/3.3v, off -> GND)
// *so key pins need to be connected to GND with pull down resistors (10kohm) 
#define BUZZER_PIN 5              // pin for buzzer (need pwm)
#define DISPLAY_BACKLIGHT_PIN  6  // pin for lcd backlight (need pwm)
#define KEY_PIN_PREV 4            // pin for previous button
#define KEY_PIN_NEXT 3            // pin for next button
#define KEY_PIN_SELECT 2          // pin for select button

// settings for buzzer
#define BUZZER_DELAY 15                        // millisec
const int buzzer_volumes[4] = { 0, 1, 5, 10 };  // 4 steps volume (0=silence, 1024=max)
int buzzer_volume = 1;                         // initial volume step

// settings for animation
#define ANIMATION_STEP 2      // animation smoothness (1=smooth, 10=no animation)
#define ANIMATION_MAXSTEP 10  // animation max step

// variables for animation
boolean animation_required = true;
uint8_t animation_progress = 0;    // animation progress (0=start, 10=end)

// variables for transition
boolean transition_required = true;

// settings for display
const int display_contrasts[4] = { 125, 150, 175, 200 };  // 4 steps contrast
int display_contrast = 1;
boolean display_flip = false;
const int display_brightnesses[4] = { 0, 100, 150, 200 };   // 4 steps brightness (0=none, 255=max)
int display_brightness = 3;

// variables for watch
time_t last_time;

// settings for power saving
//const unsigned long power_sleepdelay = 600000;  // powerdown (millisec, 0=never powerdown)
const unsigned long power_sleepdelay = 0;  // powerdown (millisec, 0=never powerdown)
const unsigned long power_lcdoffdelays[4] = {0, 5000, 10000, 30000};  // lcd off (millisec, 0=always on)
int power_lcdoffdelay = 0;
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
uint8_t mode_current = MODE_MENU;
uint8_t mode_prev = MODE_MENU;

void setup(void) {
  // set contrast
  u8g.setContrast(display_contrasts[display_contrast]);
  
  setTime(0, 0, 0, 1, 1, 2015);
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
//      animation_progress = 0;
//      animation_required = true;
    }
    
    // show main menu 
    updateWatch();
  }
  
  // check transition
  checkTransitionRequired();
  
  // check power down
  checkPowerDownRequired();
  
  delay(10);
}

