#include <MCUFRIEND_kbv.h>
#include <SPI.h>
#include <SD.h>

MCUFRIEND_kbv tft;

// ===== Touch Pin 定義 =====
#define YP A1
#define XM A2
#define YM 7
#define XP 6

// ===== 參數 =====
#define Z_THRESHOLD 500
const unsigned long IDLETIME = 10000; // 10 秒 (毫秒)

// ===== 動畫檔案列表 =====
const char* stateDirs[] = {
  "ori",
  "idle",
  "touch"
};

enum State {
  ORI,
  IDLE,
  TOUCH
};

State currentState = ORI;

#define SD_CS 10

#define WIDTH  320
#define HEIGHT 240

#define CHUNK 260  

uint8_t buffer[CHUNK];

unsigned long lastTouch = 0;     // 上次觸控時間

void playFolder(const char* dirname)
{
  
  // Serial.print("Opening dir: ");
  // Serial.println(dirname);

  File dir = SD.open(dirname);

  if (!dir) {
    Serial.println("Open dir failed");
    return;
  }

  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry) break;
    
    String name = entry.name();
    name.toLowerCase();

    if (name.endsWith(".raw")) {
        String fullpath = String(dirname) + "/" + entry.name();

        unsigned long startTime = millis();
     
        showRAW(fullpath.c_str());

        unsigned long endTime = millis();

        Serial.print("Display time: ");
        Serial.print(endTime - startTime);
        Serial.println(" ms");
    }

    entry.close();
  }

  dir.close();
}

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
    tft.pushColors(buffer, n/2, first);

    first = false;
  }

  f.close();
}

// ===== Setup =====
void setup() {
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
  lastTouch = millis();
}

// ===== 觸控判斷 =====
bool isTouching() {
  pinMode(XP, OUTPUT); digitalWrite(XP, LOW);
  pinMode(YM, OUTPUT); digitalWrite(YM, HIGH);

  pinMode(XM, INPUT); digitalWrite(XM, LOW);
  pinMode(YP, INPUT); digitalWrite(YP, LOW);

  delay(2);
  int z1 = analogRead(XM);
  int z2 = analogRead(YP);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  int z = 1023 - (z2 - z1);
 
  return z > Z_THRESHOLD;
}

// ===== 主迴圈 =====
void loop() {
  if (isTouching()) {
    currentState = TOUCH;
  } else if(millis() - lastTouch > IDLETIME) {
    currentState = IDLE;
  } else {
    currentState = ORI;
  }

  playFolder(stateDirs[currentState]);

  if (currentState == TOUCH)    
    lastTouch = millis();  
  delay(50);
}