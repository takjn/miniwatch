#define SETTINGS_ITEMS 4
char *settings_items[SETTINGS_ITEMS] = { "Display", "Sound", "Date&Time", "Back"};
const uint8_t *settings_icons[SETTINGS_ITEMS] = {monitor_bitmap, musical_note_bitmap, clock_bitmap, action_undo_note_bitmap};

uint8_t settings_current = 0;
uint8_t settings_prev = -1;

void drawSettings(void) {
  drawFrame("SETTINGS", settings_items[settings_current]);
  
  // draw icon (32 x 32 pixel)
  int aoffset = (ANIMATION_MAXSTEP - animation_progress) * (64 / ANIMATION_MAXSTEP) * (settings_current - settings_prev);
  for (int i=0;i<SETTINGS_ITEMS;i++) {
    int x = 48 + (i - settings_current) * 64 + aoffset;
    if (x < 128 && x > -64)
      u8g.drawBitmapP(x, 14, 4, 32, settings_icons[i]);
  }

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
          mode_current = MODE_SETDISPLAY;
      } 
      else if (settings_current == 1) {
          initSetsound();
          mode_current = MODE_SETSOUND;
      } 
      else if (settings_current == 2) {
          initSettime();
          mode_current = MODE_SETTIME;
      } 
      else if (settings_current == 3) {
        //  Back
        settings_current = 0;
        settings_prev = -1;
        mode_current = MODE_MENU;
      }
      
      animation_required = true;
      break;
  }
}

