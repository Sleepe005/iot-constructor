#include <WiFi.h>
#include <string.h>

#define MAX_PAIRS 10
#define MAX_KEY_LEN 32
#define MAX_VALUE_LEN 64

// статус работы программы (0 - открыта точка доступа; 1 - ожидание ssid и pass; 2 - вещание в сеть о своём присутствии; 3 - ожидание команд)
int workStatus = 0;

char ssid[MAX_VALUE_LEN];
char pass[MAX_VALUE_LEN];

int port = 8080;
WiFiServer server(port);

void setup() {
  Serial.begin(115200);
}

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
}

void connectToWiFi() {
  Serial.print("Подключение к Wi-Fi");
  WiFi.begin(ssid, pass);

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
    workStatus = 2;
  } else {
    Serial.println("\n❌ Не удалось подключиться к Wi-Fi.");
  }

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
    }else if(strcmp(parsed[0].value, "wifi") == 0){
      strncpy(ssid, parsed[1].value, MAX_VALUE_LEN - 1);
      ssid[MAX_VALUE_LEN - 1] = '\0';

      strncpy(pass, parsed[2].value, MAX_VALUE_LEN - 1);
      pass[MAX_VALUE_LEN - 1] = '\0';

      Serial.println("Получены данные для подключения");
      Serial.println(ssid);
      Serial.println(pass);

      workStatus = 1;
    }else{
      requestClient.write("Good");
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
  }else if(workStatus == 1){
    WiFi.softAPdisconnect(true);
    // WiFi.begin(ssid, pass);
    connectToWiFi();
    // workStatus = 2;
  }
}
