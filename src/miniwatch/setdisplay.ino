#define SETDISPLAY_ITEMS 5
char *setdisplay_items[SETDISPLAY_ITEMS] = { "Brightness", "Contrast", "Screen off sec", "Flip", "Back"};

uint8_t setdisplay_current = 0;
uint8_t setdisplay_prev = -1;

void drawSetdisplay(void) {
  drawFrame("Display", setdisplay_items[setdisplay_current]);
  
  // draw icon (32 x 32 pixel)
  int aoffset = (ANIMATION_MAXSTEP - animation_progress) * (32 / ANIMATION_MAXSTEP) * (setdisplay_current - setdisplay_prev);
  for (int i=0;i<SETDISPLAY_ITEMS;i++) {
    int offset = (i - setdisplay_current) * 32 + aoffset;
    
    u8g.setFont(u8g_font_freedoomr10r);
    u8g.setFontPosCenter();
    
    char buffer[4];
    if (i < 4) {
      if (i == 0) {
        sprintf(buffer, "%03d", display_brightnesses[display_brightness]);
      }
      else if (i == 1) {
        sprintf(buffer, "%03d", display_contrasts[display_contrast]);
      }
      else if (i == 2) {
        sprintf(buffer, "%03d", (power_lcdoffdelays[power_lcdoffdelay] / 1000));
      }
      else if (i == 3) {
        if (display_flip) {
          sprintf(buffer, "%s", "YES");
        }
        else {
          sprintf(buffer, "%s", "NO");
        }
      }
      
      u8g.setScale2x2();
      u8g.drawStr(22 + offset, 16, buffer); 
      u8g.undoScale();
    }
    else {
      int x = (24 + offset) * 2;
      if (x < 128 && x > -64)
        u8g.drawBitmapP(x, 14, 4, 32, action_undo_note_bitmap);
    }
  }
  
}

void updateSetdisplay(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  setdisplay_prev = setdisplay_current;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if ( setdisplay_current < (SETDISPLAY_ITEMS - 1) ) {
        setdisplay_current++;
      }
      else {
        setdisplay_current = 0;
      }
      animation_required = true;
      break;
    case KEY_PREV:
      if ( setdisplay_current > 0 ) {
        setdisplay_current--;
      }
      else {
        setdisplay_current = (SETDISPLAY_ITEMS - 1);
      }
      animation_required = true;
      break;
    case KEY_SELECT:
      if (setdisplay_current == 0) {
        display_brightness++;
        if (display_brightness > 3)
          display_brightness = 0;
          analogWrite(DISPLAY_BACKLIGHT_PIN, display_brightnesses[display_brightness]);
      }
      else if (setdisplay_current == 1) {
        display_contrast++;
        if (display_contrast > 3)
          display_contrast = 0;
        u8g.setContrast(display_contrasts[display_contrast]);
      }
      else if (setdisplay_current == 2) {
        power_lcdoffdelay++;
        if (power_lcdoffdelay > 3)
          power_lcdoffdelay = 0;
      }
      else if (setdisplay_current == 3) {
        display_flip = !display_flip;
        if (display_flip)
          u8g.setRot180();
        else
          u8g.undoRotation();
      }
      else if (setdisplay_current == 4) {
        //  Back
        setdisplay_current = 0;
        setdisplay_prev = -1;
        mode_current = MODE_SETTINGS;
      }
      animation_required = true;
      break;
  }  
}

