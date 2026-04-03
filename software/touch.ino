// ===== Touch Pin 定義 =====
#define YP A1
#define XM A2
#define YM 7
#define XP 6

// ===== 參數 =====
#define Z_THRESHOLD 500

// ===== Setup =====
void setup() {
  Serial.begin(9600);
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

  int z = 1023 - (z2 - z1);
  
  Serial.print("Touch -> ");
  Serial.print(" Z: "); Serial.println(z);
  return z > Z_THRESHOLD;
}

// ===== 主迴圈 =====
void loop() {
  if (isTouching()) {
      // 判定為有觸控
  }

  delay(50);
}