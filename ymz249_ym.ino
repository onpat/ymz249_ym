#include "song.h"
// Output Pins
const byte WRCS_PIN = 10;
const byte A0_PIN = 11;
const byte RESET_PIN = 12;
const int DATA_PIN[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

void setup() {

  for (int i = 0; i < 8; i++) {
    pinMode(DATA_PIN[i], OUTPUT);
  }
  pinMode(WRCS_PIN, OUTPUT);
  pinMode(A0_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  
  reset();
  
  set_register(0x07, 0b00111000);
  set_register(0x08, 0);
  set_register(0x09, 0);
  set_register(0x0a, 0);
  SetFrequency(0, 0);
  SetFrequency(1, 0);
  SetFrequency(2, 0);

  delay(100);

  SetFrequency(0, 440);
  set_register(0x08, 15);
  delay(1000);
  set_register(0x08, 0);
  SetFrequency(0, 0);

}

void loop() {
  unsigned char addr = 0, data = 0;
  int i = 0;
  //for (int i = 0 ; i < 25000 ; i++) {
  while(1) {
     addr = pgm_read_byte(sngfile + i);
     i++;
     data = pgm_read_byte(sngfile + i);
     i++;
     if (addr == 0xff or data == 0xff) {
      break;
     }
     if (addr < 16) {
       set_register(addr, data);
     } else if (addr == 16) {
       delay(20);
     }
   }
}

// 初期化
void reset() {
  set_register(0x00, 0);
  set_register(0x01, 0);
  set_register(0x02, 0);
  set_register(0x03, 0);
  set_register(0x04, 0);
  set_register(0x05, 0);
  set_register(0x06, 0);

  set_register(0x07, 0);
  set_register(0x08, 0);
  set_register(0x09, 0);
  set_register(0x0a, 0);

  set_register(0x0b, 0);
  set_register(0x0c, 0);
  set_register(0x0d, 0);

  digitalWrite(WRCS_PIN, HIGH);
  digitalWrite(A0_PIN, LOW);
  digitalWrite(RESET_PIN, LOW);
  delay(10);
  digitalWrite(RESET_PIN, HIGH);

}

void SetFrequency(int ch, word freq) {
  word cal_freqency = 0;
  if (freq != 0) {
    cal_freqency = 125000 / freq;
  }
  cal_freqency &= 0b0000111111111111;
  set_register(0x00 + (ch * 2), cal_freqency & 0xff);
  set_register(0x01 + (ch * 2), (cal_freqency >> 8) & 0xff);
}

// レジスタセット
void set_register(byte addr, byte value)
{
  // addr
  digitalWrite(WRCS_PIN, LOW);
  digitalWrite(A0_PIN, LOW);
  //for (int i = 0; i < 8; i++) {
  //  digitalWrite(DATA_PIN[i], bitRead(addr, i));
  //}
  PORTD = addr;
  digitalWrite(WRCS_PIN, HIGH);

  // value
  digitalWrite(WRCS_PIN, LOW);
  digitalWrite(A0_PIN, HIGH);
  //for (int i = 0; i < 8; i++) {
  //  digitalWrite(DATA_PIN[i], bitRead(value, i));
  //}
  PORTD = value;
  digitalWrite(WRCS_PIN, HIGH);
}
