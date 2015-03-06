/*
  リアルタイムクロック RTC-4543 評価 WSN366使用
 copyright (c) 2013  www.wsnak.com
 
 13/11/03 新規作成
 */

#define SCK_H digitalWrite(RTC4534_SCK, HIGH)
#define SCK_L digitalWrite(RTC4534_SCK, LOW)

#define DIR_IN  pinMode(RTC4534_DATA, INPUT)   // DATA (I/O)
#define DIR_OUT pinMode(RTC4534_DATA, OUTPUT)

#define GET_BIT    digitalRead(RTC4534_DATA)
#define PUT_BIT(x) digitalWrite(RTC4534_DATA, x)

#define WR_W digitalWrite(RTC4534_RW, HIGH)
#define WR_R digitalWrite(RTC4534_RW, LOW)

#define CE_ENA digitalWrite(RTC4534_CE, HIGH)
#define CE_DIS digitalWrite(RTC4534_CE, LOW)

char strBuf[16];

void setupRtc()
{
  pinMode(RTC4534_SCK, OUTPUT);   // SCK
  pinMode(RTC4534_DATA, OUTPUT);   // DATA (I/O)
  pinMode(RTC4534_RW, OUTPUT);   // RW
  pinMode(RTC4534_CE, OUTPUT);   // CE

  if(getFDTBit()) {
//  if(1) {
    // FDTビット=1(日時不正)
    // 日付設定
    sSec  = 0;
    sMin  = 0x00;
    sHour = 0x17;
    sWeek = 0x6;
    sDay  = 0x6;
    sMon  = 0x3;
    sYear = 0x14;

    putString(0);                     // 日時設定
    Serial.println("set date & time");
    delay(50);
  } 
  else {
    Serial.println("now");
  }
}

void setTimeFromRtc() {
  getString();                        // 日時取得

  tmElements_t t;
  
  t.Year = bcdToDec(dYear);
  t.Month = bcdToDec(dMon & 0x1f);
  t.Day = bcdToDec(dDay & 0x3f);
  t.Hour = bcdToDec(dHour & 0x3f);
  t.Minute = bcdToDec(dMin & 0x7f);
  t.Second = bcdToDec(dSec);

//  Serial.print(t.Year);
//  Serial.print("/");
//  Serial.print(t.Month);
//  Serial.print("/");
//  Serial.print(t.Day);
//  Serial.print(" ");
//  Serial.print(t.Hour);
//  Serial.print(":");
//  Serial.print(t.Minute);
//  Serial.print(":");
//  Serial.print(t.Second);
//  Serial.println("");
  
  setTime(t.Hour, t.Minute, t.Second, t.Day, t.Month, t.Year);
}

//
// 8ビット取得
//
byte get8Bit(void) {
  byte i, dat = 0;

  for(i = 0; i < 8; i++) {
    dat >>= 1;
    SCK_H;              // SCKパルス
    SCK_L;
    if(GET_BIT) {       // SCK立ち下がりで読み出し
      dat |= 0x80;      // b7
    }
  }

  return dat;
}

//
// 4ビット取得
//
byte get4Bit(void) {
  byte i, dat = 0;

  for(i = 0; i < 4; i++) {
    dat >>= 1;
    SCK_H;              // SCKパルス
    SCK_L;
    if(GET_BIT) {       // SCK立ち下がりで読み出し
      dat |= 0x08;      // b3
    }
  }

  return dat;
}

//
// 54ビット全データ読み出し
//
byte getString(void) {
  byte dat, fdt;

  DIR_IN;
  WR_R;
  CE_ENA;
  delayMicroseconds(1);   // セットアップタイム

  dat = get8Bit();
  if(dat & 0x80) {
    fdt = 1;
  } 
  else {
    fdt = 0;
  }
  dSec  = dat & 0x7F;     // FDTビットをクリア
  dMin  = get8Bit();
  dHour = get8Bit();
  dWeek = get4Bit();
  dDay  = get8Bit();
  dMon  = get8Bit();
  dYear = get8Bit();

  CE_DIS;

  return fdt;             // FDTビットの状態を返す
}

//
// FDTビットの状態を読み出す
//
byte getFDTBit(void) {
  byte dat, fdt;

  DIR_IN;
  WR_R;
  CE_ENA;
  delayMicroseconds(1);   // セットアップタイム

  dat = get8Bit();        // 最初の8bit取得
  if(dat & 0x80) {
    fdt = 1;
  } 
  else {
    fdt = 0;
  }

  CE_DIS;

  return fdt;             // FDTビットの状態を返す
}

//
// 8ビット出力
//
void put8Bit(byte dat) {
  byte i;

  for(i = 0; i < 8; i++) {
    PUT_BIT(dat & 0x01);    // DATAビット出力 (LSB)
    delayMicroseconds(1);   // セットアップタイム
    SCK_H;                  // SCKパルス
    SCK_L;
    dat >>= 1;
  }
}

//
// 4ビット出力
//
void put4Bit(byte dat) {
  byte i;

  for(i = 0; i < 4; i++) {
    PUT_BIT(dat & 0x01);    // DATAビット出力 (LSB)
    delayMicroseconds(1);   // セットアップタイム
    SCK_H;                  // SCKパルス
    SCK_L;
    dat >>= 1;
  }
}

//
// 54ビット全データ書き込み
//
byte putString(byte fdt) {
  byte dat;

  DIR_OUT;
  WR_W;
  CE_ENA;
  delayMicroseconds(1);   // セットアップタイム

  dat = (fdt << 7) | sSec;
  put8Bit(dat);
  put8Bit(sMin);
  put8Bit(sHour);
  put4Bit(sWeek);
  put8Bit(sDay);
  put8Bit(sMon);
  put8Bit(sYear);

  CE_DIS;
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

