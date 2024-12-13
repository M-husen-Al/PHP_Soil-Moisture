#include <WiFi.h>
#include <HTTPClient.h>

// Definisi pin
#define SOIL_SENSOR_PIN 34 // Pin untuk sensor soil moisture
#define RELAY_PIN 23       // Pin untuk relay

// Ambang batas kelembapan (0-4095 untuk ESP32 ADC)
const int moistureThreshold = 2000;

// Informasi WiFi
const char* ssid = "KPJ";      // Ganti dengan nama WiFi Anda
const char* password = "02061961"; // Ganti dengan password WiFi Anda

// URL PHP script yang akan menerima data
const char* serverName = "http://192.168.1.8/soilmoisture_project/soilmoisture_data.php"; // URL PHP Anda

// Variabel untuk melacak status pompa
bool lastPumpState = false; // false = mati, true = hidup

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Konfigurasi pin
  pinMode(SOIL_SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Pastikan relay mati saat mulai

  // Menghubungkan ke WiFi
  connectToWiFi();
}

void loop() {
  // Membaca nilai kelembapan tanah
  int soilMoistureValue = analogRead(SOIL_SENSOR_PIN);

  // Hitung persentase kelembapan (opsional)
  int moisturePercentage = map(soilMoistureValue, 0, 4095, 100, 0);

  // Tampilkan nilai kelembapan di Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

  // Logika kontrol pompa (relay)
  if (soilMoistureValue < moistureThreshold) {
    if (!lastPumpState) { // Jika pompa sebelumnya mati
      Serial.println("Status: Soil is dry. Pump ON.");
      digitalWrite(RELAY_PIN, HIGH); // Relay aktif (pompa hidup)
      sendDataToServer(soilMoistureValue); // Kirim data saat pompa hidup
      lastPumpState = true; // Perbarui status pompa
    }
  } else {
    if (lastPumpState) { // Jika pompa sebelumnya hidup
      Serial.println("Status: Soil is wet. Pump OFF.");
      digitalWrite(RELAY_PIN, LOW); // Relay mati (pompa mati)
      sendDataToServer(soilMoistureValue); // Kirim data saat pompa mati
      lastPumpState = false; // Perbarui status pompa
    }
  }

  // Delay untuk pembacaan berikutnya
  delay(2000);
}

// Fungsi untuk menghubungkan ke WiFi
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Tambahkan batas waktu untuk koneksi WiFi
  int timeout = 30; // Waktu maksimal dalam detik
  int elapsedTime = 0;

  while (WiFi.status() != WL_CONNECTED && elapsedTime < timeout) {
    delay(1000);
    elapsedTime++;
    Serial.println("Connecting...");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect to WiFi. Check credentials.");
  }
}

// Fungsi untuk mengirim data ke server PHP
void sendDataToServer(int soilMoistureValue) {
  if (WiFi.status() == WL_CONNECTED) {  // Pastikan WiFi terkoneksi
    HTTPClient http;

    // Format Data untuk POST
    String postData = "Kelembaban_tanah=" + String(soilMoistureValue);

    // Mengirim POST request
    http.begin(serverName); // Inisialisasi koneksi HTTP
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Header untuk POST
    int httpResponseCode = http.POST(postData); // Mengirimkan data

    // Mengecek status response
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println("Response payload: " + payload);
    } else {
      Serial.print("Error in HTTP request: ");
      Serial.println(httpResponseCode);
    }

    // Menutup koneksi HTTP
    http.end();
  } else {
    Serial.println("Error: Not connected to WiFi.");
  }
}
