# Sistem Pendeteksi Kebakaran dengan ESP32 dan Integrasi Blynk

## Gambaran Umum
Proyek ini mengimplementasikan sistem pendeteksi kebakaran pintar menggunakan mikrokontroler ESP32, sensor MQ-02 (asap) dan MQ-05 (gas), dengan pemantauan dan kontrol secara real-time melalui platform IoT Blynk. Sistem ini dilengkapi dengan pemicu alarm otomatis, opsi kontrol manual, dan tampilan LCD untuk pemantauan status.

## Kebutuhan Perangkat Keras
- Board ESP32
- Sensor Asap MQ-02
- Sensor Gas MQ-05
- LCD Display (16x2 dengan antarmuka I2C)
- Buzzer
- Kipas
- Kabel Jumper
- Catu Daya

## Konfigurasi Pin
```
Sensor MQ-02: GPIO 35
Sensor MQ-05: GPIO 34
LCD SDA: GPIO 25
LCD SCL: GPIO 26
Buzzer: GPIO 32
Kipas: GPIO 33
```

## Kebutuhan Perangkat Lunak
- Arduino IDE
- Library yang Dibutuhkan:
  - WiFi.h
  - BlynkSimpleEsp32.h
  - Wire.h
  - LiquidCrystal_I2C.h

## Konfigurasi Blynk
```
Template ID: TMPL6wwzOefKr
Template Name: SKRIPSI
Auth Token: VZ14MQXb8noPchk-Xvl0kBSjZDiXsj0x
```

### Pengaturan Pin Virtual
- V0: Status Gas MQ-05
- V1: Status Asap MQ-02
- V2: Status Peringatan Kebakaran
- V3: Kontrol Manual Kipas
- V4: Kontrol Manual Buzzer

## Fitur-Fitur
1. **Deteksi Otomatis**
   - Pemantauan tingkat asap dan gas secara terus menerus
   - Pemicu alarm otomatis ketika melewati ambang batas
   - Pengiriman data real-time ke aplikasi Blynk

2. **Kontrol Manual**
   - Kemampuan mengatur kipas dan buzzer melalui aplikasi Blynk
   - Kontrol perangkat peringatan secara independen

3. **Fitur Tampilan**
   - LCD menampilkan status sensor terkini
   - Pesan peringatan saat kondisi berbahaya
   - Status koneksi saat sistem mulai

4. **Fitur Keamanan**
   - Interval peringatan 5 detik
   - Aktivasi kipas dan buzzer otomatis
   - Sistem notifikasi Blynk untuk peringatan

## Nilai Ambang Batas
- Ambang Batas MQ-02 (Asap): 50%
- Ambang Batas MQ-05 (Gas): 2000 (nilai mentah)
- Rentang Sensor:
  - MQ-02: 500-4095 (dipetakan ke 0-100%)
  - MQ-05: 0-4095 (nilai mentah)

## Petunjuk Pemasangan
1. Install library yang dibutuhkan di Arduino IDE
2. Atur kredensial WiFi:
   ```cpp
   const char* ssid = "Arul";
   const char* pass = "123456789";
   ```
3. Perbarui detail autentikasi Blynk
4. Upload kode ke ESP32
5. Hubungkan perangkat keras sesuai konfigurasi pin
6. Nyalakan sistem
7. Tunggu periode pemanasan 20 detik

## Mode Operasi
- **Operasi Normal**: LCD menampilkan level gas dan asap terkini
- **Mode Peringatan**: Aktif ketika salah satu sensor melewati ambang batas
  - LCD menampilkan pesan peringatan
  - Kipas dan buzzer aktif selama 5 detik
  - Aplikasi Blynk menerima notifikasi
  - Status diperbarui ke dashboard Blynk

## Penyelesaian Masalah
- Periksa koneksi WiFi jika perangkat tidak terhubung
- Verifikasi sambungan sensor jika pembacaan tidak tepat
- Pastikan kredensial Blynk benar jika aplikasi tidak menerima data
- Periksa alamat I2C jika LCD tidak menyala

## Catatan Penting
- Sistem memerlukan periode pemanasan 20 detik untuk stabilisasi sensor
- Kontrol manual mengesampingkan fungsi otomatis
- Sensor dibaca dan diperbarui setiap detik
- Sistem peringatan memiliki periode cooldown 5 detik

## Peringatan Keselamatan
Sistem ini dirancang hanya untuk deteksi dini. Jangan mengandalkan sistem ini sebagai satu-satunya alat keselamatan kebakaran. Selalu ikuti protokol keselamatan kebakaran yang tepat dan pertahankan peralatan keselamatan kebakaran lainnya.