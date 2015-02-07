void drawFrame(char *title, char *item) {
  u8g.setScale2x2();

  // title
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.drawStr(0, 0, title); 
  
  // selected item name
  u8g.setFont(u8g_font_04b_03r);
  u8g.setFontPosBottom();
  u8g.drawStr(0, 32, item); 
  
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

