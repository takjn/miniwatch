void beep(void) {
    analogWrite(BUZZER_PIN, buzzer_volumes[buzzer_volume]);
    delay(BUZZER_DELAY); 
    analogWrite(BUZZER_PIN, 0);
}

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

void checkPowerDownRequired(void) {
  unsigned long  duration = millis() - last_millis;
  if (power_sleepdelay > 0 && duration > power_sleepdelay) {
    powerstate = 2;
    
    // sleep
    u8g.firstPage();
    do  {
      // clear screen
    } while( u8g.nextPage() ); 
    u8g.sleepOn();    
    attachInterrupt(1,wakeup,FALLING);
    noInterrupts();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    interrupts();
    sleep_cpu();
    
    // resume
    sleep_disable();
    detachInterrupt(1);
    u8g.sleepOff();
  }
  else if (power_lcdoffdelays[power_lcdoffdelay] > 0 && duration > power_lcdoffdelays[power_lcdoffdelay]) {
    powerstate = 1;
    analogWrite(DISPLAY_BACKLIGHT_PIN, 0);
  }
  else {
    // set lcd backlight
    analogWrite(DISPLAY_BACKLIGHT_PIN, display_brightnesses[display_brightness]);
  }
}

void wakeup() {
  last_millis = millis();
  powerstate = 0;
  animation_required = true;
}

void drawFrame(char *title, char *item) {
  
  int header_offset = 0;
  if (transition_required) {
    header_offset = (ANIMATION_MAXSTEP - animation_progress);
  }
  
  int footer_offset = 0;
  if (animation_required) {
    footer_offset = (ANIMATION_MAXSTEP - animation_progress);
  }

  u8g.setScale2x2();

  // title
  u8g.setFont(u8g_font_04b_03br);
  u8g.setFontPosTop();
  u8g.drawStr(0, 0 - header_offset, title); 
  
  // selected item name
  u8g.setFont(u8g_font_04b_03r);
  u8g.setFontPosBottom();
  u8g.drawStr(0, 32 + footer_offset, item); 
 
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

