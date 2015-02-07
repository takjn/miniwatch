boolean checkAnimationRequired(void) {
  animation_progress += ANIMATION_STEP;
  if (animation_progress > ANIMATION_MAXSTEP) {
    animation_required = false;
  }
  
  return animation_required;
}

boolean checkTransitionRequired(void) {
  transition_required = false;
  if (mode_current != mode_prev) {
    transition_required = true;
    mode_prev = mode_current;
  }
  
  return transition_required;
}

void drawFrame(char *title, char *item) {
  
  transition_offset = 0;
  if (transition_required) {
    transition_offset = (ANIMATION_MAXSTEP - animation_progress) * 3;
  }

  u8g.setScale2x2();

  // title
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.drawStr(0, 0, title); 
  
  // selected item name
  u8g.setFont(u8g_font_04b_03r);
  u8g.setFontPosBottom();
  u8g.drawStr(0, 32 + transition_offset, item); 
  
  // icon frame(left)
  u8g.drawLine(18, 8, 18, 22);
  u8g.drawLine(18, 8, 20, 8);
  u8g.drawLine(18, 22, 20, 22);
  
  // icon frame(right)
  u8g.drawLine(46, 8, 46, 22);
  u8g.drawLine(44, 8, 46, 8);
  u8g.drawLine(44, 22, 46, 22);
  
  u8g.undoScale();
}

