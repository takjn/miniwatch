const uint8_t rook_bitmap[] U8G_PROGMEM = {
  0x00,0x00,         // 00000000 
  0x55,0x55,         // 01010101
  0x7f,0x7f,         // 01111111
  0x3e,0x3e,         // 00111110
  0x3e,0x3e,         // 00111110 
  0x3e,0x3e,         // 00111110
  0x3e,0x3e,         // 00111110 
  0x7f,0x7f,         // 01111111
  0x00,0x00,         // 00000000 
  0x55,0x55,         // 01010101
  0x7f,0x7f,         // 01111111
  0x3e,0x3e,         // 00111110
  0x3e,0x3e,         // 00111110 
  0x3e,0x3e,         // 00111110
  0x3e,0x3e,         // 00111110 
  0x7f,0x7f          // 01111111
};

const uint8_t rook2_bitmap[] U8G_PROGMEM = {
  0x00,0x00,         // 00000000 
  0x55,0x00,         // 01010101
  0x7f,0x00,         // 01111111
  0x3e,0x00,         // 00111110
  0x3e,0x00,         // 00111110 
  0x3e,0x00,         // 00111110
  0x3e,0x00,         // 00111110 
  0x7f,0x00,         // 01111111
  0x00,0x00,         // 00000000 
  0x00,0x55,         // 01010101
  0x00,0x7f,         // 01111111
  0x00,0x3e,         // 00111110
  0x00,0x3e,         // 00111110 
  0x00,0x3e,         // 00111110
  0x00,0x3e,         // 00111110 
  0x00,0x7f          // 01111111
};


#define MENU_ITEMS 2
char *menu_strings[MENU_ITEMS] = { "Watch", "Setting" };
const uint8_t *icons[MENU_ITEMS] = {rook_bitmap, rook2_bitmap};

uint8_t menu_current = 0;
uint8_t menu_prev = 0;

void drawMenu(void) {
  u8g.setScale2x2();

  // title
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.drawStr(0, 0, "MENU"); 
  
  // selected item name
  u8g.setFont(u8g_font_04b_03r);
  u8g.setFontPosBottom();
  u8g.drawStr(0, 32, menu_strings[menu_current]); 
  
  // draw icon (16 x 16 pixel)
  for (int i=0;i<MENU_ITEMS;i++) {
    int offset = (i - menu_current) * 32 + (10 - animation_percent) * 3 * (menu_current - menu_prev);
    // TODO:clipping
    u8g.drawBitmapP( 24 + offset, 7, 2, 16, icons[i]);
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
      
//      last_key_code = KEY_NONE;
      break;
  }
}

