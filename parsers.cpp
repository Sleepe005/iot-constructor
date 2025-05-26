#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>

// void split(char *split_string, char separator, char **res){
//     std::string buf = "";
    
//     for(auto it = split_string; it != split_string+sizeof(split_string); ++it){
//         if(*it != separator){
//             buf += *it;
//         }else{
//             res.push_back(buf);
//             buf = "";
//         }
//     }
//     res.push_back(buf);
// }

// void jsonParser(std::string jsonString, std::map<std::string, std::string> &res){
//     if(strlen(jsonString.c_str()) < 2){ return; }

//     jsonString.erase(0,1);
//     jsonString.erase(jsonString.length()-1, 1);

//     std::vector<std::string> elements;
//     split(jsonString, ',', elements);
//     for(auto it = elements.begin(); it != elements.end(); it++){
//         std::vector<std::string> key_value;
//         split(*it, ':', key_value);
//         key_value[0].erase(0,1);
//         key_value[0].erase(key_value[0].length()-1, 1);
//         key_value[1].erase(0,1);
//         key_value[1].erase(key_value[1].length()-1, 1);
//         res[key_value[0]] = key_value[1];
//     }
// }

#include <string.h>

#define MAX_PAIRS 10
#define MAX_KEY_LEN 32
#define MAX_VALUE_LEN 64

struct KeyValuePair {
  char key[MAX_KEY_LEN];
  char value[MAX_VALUE_LEN];
};

void trimQuotes(char* str) {
  size_t len = strlen(str);
  if (len >= 2 && str[0] == '"' && str[len - 1] == '"') {
    // Сдвигаем строку влево и обрезаем кавычки
    memmove(str, str + 1, len - 2);
    str[len - 2] = '\0';
  }
}

void parseSimpleJson(const char* json, KeyValuePair* output, int* count) {
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

int main(){
    // std::string jsonString = "{\"name\":\"Sanya\",\"Surname\":\"Kholopov\"}";
    // std::map<std::string, std::string> json;
    // jsonParser(jsonString, json);

    // for(auto &item : json){
    //     printf("key: %s, value: %s\n", item.first.c_str(), item.second.c_str());
    // }

    const char* json = "{\"name\":\"Sanya\",\"Surname\":\"Kholopov\"}";

    KeyValuePair parsed[MAX_PAIRS];
    int pairCount = 0;

    parseSimpleJson(json, parsed, &pairCount);

    for (int i = 0; i < pairCount; i++) {
        printf("key: ");
        printf(parsed[i].key);
        printf(", value: ");
        printf(parsed[i].value);
    }
    

    return 0;
}