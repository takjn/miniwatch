
#define MENU_ITEMS 3
char *menu_items[MENU_ITEMS] = { "Watch", "Stopwatch", "Settings" };
const uint8_t *menu_icons[MENU_ITEMS] = {clock_bitmap, timer_bitmap, cog_bitmap};

uint8_t menu_current = 0;
uint8_t menu_prev = -1;

void drawMenu(void) {
  drawFrame("MENU", menu_items[menu_current]);
  
  // draw icon (32 x 32 pixel)
  int aoffset = (ANIMATION_MAXSTEP - animation_progress) * (64 / ANIMATION_MAXSTEP) * (menu_current - menu_prev);
  for (int i=0;i<MENU_ITEMS;i++) {
    int x = 48 + (i - menu_current) * 64 + aoffset;
    if (x < 128 && x > -64)
      u8g.drawBitmapP(x, 14, 4, 32, menu_icons[i]);
  }

}

void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  menu_prev = menu_current;
  
  switch ( uiKeyCode ) {
    case KEY_NEXT:
      if ( menu_current < (MENU_ITEMS - 1) ) {
        menu_current++;
      }
      else {
        menu_current = 0;
      }
      animation_required = true;
      break;
    case KEY_PREV:
      if ( menu_current > 0 ) {
        menu_current--;
      }
      else {
        menu_current = (MENU_ITEMS - 1);
      }
      animation_required = true;
      break;
    case KEY_SELECT:
      if (menu_current == 0) {
          mode_current = MODE_TIME;
      } 
      else if (menu_current == 1) {
          mode_current = MODE_STOPWATCH;
      }
      else if (menu_current == 2) {
          mode_current = MODE_SETTINGS;
      }
      
      menu_current = 0;
      menu_prev = -1;
      animation_required = true;
      break;
  }
}

