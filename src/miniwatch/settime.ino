#define SETTIME_ITEMS 7
char *settime_items[SETTIME_ITEMS] = { "Year", "Month", "Day", "Hour", "Minutes", "Save", "Cancel"};

uint8_t settime_current = 0;
uint8_t settime_prev = 0;

void drawSettime(void) {
  u8g.setScale2x2();

  // title
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.drawStr(0, 0, "DATE&TIME"); 
  
  // selected item name
  u8g.setFont(u8g_font_04b_03r);
  u8g.setFontPosBottom();
  u8g.drawStr(0, 32, settime_items[settime_current]); 
  
  // draw icon (16 x 16 pixel)
  for (int i=0;i<SETTIME_ITEMS;i++) {
    int offset = (i - settime_current) * 32 + (ANIMATION_MAXSTEP - animation_progress) * 3 * (settime_current - settime_prev);
    // TODO:clipping
    
    u8g.setFont(u8g_font_freedoomr10r);
    u8g.setFontPosCenter();
    if (i == 5) {
      u8g.drawBitmapP( 24 + offset, 7, 2, 16, watch_bitmap);
    } else if (i == 6){
      u8g.drawBitmapP( 24 + offset, 7, 2, 16, gear_bitmap);
    } else {
      u8g.drawStr(26 + offset, 16, "00"); 
    }
  }
  
  // icon frame(left)
  u8g.drawLine(18,8,18,22);
  u8g.drawLine(18,8,20,8);
  u8g.drawLine(18,22,20,22);
  
  // icon frame(right)
  u8g.drawLine(46,8,46,22);
  u8g.drawLine(44,8,46,8);
  u8g.drawLine(44,22,46,22);
  
  u8g.undoScale();
  
}

void updateSettime(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  settime_prev = settime_current;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if ( settime_current < (SETTIME_ITEMS - 1) ) {
        settime_current++;
      }
      animation_required = true;
      break;
    case KEY_PREV:
      if ( settime_current > 0 ) {
        settime_current--;
      }
      animation_required = true;
      break;
    case KEY_SELECT:
      if (settime_current == 6) {
          mode_current = MODE_MENU;
      } 
      animation_required = true;
      break;
  }
}

