#include <WiFi.h>

const char* ssid = "Sleepe";
const char* password = "sleepewww";
int port = 8080;

WiFiServer server(port);

void connectToWiFi() {
  Serial.print("Подключение к Wi-Fi");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Wi-Fi подключён!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Не удалось подключиться к Wi-Fi.");
  }
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();

  server.begin();
  Serial.print("Сервер запущен и слушает порт ");
  Serial.println(port);
}

void loop() {
  WiFiClient client = server.available();
  if (client && client.connected()) {
    Serial.println("🔌 Клиент подключён");

    String fullMessage = "";  // Будем собирать сюда все данные

    // Пока клиент подключён
    while (client.connected()) {
      while (client.available()) {
        char c = client.read();     // Читаем по одному байту
        fullMessage += c;
      }
      
      Serial.println("Сообщение от клиента:\n");
      Serial.println(fullMessage);

      client.println("✅ Сообщение получено");
      break;
    }

    client.stop();
    Serial.println("❌ Клиент отключён");
  }

  delay(10);
}
