boolean stopwatch_started = false;
unsigned long stopwatch_startmillis = 0;
unsigned long stopwatch_duration = 0;

void drawStopwatch(void) {
  char buffer[20];
  
  int header_offset = 0;
  if (transition_required) {
    header_offset = (ANIMATION_MAXSTEP - animation_progress);
  }
  
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.setScale2x2();
  u8g.drawStr(0, 0 - header_offset, "STOPWATCH"); 
  u8g.undoScale();
  
  if (stopwatch_started) {
    stopwatch_duration = millis() - stopwatch_startmillis;
  }
  
  int ms = stopwatch_duration % 1000;
  unsigned long _duration = (stopwatch_duration - ms) / 1000;
  int mi = _duration / 60;
  int sec = _duration % 60;
  
  if (mi > 60) {
    stopwatch_started = false;
  }
  
  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setFontPosTop();
  u8g.setScale2x2();
  sprintf(buffer, "%02d:%02d %03d", mi, sec, ms);
  u8g.drawStr(0, 12, buffer);
  u8g.undoScale();
  
  // battery status
  u8g.drawFrame(0, 56 + header_offset,16, 8 + header_offset);
  u8g.drawBox(2, 58 + header_offset,12, 4 + header_offset);
  u8g.drawLine(16, 58 + header_offset,16, 61 + header_offset);
}

void updateStopwatch(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      stopwatch_started = !stopwatch_started;
      if (stopwatch_startmillis == 0) {
        stopwatch_startmillis = millis();
      }
      break;
    case KEY_PREV:
      if (!stopwatch_started) {
        stopwatch_duration = 0;
        stopwatch_startmillis = 0;
      }
      break;
    case KEY_SELECT:
      //  Backardu
      settings_current = 0;
      settings_prev = -1;
      mode_current = MODE_MENU;
      animation_required = true;
      break;
      
  }
}

