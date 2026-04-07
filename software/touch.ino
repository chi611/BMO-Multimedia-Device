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
// int totalFrames = sizeof(anim) / sizeof(anim[0]);

#define SD_CS 10

#define WIDTH  320
#define HEIGHT 240

#define CHUNK 80  

uint8_t buffer[CHUNK];
uint16_t colors[CHUNK/2];

void playFolder(const char* dirname)
{
  Serial.print("Opening dir: ");
  Serial.println(dirname);

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
        showRAW(fullpath.c_str());
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
  // showRAW(anim[0]);
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

    // 切狀態
    currentState = (State)((currentState + 1) % 3);

    Serial.print("Switch to state: ");
    Serial.println(currentState);

    playFolder(stateDirs[currentState]);
  }

  delay(50);
}