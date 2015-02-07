#define SETSOUND_ITEMS 3
const char *setsound_items[SETSOUND_ITEMS] = { "Volume", "Save", "Cancel"};

uint8_t setsound_current = 0;
uint8_t setsound_prev = 0;

int volume = 0;

void initSetsound(void) {
  volume = buzzer_volume;
}

void drawSetsound(void) {
  u8g.setScale2x2();

  // title
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.drawStr(0, 0, "SOUND"); 
  
  // selected item name
  u8g.setFont(u8g_font_04b_03r);
  u8g.setFontPosBottom();
  u8g.drawStr(0, 32, setsound_items[setsound_current]); 
  
  // draw icon (16 x 16 pixel)
  for (int i=0;i<SETSOUND_ITEMS;i++) {
    int offset = (i - setsound_current) * 32 + (ANIMATION_MAXSTEP - animation_progress) * 3 * (setsound_current - setsound_prev);
    
    u8g.setFont(u8g_font_freedoomr10r);
    u8g.setFontPosCenter();
    
    char buffer[3];
    if (i < 1) {
      if (i == 0) {
        sprintf(buffer, "%02d", buzzer_volume);
      }
      
      u8g.drawStr(26 + offset, 16, buffer); 
    }
    else {
      if (i == 1) {
        u8g.drawBitmapP( 24 + offset, 7, 2, 16, watch_bitmap);
      }
      else if (i == 2){
        u8g.drawBitmapP( 24 + offset, 7, 2, 16, gear_bitmap);
      }
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

void updateSetsound(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  setsound_prev = setsound_current;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if ( setsound_current < (SETSOUND_ITEMS - 1) ) {
        setsound_current++;
      }
      else {
        setsound_current = 0;
      }
      animation_required = true;
      break;
    case KEY_PREV:
      if ( setsound_current > 0 ) {
        setsound_current--;
      }
      else {
        setsound_current = (SETSOUND_ITEMS - 1);
      }

      animation_required = true;
      break;
    case KEY_SELECT:
      if (setsound_current == 0) {
        buzzer_volume++;
        if (buzzer_volume > 3)
          buzzer_volume = 0;
      }
      else if (setsound_current == 1) {
        // Save
        setsound_current = 0;
        setsound_prev = 0;
        mode_current = MODE_MENU;
      }
      else if (setsound_current == 2) {
        //  Cancel
        buzzer_volume = volume;
        setsound_current = 0;
        setsound_prev = 0;
        mode_current = MODE_MENU;
      }
      
      animation_required = true;
      break;
  }
}

