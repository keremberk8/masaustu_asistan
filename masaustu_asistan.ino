#include <Arduino_GFX_Library.h>

#define SENSOR_1     34 
#define SENSOR_2     35 
#define TFT_CS       15
#define TFT_DC       27
#define TFT_RST      33

#define EMO_KOYU      0x0110 
#define EMO_PARLAK    0x07FF 
#define SIYAH         0x0000
#define KIRMIZI       0xF800
#define SARI          0xFFE0

Arduino_DataBus *bus = new Arduino_HWSPI(TFT_DC, TFT_CS);
Arduino_GFX *gfx = new Arduino_ST7789(bus, TFT_RST, 1, true);

// --- DURUMLAR ---
int aktifMod = 0; 
int menuSecim = 0;
bool modDegisti = true; 
unsigned long sonEtkilesim = 0;

// --- DOKUNMATİK SİSTEMİ ---
unsigned long sonTıkZamanı = 0;
int tıkSayısı = 0;
const int çiftTıkHızı = 350; 
bool sonDurum = LOW;
bool dokunusİslemde = false; // BASILI TUTMAYI ENGELLEYEN KRİTİK KİLİT

// --- GÖZ DEĞİŞKENLERİ ---
int eskiX = 0, eskiY = 0, eskiDurum = -1;

// --- OYUN DEĞİŞKENLERİ ---
float dinoY = 140.0; float dikeyHiz = 0; int engelX = 320; int skor = 0; bool oyunBitti = false;
int pomDak = 25; int pomSan = 0; bool pomCalisiyor = false; unsigned long pomZaman = 0;
float birdY = 100.0; float birdHiz = 0; int boruX = 320; int boruBoslukY = 80; int flappySkor = 0;

void setup() {
  pinMode(SENSOR_1, INPUT); 
  pinMode(SENSOR_2, INPUT);
  gfx->begin(); 
  gfx->fillScreen(SIYAH);
  sonEtkilesim = millis();
}

// --- GÖZLERİ ÇİZEN ANA FONKSİYON ---
void emoGozCiz(int offsetX, int offsetY, int duygu) {
  // Göz moduna girince ekranı bir kez sil ama sonra dokunma
  if (modDegisti) { 
    gfx->fillScreen(SIYAH); 
    modDegisti = false; 
    eskiDurum = -1; // Tekrar çizimi zorla
  }
  
  if (offsetX == eskiX && offsetY == eskiY && duygu == eskiDurum) return;

  gfx->fillRect(0, 40, 320, 160, SIYAH); // Göz alanını temizle
  int boyut = 110, kose = 35;
  int xPozs[] = {25 + offsetX, 185 + offsetX};
  int yYeri = 65 + offsetY;

  for (int i = 0; i < 2; i++) {
    int cx = xPozs[i];
    gfx->fillRoundRect(cx, yYeri, boyut, boyut, kose, EMO_KOYU);
    gfx->fillRoundRect(cx + 5, yYeri + 5, boyut - 10, boyut - 10, kose - 5, EMO_PARLAK);
    if (duygu == 1) gfx->fillRoundRect(cx - 2, yYeri - 10, boyut + 4, 45, 20, SIYAH);
    else if (duygu == 2) {
      if (i == 0) gfx->fillTriangle(cx-2, yYeri-2, cx+boyut+2, yYeri-2, cx+boyut+2, yYeri+45, SIYAH);
      else gfx->fillTriangle(cx-2, yYeri-2, cx+boyut+2, yYeri-2, cx-2, yYeri+45, SIYAH);
    }
  }
  eskiX = offsetX; eskiY = offsetY; eskiDurum = duygu;
}

void menuGuncelle() {
  if (modDegisti) { gfx->fillScreen(SIYAH); modDegisti = false; }
  const char* isimler[] = {"DERS SAYACI", "DINO OYUNU", "FLAPPY BIRD", "ANA EKRAN"};
  for (int i = 0; i < 4; i++) {
    int y = 75 + (i * 40);
    if (i == menuSecim) {
      gfx->fillRoundRect(20, y - 5, 280, 35, 10, EMO_PARLAK);
      gfx->setTextColor(SIYAH);
    } else {
      gfx->fillRect(20, y - 5, 280, 35, SIYAH);
      gfx->drawRoundRect(20, y - 5, 280, 35, 10, EMO_KOYU);
      gfx->setTextColor(EMO_PARLAK);
    }
    gfx->setCursor(40, y + 3); gfx->setTextSize(2); gfx->println(isimler[i]);
  }
}

