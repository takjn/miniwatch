
#define MENU_ITEMS 2
char *menu_items[MENU_ITEMS] = { "Watch", "Settings" };
const uint8_t *menu_icons[MENU_ITEMS] = {clock_bitmap, cog_bitmap};

uint8_t menu_current = 0;
uint8_t menu_prev = 0;

void drawMenu(void) {
  drawFrame("MENU", menu_items[menu_current]);
  
  // draw icon (16 x 16 pixel)
  u8g.setScale2x2();
  for (int i=0;i<MENU_ITEMS;i++) {
    int offset = (i - menu_current) * 32 + (ANIMATION_MAXSTEP - animation_progress) * 3 * (menu_current - menu_prev);
    u8g.drawBitmapP( 24 + offset, 7, 2, 16, menu_icons[i]);
  }
  u8g.undoScale();
  
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
          mode_current = MODE_SETTINGS;
      }
      
      animation_required = true;
      break;
  }
}

