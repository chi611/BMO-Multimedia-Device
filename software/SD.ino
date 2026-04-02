#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <SD.h>

MCUFRIEND_kbv tft;

#define SD_CS 10   // UNO 的 SD 卡 CS 腳位

File bmpFile;

// ------------------------------------------------------

uint16_t read16(File f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(File f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}

// ------------------------------------------------------

void showBMP(char *filename, int x, int y)
{
  uint32_t bmpWidth, bmpHeight;
  uint16_t bmpDepth;
  uint32_t bmpImageoffset;
  uint32_t rowSize;

  bmpFile = SD.open(filename);
  if (!bmpFile) {
    Serial.println("BMP file not found");
    return;
  }

  if (read16(bmpFile) == 0x4D42) {  // 確認是 BMP
    read32(bmpFile);                 // file size
    read32(bmpFile);                 // creator bytes
    bmpImageoffset = read32(bmpFile);

    read32(bmpFile);                 // DIB header size
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);

    if (read16(bmpFile) == 1) {
      bmpDepth = read16(bmpFile);

      if (bmpDepth == 24 && read32(bmpFile) == 0) {

        rowSize = (bmpWidth * 3 + 3) & ~3;

        for (int row = 0; row < bmpHeight; row++) {

          bmpFile.seek(bmpImageoffset + (bmpHeight - 1 - row) * rowSize);

          for (int col = 0; col < bmpWidth; col++) {

            uint8_t b = bmpFile.read();
            uint8_t g = bmpFile.read();
            uint8_t r = bmpFile.read();

            uint16_t color = tft.color565(r, g, b);
            tft.drawPixel(x + col, y + row, color);
          }
        }
      }
    }
  }

  bmpFile.close();
}

// ------------------------------------------------------

void setup()
{
  Serial.begin(9600);

  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(0x0000);

  Serial.println("Init SD...");

  if (!SD.begin(SD_CS)) {
    Serial.println("SD init failed!");
    return;
  }

  Serial.println("SD OK");

  Serial.println("Start drawing...");
  unsigned long startTime = millis();

  showBMP("test.bmp", 0, 0);

  unsigned long endTime = millis();

  Serial.print("Display time: ");
  Serial.print(endTime - startTime);
  Serial.println(" ms");
}

// ------------------------------------------------------

void loop()
{
}