void loop() {
  bool kafaOkuma = digitalRead(SENSOR_1);
  bool elOkuma = digitalRead(SENSOR_2);
  unsigned long simdi = millis();

  // GERİ TUŞU
  if (elOkuma == HIGH && aktifMod != 0) {
    aktifMod = (aktifMod == 1) ? 0 : 1;
    modDegisti = true; delay(300);
  }

  // SENSÖR ANALİZİ (BAS-ÇEK MANTIĞI)
  if (kafaOkuma == HIGH && !sonDurum) { // Dokunulduğu AN
    sonDurum = true;
    tıkSayısı++;
    if (tıkSayısı == 1) sonTıkZamanı = simdi;
    
    // OYUNLARDA ANLIK ZIPLAMA (Beklemeden)
    if (aktifMod == 3 && !oyunBitti && dinoY >= 135) dikeyHiz = -12.0;
    if (aktifMod == 4 && !oyunBitti) birdHiz = -6.5; 
  } 
  
  if (kafaOkuma == LOW && sonDurum) { // El çekildiği AN
    sonDurum = false;
  }

  // MENÜ GEZİNTİSİ (Zamanlama ile çift tık/tek tık ayrımı)
  if (tıkSayısı > 0 && (simdi - sonTıkZamanı > çiftTıkHızı)) {
    if (tıkSayısı >= 2) { // ÇİFT TIK: SEÇ
      if (aktifMod == 1) {
        if (menuSecim == 0) aktifMod = 2;
        else if (menuSecim == 1) { aktifMod = 3; oyunBitti = false; engelX = 320; dinoY = 140; skor = 0; modDegisti = true; }
        else if (menuSecim == 2) { aktifMod = 4; oyunBitti = false; boruX = 320; birdY = 100; birdHiz = 0; flappySkor = 0; modDegisti = true; }
        else aktifMod = 0;
        modDegisti = true;
      }
    } else { // TEK TIK: MENÜDE GEZ VEYA OYUNU BAŞLAT
      if (aktifMod == 0) { aktifMod = 1; modDegisti = true; }
      else if (aktifMod == 1) { menuSecim = (menuSecim + 1) % 4; menuGuncelle(); }
      else if (aktifMod == 2) { pomCalisiyor = !pomCalisiyor; }
      else if ((aktifMod == 3 || aktifMod == 4) && oyunBitti) { oyunBitti = false; modDegisti = true; boruX = 320; engelX = 320; skor = 0; flappySkor = 0; birdY = 100; birdHiz = 0; }
    }
    tıkSayısı = 0; sonEtkilesim = simdi;
  }

  // MOD YÖNETİMİ
  switch (aktifMod) {
    case 0: emoGozCiz(0, 0, 0); break; // Gözler modu sabitlendi
    case 1: menuGuncelle(); break;
    case 2: runPomodoro(simdi); break;
    case 3: runDino(); break;
    case 4: runFlappy(); break;
  }
}

void runPomodoro(unsigned long simdi) {
  if (modDegisti) { gfx->fillScreen(SIYAH); modDegisti = false; pomDak = 25; pomSan = 0; }
  if (pomCalisiyor && (simdi - pomZaman >= 1000)) {
    pomZaman = simdi;
    if (pomSan == 0) { if (pomDak > 0) { pomDak--; pomSan = 59; } } else pomSan--;
  }
  gfx->setCursor(60, 100); gfx->setTextSize(6); gfx->setTextColor(EMO_PARLAK, SIYAH);
  if(pomDak < 10) gfx->print("0"); gfx->print(pomDak); gfx->print(":");
  if(pomSan < 10) gfx->print("0"); gfx->print(pomSan);
}

void runDino() {
  if (modDegisti) { gfx->fillScreen(SIYAH); modDegisti = false; }
  if (!oyunBitti) {
    gfx->fillRect(40, (int)dinoY, 22, 22, SIYAH); gfx->fillRect(engelX, 140, 18, 22, SIYAH);
    dikeyHiz += 1.0; dinoY += dikeyHiz; if (dinoY > 140) { dinoY = 140; dikeyHiz = 0; }
    engelX -= 8; if (engelX < -20) { engelX = 320; skor++; }
    if (engelX < 60 && engelX > 20 && dinoY > 120) oyunBitti = true;
    gfx->fillRect(40, (int)dinoY, 20, 20, EMO_PARLAK); gfx->fillRect(engelX, 140, 15, 20, KIRMIZI);
    gfx->drawFastHLine(0, 160, 320, EMO_KOYU);
    gfx->setCursor(250, 10); gfx->setTextSize(2); gfx->setTextColor(EMO_PARLAK, SIYAH); gfx->print(skor);
  } else { gfx->setCursor(80, 100); gfx->setTextColor(KIRMIZI, SIYAH); gfx->print("GAME OVER"); }
  delay(20);
}

void runFlappy() {
  if (modDegisti) { gfx->fillScreen(SIYAH); modDegisti = false; boruBoslukY = 80; }
  if (!oyunBitti) {
    gfx->fillRect(50, (int)birdY, 18, 18, SIYAH); 
    gfx->fillRect(boruX, 0, 30, 240, SIYAH); 

    birdHiz += 0.5; // Yerçekimi azaltıldı (Hafifledi)
    birdY += birdHiz;
    boruX -= 5; // Boru hızı ayarlandı
    
    if (boruX < -30) { boruX = 320; boruBoslukY = random(30, 140); flappySkor++; }
    if (birdY > 220 || birdY < 0 || (boruX < 65 && boruX > 35 && (birdY < boruBoslukY || birdY > boruBoslukY + 70))) oyunBitti = true;

    gfx->fillRect(50, (int)birdY, 15, 15, SARI); 
    gfx->fillRect(boruX, 0, 25, boruBoslukY, EMO_KOYU); 
    gfx->fillRect(boruX, boruBoslukY + 80, 25, 240, EMO_KOYU); // Boşluk genişletildi
    gfx->setCursor(250, 10); gfx->setTextSize(2); gfx->setTextColor(EMO_PARLAK, SIYAH); gfx->print(flappySkor);
  } else { gfx->setCursor(80, 100); gfx->setTextColor(KIRMIZI, SIYAH); gfx->print("GAME OVER"); }
  delay(20);
}