tmElements_t rtc_datetime;

void setupRtc()
{
  pinMode(RTC4534_SCK, OUTPUT);  // SCK
  pinMode(RTC4534_DATA, OUTPUT); // DATA (I/O)
  pinMode(RTC4534_WR, OUTPUT);   // WR
  pinMode(RTC4534_CE, OUTPUT);   // CE

  if (getRTC(rtc_datetime)) {
    setRTC(14, 3, 6, 6, 21, 0, 0);
  }

}

void setTimeFromRtc() {
  getRTC(rtc_datetime);
  setTime(rtc_datetime.Hour, rtc_datetime.Minute, rtc_datetime.Second, rtc_datetime.Day, rtc_datetime.Month, rtc_datetime.Year);
}

byte getRTC(tmElements_t &dt) {

  pinMode(RTC4534_DATA, INPUT);    // DATA direction
  digitalWrite(RTC4534_WR, LOW);   // RTC read modeWR_R;
  digitalWrite(RTC4534_CE, HIGH);  // Chip Enable : true
  delayMicroseconds(1);

  dt.Second  = _getBits(4) + (_getBits(3) * 10);
  byte fdt = _getBits(1); // FDT bit

  dt.Minute  = _getBits(4) + (_getBits(3) * 10);
  _getBits(1); // User bit : b0

  dt.Hour = _getBits(4) + (_getBits(2) * 10);
  _getBits(1); // User bit : b1
  _getBits(1); // User bit : b2

  _getBits(3); // week
  _getBits(1); // User bit : b3

  dt.Day  = _getBits(4) + (_getBits(2) * 10);
  _getBits(1); // User bit : b4
  _getBits(1); // User bit : b5

  dt.Month  = _getBits(4) + (_getBits(1) * 10);
  _getBits(1); // User bit : b6
  _getBits(1); // User bit : b7
  _getBits(1); // TM bit

  dt.Year = _getBits(4) + (_getBits(4) * 10);

  digitalWrite(RTC4534_CE, LOW);  // Chip Enable : false

  return fdt;
}

void setRTC(byte yy, byte MM, byte dd, byte week, byte hh, byte minutes, byte seconds) {

  pinMode(RTC4534_DATA, OUTPUT);   // DATA direction
  digitalWrite(RTC4534_WR, HIGH);  // RTC write mode
  digitalWrite(RTC4534_CE, HIGH);  // Chip Enable : true
  delayMicroseconds(1);

  _setBits(4, seconds % 10);
  _setBits(3, seconds / 10);
  _setBits(1, 0);  // FDT bit

  _setBits(4, minutes % 10);
  _setBits(3, minutes / 10);
  _setBits(1, 0); // User bit : b0

  _setBits(4, hh % 10);
  _setBits(2, hh / 10);
  _setBits(1, 0); // User bit : b1
  _setBits(1, 0); // User bit : b2

  _setBits(3, week);
  _setBits(1, 0); // User bit : b3

  _setBits(4, dd % 10);
  _setBits(2, dd / 10);
  _setBits(1, 0); // User bit : b4
  _setBits(1, 0); // User bit : b5

  _setBits(4, MM % 10);
  _setBits(1, MM / 10);
  _setBits(1, 0); // User bit : b6
  _setBits(1, 0); // User bit : b7
  _setBits(1, 0); // TM bit

  _setBits(4, yy % 10);
  _setBits(4, yy / 10);

  digitalWrite(RTC4534_CE, LOW);  // Chip Enable : false
}

byte _getBits(int count) {
  byte i, data = 0;
  byte b = 0;

  for(i = 0; i < count; i++) {
    // Generate Serial Clock Pulse
    digitalWrite(RTC4534_SCK, HIGH);
    digitalWrite(RTC4534_SCK, LOW);

    b = digitalRead(RTC4534_DATA);
    data |= (b<<i);
  }

  return data;
}

void _setBits(int length, byte data) {
  byte i;

  for(i = 0; i < length; i++) {
    digitalWrite(RTC4534_DATA, (data >> i) & 0x01);
    delayMicroseconds(1);

    // Generate Serial Clock Pulse
    digitalWrite(RTC4534_SCK, HIGH);
    digitalWrite(RTC4534_SCK, LOW);
  }
}


