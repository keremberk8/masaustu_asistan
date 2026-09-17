# Masaüstü Asistan

ESP32 tabanlı, küçük bir TFT ekran üzerinde **etkileşimli yüz ifadeleri, mini oyunlar ve Pomodoro sayacı** sunan deneysel masaüstü asistan projesidir.

Dokunma sensörleri üzerinden kullanıcı etkileşimini algılar ve ekranı gerçek zamanlı olarak günceller. Projede aynı cihaz üzerinde emote/göz animasyonları, menü sistemi, Pomodoro ve iki farklı mini oyun bulunur.

## ✨ Özellikler

- 👀 Etkileşimli göz / emote ekranı
- 🖐️ Sensör tabanlı kullanıcı etkileşimi
- 📋 Menü sistemi
- ⏱️ 25 dakikalık Pomodoro modu
- 🦖 Dino mini oyunu
- 🐦 Flappy Bird benzeri mini oyun
- 🎨 ST7789 TFT ekran arayüzü
- ⚡ ESP32 tabanlı gerçek zamanlı kontrol

## 🛠️ Donanım ve Teknolojiler

- ESP32
- ST7789 TFT ekran
- Dokunma / yakınlık sensörleri
- Arduino IDE
- C/C++
- `Arduino_GFX_Library`

## 🎮 Modlar

| Mod | Açıklama |
|---|---|
| 👀 Ana ekran | Animasyonlu göz/emote ekranı |
| 📋 Menü | Modlar arasında gezinme |
| ⏱️ Pomodoro | Odaklanma için geri sayım |
| 🦖 Dino | Engel aşmaya dayalı mini oyun |
| 🐦 Flappy | Borulardan kaçmaya dayalı mini oyun |

## 🕹️ Etkileşim

Sistem iki sensörden gelen girişleri kullanır. Tek tıklama menüde gezinmek veya ilgili modu başlatmak için, çift tıklama ise seçim yapmak için kullanılır.

Oyun modlarında sensör girişi karakterin hareketini kontrol eder.

## 📁 Proje Yapısı

```text
masaustu_asistan/
├── masaustu_asistan.ino
└── README.md
```

## 🚀 Kurulum

1. Arduino IDE'yi açın.
2. ESP32 kart desteğini kurun.
3. `Arduino_GFX_Library` kütüphanesini yükleyin.
4. TFT ve sensör bağlantılarını kaynak koddaki pinlerle eşleştirin.
5. `masaustu_asistan.ino` dosyasını ESP32'ye yükleyin.

## ⚙️ Pin Yapılandırması

Kaynak kodda temel bağlantılar şu şekilde tanımlanmıştır:

| Bileşen | Pin |
|---|---:|
| Sensör 1 | GPIO 34 |
| Sensör 2 | GPIO 35 |
| TFT CS | GPIO 15 |
| TFT DC | GPIO 27 |
| TFT RST | GPIO 33 |

Donanım değiştirildiğinde bu değerler kaynak koddan güncellenmelidir.

## 🚧 Geliştirme Durumu

**Deneysel / hobi projesi**

Proje, ileride daha gelişmiş animasyonlar, yeni oyunlar, sesli geri bildirim ve farklı sensörlerle genişletilebilecek şekilde tasarlanmıştır.
