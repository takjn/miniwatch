#define SETTIME_ITEMS 7
char *settime_items[SETTIME_ITEMS] = { "Year", "Month", "Day", "Hour", "Minutes", "Save", "Cancel"};

uint8_t settime_current = 0;
uint8_t settime_prev = 0;

int yy = 0;
int mm = 0;
int dd = 0;
int hh = 0;
int mi = 0;

void initSettime(void) {
  yy = year() - 2000;
  mm = month();
  dd = day();
  hh = hour();
  mi = minute();
}

void drawSettime(void) {
  drawFrame("DATE&TIME", settime_items[settime_current]);
  
  // draw icon (16 x 16 pixel)
  u8g.setScale2x2();
  for (int i=0;i<SETTIME_ITEMS;i++) {
    int offset = (i - settime_current) * 32 + (ANIMATION_MAXSTEP - animation_progress) * 3 * (settime_current - settime_prev);
    
    u8g.setFont(u8g_font_freedoomr10r);
    
    char buffer[3];
    
    u8g.setFontPosCenter();
    
    if (i < 5) {
      if (i == 0) {
        sprintf(buffer, "%02d", yy);
      }
      else if (i == 1) {
        sprintf(buffer, "%02d", mm);
      }
      else if (i == 2) {
        sprintf(buffer, "%02d", dd);
      }
      else if (i == 3) {
        sprintf(buffer, "%02d", hh);
      }
      else if (i == 4) {
        sprintf(buffer, "%02d", mi);
      }
      u8g.drawStr(26 + offset, 16, buffer); 
    }
    else {
      if (i == 5) {
        u8g.drawBitmapP( 24 + offset, 7, 2, 16, watch_bitmap);
      }
      else if (i == 6){
        u8g.drawBitmapP( 24 + offset, 7, 2, 16, gear_bitmap);
      }
    }
  }
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
      else {
        settime_current = 0;
      }
      animation_required = true;
      break;
    case KEY_PREV:
      if ( settime_current > 0 ) {
        settime_current--;
      }
      else {
        settime_current = (SETTIME_ITEMS - 1);
      }

      animation_required = true;
      break;
    case KEY_SELECT:
      if (settime_current == 0) {
        yy++;
      }
      else if (settime_current == 1) {
        mm++;
        if (mm > 12)
          mm = 1;
      }
      else if (settime_current == 2) {
        dd++;
        if (mm == 2) {
          if (dd > 28)
            dd = 1;
        }
        else if (mm == 4 || mm == 6 || mm == 9) {
          if (dd > 30)
            dd = 1;
        }
        else {
          if (dd > 31)
            dd = 1;
        }
      }
      else if (settime_current == 3) {
        hh++;
        if (hh > 23)
          hh = 0;
      }
      else if (settime_current == 4) {
        mi++;
        if (mi > 59)
          mi = 0;
      }
      else if (settime_current == 5) {
        // Save
        setTime(hh, mi, 0, dd, mm, yy);
        settime_current = 0;
        settime_prev = 0;
        mode_current = MODE_SETTINGS;
      }
      else if (settime_current == 6) {
        //  Cancel
        settime_current = 0;
        settime_prev = 0;
        mode_current = MODE_SETTINGS;
      }
      
      animation_required = true;
      break;
  }
}

