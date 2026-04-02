#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

MCUFRIEND_kbv tft;

#define SD_CS 10

#define WIDTH  320
#define HEIGHT 240

#define CHUNK 240  

uint8_t buffer[CHUNK];
uint16_t colors[CHUNK/2];

void showRAW(const char *filename)
{
  File f = SD.open(filename);
  if (!f) {
    Serial.println("File open failed");
    return;
  }
  
  bool first = true;
  while(f.available())
  {
    int n = f.read(buffer, CHUNK);

    for(int i=0;i<(n/2);i++)
    {
      colors[i] = (buffer[i*2] << 8) | buffer[i*2+1];
    }
    tft.pushColors(colors, n/2, first);
    if(first)
      first = false;
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