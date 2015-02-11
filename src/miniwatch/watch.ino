int lastsec = 0;
boolean tic = false;

void drawWatch(void) {
  char buffer[20];
  
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
  
  drawWatchFrame(buffer);
  
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

