#define SETSOUND_ITEMS 3
char *setsound_items[SETSOUND_ITEMS] = { "Volume", "Save", "Cancel"};

uint8_t setsound_current = 0;
uint8_t setsound_prev = -1;

int volume = 0;

void initSetsound(void) {
  volume = buzzer_volume;
}

void drawSetsound(void) {
  drawFrame("SOUND", setsound_items[setsound_current]);
  
  // draw icon (32 x 32 pixel)
  int aoffset = (ANIMATION_MAXSTEP - animation_progress) * (32 / ANIMATION_MAXSTEP) * (setsound_current - setsound_prev);
  for (int i=0;i<SETSOUND_ITEMS;i++) {
    int offset = (i - setsound_current) * 32 + aoffset;
    
    u8g.setFont(u8g_font_freedoomr10r);
    u8g.setFontPosCenter();
    
    char buffer[3];
    if (i < 1) {
      if (i == 0) {
        sprintf(buffer, "%02d", buzzer_volume);
      }
      
      u8g.setScale2x2();
      u8g.drawStr(26 + offset, 16, buffer); 
      u8g.undoScale();
    }
    else {
      if (i == 1) {
        u8g.drawBitmapP((24 + offset) * 2, 14, 4, 32, check_bitmap);
      }
      else if (i == 2){
        u8g.drawBitmapP((24 + offset) * 2, 14, 4, 32, action_undo_note_bitmap);
      }
    }
  }
  
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
      beep();
      animation_required = true;
      break;
    case KEY_PREV:
      if ( setsound_current > 0 ) {
        setsound_current--;
      }
      else {
        setsound_current = (SETSOUND_ITEMS - 1);
      }

      beep();
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
        setsound_prev = -1;
        mode_current = MODE_SETTINGS;
      }
      else if (setsound_current == 2) {
        //  Cancel
        buzzer_volume = volume;
        setsound_current = 0;
        setsound_prev = -1;
        mode_current = MODE_SETTINGS;
      }
      
      beep();
      animation_required = true;
      break;
  }  
}

