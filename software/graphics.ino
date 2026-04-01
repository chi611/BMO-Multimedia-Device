#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

void setup() {
  Serial.begin(9600);

  uint16_t ID = tft.readID();   // 讀取 LCD 型號
  tft.begin(ID);

  tft.setRotation(1);           // 橫向顯示
  tft.fillScreen(0x0000);       // 清空畫面（黑色）

  // 顯示文字確認 LCD 正常
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.print("Shape Test");

  delay(1000);

  drawRectTest();
  delay(1500);

  drawCircleTest();
}

void loop() {
}

// =======================
// 矩形測試
// =======================
void drawRectTest() {
  tft.fillScreen(0x0000);

  tft.setTextColor(0xFFE0); // 黃色
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.print("Rectangle Test");

  // 實心矩形
  tft.fillRect(40, 80, 80, 60, 0xF800); // 紅色

  // 空心矩形
  tft.drawRect(150, 80, 80, 60, 0x07E0); // 綠色

  // 多層矩形
  for (int i = 0; i < 5; i++) {
    tft.drawRect(30 + i * 10, 170 + i * 5, 180 - i * 20, 60 - i * 10, 0x07FF);
  }
}

// =======================
// 圓形測試
// =======================
void drawCircleTest() {
  tft.fillScreen(0x0000);

  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.print("Circle Test");

  // 實心圓
  tft.fillCircle(80, 140, 40, 0x001F); // 藍色

  // 空心圓
  tft.drawCircle(180, 140, 40, 0xFFFF); // 白色

  // 多層圓
  for (int r = 10; r <= 40; r += 5) {
    tft.drawCircle(120, 220, r, 0xF81F);
  }
}