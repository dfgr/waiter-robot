#define r 6
#define g 9
#define b 10
#define horn 11

unsigned long lastCommandTime = 0;
bool isAnimationMode = true;

void setup() {
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(horn, OUTPUT);

  Serial.begin(115200); // ESP8266 ile seri iletişim başlatılıyor
  Serial.print("wakeup");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Komut okuma

    if (command.startsWith("LED")) {
      int red = command.substring(4, 7).toInt();
      int green = command.substring(8, 11).toInt();
      int blue = command.substring(12, 15).toInt();

      analogWrite(r, 255 - red);
      analogWrite(g, 255 - green);
      analogWrite(b, 255 - blue);

      lastCommandTime = millis();
      isAnimationMode = false;
    } else if (command.startsWith("HORN")) {
      int state = command.substring(5).toInt();
      digitalWrite(horn, state);
    }
  }

  // 5 saniye boyunca komut gelmezse animasyon moduna geç
  if (millis() - lastCommandTime > 5000) {
    isAnimationMode = true;
  }

  if (isAnimationMode) {
    runAnimation();
  }
}

void runAnimation() {
  static float red = 255, green = 0, blue = 0;
  static int phase = 0;
  static unsigned long lastUpdateTime = 0;

  if (millis() - lastUpdateTime > 15) { // Daha yavaş geçiş için 50 ms gecikme
    lastUpdateTime = millis();

    switch (phase) {
      case 0: // Kırmızıdan yeşile
        green += 5;
        if (green >= 255) {
          green = 255;
          phase = 1;
        }
        break;
      case 1: // Yeşilden maviye
        red -= 5;
        if (red <= 0) {
          red = 0;
          phase = 2;
        }
        break;
      case 2: // Maviden kırmızıya
        blue += 5;
        if (blue >= 255) {
          blue = 255;
          phase = 3;
        }
        break;
      case 3: // Kırmızı ve mavi karışımından yeşile geri dön
        green -= 5;
        if (green <= 0) {
          green = 0;
          phase = 4;
        }
        break;
      case 4: // Yeşil ve mavi karışımından kırmızıya geri dön
        blue -= 5;
        if (blue <= 0) {
          blue = 0;
          phase = 5;
        }
        break;
      case 5: // Tüm fazları tamamlayıp başa dön
        red += 5;
        if (red >= 255) {
          red = 255;
          phase = 0;
        }
        break;
    }

    analogWrite(r, 255 - (int)red);
    analogWrite(g, 255 - (int)green);
    analogWrite(b, 255 - (int)blue);
  }
}
