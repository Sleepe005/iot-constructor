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
#include <ncurses.h>

int getSocket(struct addrinfo *res){
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // Пробуем подключиться к устройству
    int connres = connect(sockfd, res->ai_addr, res->ai_addrlen);
    
    if(connres == -1){ return 0; }

    return sockfd;
}

bool sendToESP(struct addrinfo *res, char *request, char *response){
    int sockfd = getSocket(res);

    // Отправляем запрос
    send(sockfd, request, strlen(request), 0);
    
    // Получаем ответ
    int bytesReceived = recv(sockfd, response, 2047, 0);

    close(sockfd);
    // Проверяем были ли получен ответ
    if (bytesReceived > 0) {
        response[bytesReceived] = '\0';
        return true;
    } else {
        return false;
    }
}

bool findESP(struct addrinfo *res){
    char request[] = "{\"method\":\"ping\"}";
    char response[2048] = {0};
    return sendToESP(res, request, response);
}

bool sendSettingsESP(){
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    getaddrinfo("192.168.4.1", "8080", &hints, &res);

    if(!findESP(res)){
        clear();
        printw("ESP not found");
        getch();
        return false;
    }

    char request[] = "{\"method\":\"wifi\",\"ssid\":\"Sleepe\",\"pass\":\"sleepewww\"}";
    char response[2048] = {0};
    sendToESP(res, request, response);

    return true;
}

int main(){
    setlocale(LC_ALL, "");
    initscr();

    printw("Подключитесь к wifi сети ESP\nPress any key to continue after connectig to ESP WiFi...");
    getch();
    clear();

    sendSettingsESP();

    clear();
    printw("Устройство получило данные, производится подключение. Переключите сеть WiFi");
    getch();
    clear();

    return 0;
}