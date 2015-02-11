#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

uint8_t last_key_code = KEY_NONE;

void keySetup(void) {
  // configure input keys 
  pinMode(KEY_PIN_PREV, INPUT_PULLUP);      // set pin to input
  pinMode(KEY_PIN_NEXT, INPUT_PULLUP);           // set pin to input
  pinMode(KEY_PIN_SELECT, INPUT_PULLUP);         // set pin to input
}

void getKeyPress(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(KEY_PIN_PREV) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(KEY_PIN_NEXT) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(KEY_PIN_SELECT) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else 
    uiKeyCodeFirst = KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst ) {
    uiKeyCode = uiKeyCodeFirst;
    
    // buzzer
    if ( uiKeyCode != KEY_NONE && last_key_code != uiKeyCode) {
      // for power saving
      last_millis = millis();
      powerstate = 0;
      if (mode_current != MODE_SETSOUND)
        beep();
    }
    
  }
  else {
    uiKeyCode = KEY_NONE;
  }
}

