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
  
  pinMode(KEY_PREV, INPUT);           // set pin to input
  digitalWrite(KEY_PREV, HIGH);       // turn on pullup resistors
  pinMode(KEY_NEXT, INPUT);           // set pin to input
  digitalWrite(KEY_NEXT, HIGH);       // turn on pullup resistors
  pinMode(KEY_SELECT, INPUT);           // set pin to input
  digitalWrite(KEY_SELECT, HIGH);       // turn on pullup resistors
}

void getKeyPress(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(KEY_PREV) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(KEY_NEXT) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(KEY_SELECT) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else 
    uiKeyCodeFirst = KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst ) {
    uiKeyCode = uiKeyCodeFirst;
    
    // buzzer
    if ( uiKeyCode != KEY_NONE && last_key_code != uiKeyCode && mode_current != MODE_SETSOUND) {
      beep();
    }
  }
  else {
    uiKeyCode = KEY_NONE;
  }
}

