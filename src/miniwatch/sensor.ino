// see: http://radiopench.blog96.fc2.com/blog-entry-490.html

float cpuTemp(){                     // CPU温度測定関数
  long sum=0;
  adcSetup(0xC8);                    // Vref=1.1V, input=ch8
  for(int n=0; n < 100; n++){
    sum = sum + adc();               // adcの値を読んで積分
  }
  return (sum * 1.1/102.4)- 342.5;   // 温度を計算して戻り値にする。-342.5は要調整
}
 
float cpuVcc(){                      // 電源電圧(AVCC)測定関数
  long sum=0;
  adcSetup(0x4E);                    // Vref=AVcc, input=internal1.1V
  for(int n=0; n < 10; n++){
    sum = sum + adc();               // adcの値を読んで積分
  }
  return (1.1 * 10240.0)/ sum;       // 電圧を計算して戻り値にする
}
 
void adcSetup(byte data){            // ADコンバーターの設定
  ADMUX = data;                      // ADC Multiplexer Select Reg.
  ADCSRA |= ( 1 << ADEN);            // ADC イネーブル
  ADCSRA |= 0x07;                    // AD変換クロック　CK/128
  delay(10);                         // 安定するまで待つ
}
 
unsigned int adc(){                  // ADCの値を読む
  unsigned int dL, dH;
  ADCSRA |= ( 1 << ADSC);            // AD変換開始
  while(ADCSRA & ( 1 << ADSC) ){     // 変換完了待ち
  }
  dL = ADCL;                         // LSB側読み出し 
  dH = ADCH;                         // MSB側
  return dL | (dH << 8);             // 10ビットに合成した値を返す
}

