#include <RC100.h>

#define RC100_MODE 1 // ボーレート57600
#define TIMER1_PERIOD ((uint32)10000) // microsec

RC100 rc100;
int receive_data = 0;

void timer1_interrupt_handler() {
  if(rc100.available()){
    receive_data = rc100.readData();
  }
  
  static boolean output = HIGH;
  digitalWrite(BOARD_LED_PIN, output);
  digitalWrite(6, output);
  output = !output;
}

void setup() {
  pinMode(BOARD_LED_PIN, OUTPUT);
  pinMode(6, OUTPUT);

  // コントローラ初期化
  rc100.begin(RC100_MODE);

  // タイマ1設定
  //// channeは1を使う
  const int channel = 1;
  //// タイマ停止
  Timer1.pause();
  //// 周期設定
  Timer1.setPeriod(TIMER1_PERIOD);
  //// コンペアチャンネルの設定
  Timer1.setCompare(channel, Timer1.getOverflow());
  //// 割り込み関数設定
  Timer1.attachInterrupt(channel, timer1_interrupt_handler);
  //// タイマのリフレッシュ(設定の適応)
  Timer1.refresh();
  //// タイマ再開
  Timer1.resume();
}

void loop() {
  SerialUSB.println(receive_data);
}