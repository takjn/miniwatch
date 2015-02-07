#include <Time.h>
#include "U8glib.h"

// U8GLIB constructor
// devices with all constructor calls is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_MINI12864 u8g(9, 10, 13, 11);

#define ANIMATION_STEP 2;  // animation smoothness (1=smooth, 10=no animation)

time_t last_time;

boolean animation_required = true;
uint8_t animation_percent = 0;

// mode
#define MODE_TIME 0
#define MODE_MENU 1
//uint8_t mode_current = MODE_TIME;
uint8_t mode_current = MODE_MENU;


void setup(void) {
  //Serial.begin(9600);
  
  // flip screen, if required
  // u8g.setRot180();
  //u8g.setContrast(0);
  
  setTime(0, 0, 0, 1, 1, 2015);
  last_time = now();

  keySetup();                    // setup key detection and debounce algorithm

}

void loop(void) {
  getKeyPress();
  
  if (mode_current == MODE_MENU) {
    
    // animation loop
    animation_percent = 0;
    while (animation_required){
      
      // picture loop
      u8g.firstPage();
      do  {
        drawMenu();
      } while( u8g.nextPage() ); 
      
      animation_percent += ANIMATION_STEP;
      if (animation_percent > 10) {
        animation_required = false;
      }
    } 
    
    // update menu
    updateMenu();
  }
  else {
    if ( last_time < now() ) {
      // picture loop
      u8g.firstPage(); 
      do {
        drawWatch();
      } while( u8g.nextPage() );
      
      last_time = now();
    }
    
    // show main menu 
    watchMenu();
  }
  
  delay(10);  
}

