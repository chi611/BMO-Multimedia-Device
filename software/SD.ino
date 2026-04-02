#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

MCUFRIEND_kbv tft;

#define SD_CS 10

#define WIDTH  320
#define HEIGHT 240

#define CHUNK 80   // 80 pixels

uint16_t buffer[CHUNK];

void showRAW(const char *filename)
{
  File f = SD.open(filename);
  if (!f) {
    Serial.println("File open failed");
    return;
  }

  for (int y = 0; y < HEIGHT; y++) {

    for (int x = 0; x < WIDTH; x += CHUNK) {

      for (int i = 0; i < CHUNK; i++) {
        uint8_t hi = f.read();
        uint8_t lo = f.read();
        buffer[i] = (hi << 8) | lo;
      }

      tft.pushColors(buffer, CHUNK, (x == 0 && y == 0));
    }
  }

  f.close();
}

void setup()
{
  Serial.begin(9600);

  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(0x0000);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD init failed!");
    return;
  }

  Serial.println("SD OK");

  Serial.println("Start drawing...");
  unsigned long startTime = millis();

  showRAW("test.raw");

  unsigned long endTime = millis();

  Serial.print("Display time: ");
  Serial.print(endTime - startTime);
  Serial.println(" ms");
}

void loop()
{
}