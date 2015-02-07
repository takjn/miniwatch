#define SETTINGS_ITEMS 3
char *settings_items[SETTINGS_ITEMS] = { "Date&Time", "Sound", "Back"};
const uint8_t *settings_icons[SETTINGS_ITEMS] = {clock_bitmap, musical_note_bitmap, action_undo_note_bitmap};

uint8_t settings_current = 0;
uint8_t settings_prev = 0;

void drawSettings(void) {
  drawFrame("SETTINGS", settings_items[settings_current]);
  
  // draw icon (16 x 16 pixel)
  u8g.setScale2x2();
  for (int i=0;i<SETTINGS_ITEMS;i++) {
    int offset = (i - settings_current) * 32 + (ANIMATION_MAXSTEP - animation_progress) * 3 * (settings_current - settings_prev);
    u8g.drawBitmapP( 24 + offset, 7, 2, 16, settings_icons[i]);
  }
  u8g.undoScale();
  
}

void updateSettings(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  settings_prev = settings_current;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if ( settings_current < (SETTINGS_ITEMS - 1) ) {
        settings_current++;
      }
      else {
        settings_current = 0;
      }
      animation_required = true;
      break;
    case KEY_PREV:
      if ( settings_current > 0 ) {
        settings_current--;
      }
      else {
        settings_current = (SETTINGS_ITEMS - 1);
      }

      animation_required = true;
      break;
    case KEY_SELECT:
      if (settings_current == 0) {
          initSettime();
          mode_current = MODE_SETTIME;
      } 
      else if (settings_current == 1) {
          initSetsound();
          mode_current = MODE_SETSOUND;
      } 
      else if (settings_current == 2) {
        //  Back
        settings_current = 0;
        settings_prev = 0;
        mode_current = MODE_MENU;
      }
      
      animation_required = true;
      break;
  }
}

