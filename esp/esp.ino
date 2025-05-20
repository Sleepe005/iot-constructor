#include <WiFi.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

char ssid[] = "Sleepe";
char password[] = "sleepewww";
int status;

void setup() {
  Serial.begin(112500);

  // Попытка подключения к WiFi
  WiFi.begin(ssid, password);
  Serial.print("Подключение к Wi-Fi");

  // Визуаолизация подключения
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  // Проверка подключения
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Wi-Fi подключён!");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Не удалось подключиться к Wi-Fi.");
  }
}

void loop() {

}
