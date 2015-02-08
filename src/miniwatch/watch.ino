int lastsec = 0;
boolean tic = false;

void drawWatch(void) {
  char buffer[20];
  
  int header_offset = 0;
  if (transition_required) {
    header_offset = (ANIMATION_MAXSTEP - animation_progress);
  }
  
  String s = dayShortStr(weekday());
  s += " ";
  if (day()<10) {
    s += "0";
  }
  s += day();
  s += " ";
  s += monthShortStr(month());
//  s += " ";
//  s += year();
  s.toCharArray(buffer, 20);
  
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.setScale2x2();
  u8g.drawStr(0, 0 - header_offset, buffer); 
  u8g.undoScale();


  u8g.setFont(u8g_font_freedoomr25n);
  u8g.setFontPosTop();
  if (tic)
    sprintf(buffer, "%02d:%02d", hour(), minute());
  else
    sprintf(buffer, "%02d %02d", hour(), minute());
    
  u8g.drawStr(0, 23, buffer); 

  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setFontPosTop();
  u8g.setScale2x2();
  sprintf(buffer, "%02d", second());
  u8g.drawStr(47, 12, buffer);
  u8g.undoScale();
  
  // battery status
  u8g.drawFrame(0, 56 + header_offset,16, 8 + header_offset);
  u8g.drawBox(2, 58 + header_offset,12, 4 + header_offset);
  u8g.drawLine(16, 58 + header_offset,16, 61 + header_offset);
  
}


void updateWatch(void) {
  if (second() != lastsec) {
    tic = !tic;
    lastsec = second();
  }
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) {
    case KEY_SELECT:
      mode_current = MODE_MENU;
      animation_required = true;
      break;
  }
}

