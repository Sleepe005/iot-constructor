#include <WiFi.h>
#include <string.h>


// статус работы программы (0 - открыта точка доступа, ожидание ssid и pass; 1 - вещание в сеть о своём присутствии; 2 - ожидание команд)
int workStatus = 0;

int port = 8080;
WiFiServer server(port);

void setup() {
  Serial.begin(115200);
  // WiFi.mode(WIFI_AP);
  // WiFi.softAP("ESP");
}

#define MAX_PAIRS 10
#define MAX_KEY_LEN 32
#define MAX_VALUE_LEN 64



void trimQuotes(char* str) {
  size_t len = strlen(str);
  if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
    // Сдвигаем строку влево и обрезаем кавычки
    memmove(str, str + 1, len - 2);
    str[len - 2] = '\0';
  }
}

struct KeyValuePair {
  char key[MAX_KEY_LEN];
  char value[MAX_VALUE_LEN];
};

void parseSimpleJson(const char* json, struct KeyValuePair* output, int* count) {
  *count = 0;

  // Копируем строку в рабочий буфер
  char buffer[256];
  strncpy(buffer, json, sizeof(buffer) - 1);
  buffer[sizeof(buffer) - 1] = '\0';

  // Удаляем фигурные скобки
  size_t len = strlen(buffer);
  if (buffer[0] == '{') memmove(buffer, buffer + 1, len--);
  if (buffer[len - 1] == '}') buffer[len - 1] = '\0';

  // Разбиваем по запятым
  char* token = strtok(buffer, ",");
  while (token != NULL && *count < MAX_PAIRS) {
    char* colon = strchr(token, ':');
    if (colon != NULL) {
      *colon = '\0'; // разделяем ключ и значение

      char* key = token;
      char* value = colon + 1;

      // Удаляем пробелы в начале и конце
      while (*key == ' ') key++;
      while (*value == ' ') value++;

      trimQuotes(key);
      trimQuotes(value);

      strncpy(output[*count].key, key, MAX_KEY_LEN - 1);
      output[*count].key[MAX_KEY_LEN - 1] = '\0';

      strncpy(output[*count].value, value, MAX_VALUE_LEN - 1);
      output[*count].value[MAX_VALUE_LEN - 1] = '\0';

      (*count)++;
    }
    token = strtok(NULL, ",");
  }
}

void onAPmode(){
  // Переводим ESP в режим AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP");
  Serial.println("AP режим запущен");
  
  // Запускаем сервер и слушаем порт 
  server.begin();
  Serial.println("Сервер запущен и слушает порт 8080");

  // Открываем подключение
  WiFiClient requestClient = server.available();
  if(requestClient && requestClient.connected()){
    Serial.println("Новое подключение");

    String request = "";
    // Читаем текст запроса
    while (requestClient.connected()) {
      while (requestClient.available()) {
        char c = requestClient.read();     // Читаем по одному байту
        request += c;
      }

      break;
    }

    Serial.println("От клиента пришло:");
    Serial.println(request);

    // парсим текст запроса
    KeyValuePair parsed[MAX_PAIRS];
    int pairCount = 0;

    parseSimpleJson(request.c_str(), parsed, &pairCount);
    
    if(strcmp(parsed[0].value, "ping") == 0){
      Serial.println("Пришёл ping");
      requestClient.write("ESPpong");
    }
  }

  requestClient.stop();
  delay(10);
}

void getRequest(){
  // Открываем подключение
  WiFiClient requestClient = server.available();
  if(requestClient && requestClient.connected()){
    Serial.println("Новое подключение");

    String request = "";
    // Читаем текст запроса
    while (requestClient.connected()) {
      while (requestClient.available()) {
        char c = requestClient.read();     // Читаем по одному байту
        request += c;
      }

      break;
    }

    Serial.println("От клиента пришло:");
    Serial.println(request);

    // парсим текст запроса
    KeyValuePair parsed[MAX_PAIRS];
    int pairCount = 0;

    parseSimpleJson(request.c_str(), parsed, &pairCount);
    
    if(strcmp(parsed[0].value, "ping") == 0){
      Serial.println("Пришёл ping");
      requestClient.write("ESPpong");
    }
  }

  requestClient.stop();
  delay(10);
}

void loop() {
  if(workStatus == 0){
    Serial.println("Переводим ESP в режим AP");
    onAPmode();
    while(workStatus == 0){
      getRequest();
    }
  }
}
