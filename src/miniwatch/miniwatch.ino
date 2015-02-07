#include <Time.h>
#include "U8glib.h"

// U8GLIB constructor
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_MINI12864 u8g(9, 10, 13, 11, 12);

// setting for buzzer
#define BUZZER_PIN 5                           // pin for buzzer (need pwm)
const int buzzer_volumes[4] = { 0, 1, 10, 50 };  // 4 steps volume (0=silence, 1024=max)
int buzzer_volume = 1;                         // initial volume step

// setting for animation
#define ANIMATION_STEP 2      // animation smoothness (1=smooth, 10=no animation)
#define ANIMATION_MAXSTEP 10  // animation max step

// variables for animation
boolean animation_required = true;
uint8_t animation_progress = 0;    // animation progress (0=start, 10=end)

// variables for transition
boolean transition_required = true;

// variables for watch
time_t last_time;

// mode
#define MODE_TIME 0      // Watch
#define MODE_MENU 1      // Menu
#define MODE_SETTINGS 2  // Settings
#define MODE_SETTIME 3   // Date&Time
#define MODE_SETSOUND 4   // Date&Time
uint8_t mode_current = MODE_MENU;
uint8_t mode_prev = MODE_MENU;

void setup(void) {
  // flip screen, if required
  // u8g.setRot180();
  
  // set contrast, if required
  //u8g.setContrast(0);
  
  setTime(0, 0, 0, 1, 1, 2015);
  last_time = now();

  keySetup();  // setup key detection and debounce algorithm
}

void loop(void) {
  getKeyPress();
  
  if (mode_current == MODE_MENU) {
    // animation loop
    animation_progress = 0;
    while (checkAnimationRequired()) {      
      // picture loop
      u8g.firstPage();
      do  {
        drawMenu();
      } while( u8g.nextPage() ); 
    } 
    
    // update menu
    updateMenu();
  }
  else if (mode_current == MODE_SETTINGS) {
    // animation loop
    animation_progress = 0;
    while (checkAnimationRequired()) {      
      // picture loop
      u8g.firstPage();
      do  {
        drawSettings();
      } while( u8g.nextPage() ); 
    }
    
    // update menu
    updateSettings();
  }
  else if (mode_current == MODE_SETTIME) {
    // animation loop
    animation_progress = 0;
    while (checkAnimationRequired()) {      
      // picture loop
      u8g.firstPage();
      do  {
        drawSettime();
      } while( u8g.nextPage() ); 
    }
    
    // update menu
    updateSettime();
  }
  else if (mode_current == MODE_SETSOUND) {
    // animation loop
    animation_progress = 0;
    while (checkAnimationRequired()) {      
      // picture loop
      u8g.firstPage();
      do  {
        drawSetsound();
      } while( u8g.nextPage() ); 
    }
    
    // update menu
    updateSetsound();
  }
  else {
    if ( last_time < now() ) {
      while (checkAnimationRequired()) {      
        // picture loop
        u8g.firstPage(); 
        do {
          drawWatch();
        } while( u8g.nextPage() );
      }
      
      last_time = now();
      animation_progress = 0;
      animation_required = true;
    }
    
    // show main menu 
    updateWatch();
  }
  
  //  check transition
  checkTransitionRequired();
  
  delay(10);
}

