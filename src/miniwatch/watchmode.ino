
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
  
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.setScale2x2();
  u8g.drawStr(0, 0, buffer); 
  u8g.undoScale();


  u8g.setFont(u8g_font_freedoomr25n);
  u8g.setFontPosTop();
  sprintf(buffer, "%02d:%02d", hour(), minute());
  u8g.drawStr(0, 23, buffer); 

  u8g.setFont(u8g_font_freedoomr10r);
  u8g.setFontPosTop();
  u8g.setScale2x2();
  sprintf(buffer, "%02d", second());
  u8g.drawStr(47, 12, buffer);
  u8g.undoScale();
  
  // battery status
  u8g.drawFrame(0, 56 ,16, 8);
  u8g.drawBox(2, 58 ,12, 4);
  u8g.drawLine(16, 58 ,16, 61);
  
}


void watchMenu(void) {
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

