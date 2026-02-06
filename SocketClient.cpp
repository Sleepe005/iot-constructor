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

void split(std::string split_string, char separator, std::vector<std::string> &res){
    std::string buf = "";
    
    for(auto it = split_string.begin(); it != split_string.end(); ++it){
        if(*it != separator){
            buf += *it;
        }else{
            res.push_back(buf);
            buf = "";
        }
    }
    res.push_back(buf);
}

class HttpRequest{
    private:
        std::string method;  
        std::string path;
        std::map<std::string, std::string> header;

    public:
        HttpRequest(std::string requestText){
            std::vector<std::string> requestLines;
            split(requestText, '\n', requestLines);

            std::vector<std::string> httpSettings;
            split(requestLines[0], ' ', httpSettings);
            this->method=httpSettings[0];
            this->path=httpSettings[1];

            int index = 1;
            while(requestLines.begin()+index != requestLines.end() && requestLines[index] != "\r"){
                std::vector<std::string> headerItem;
                split(requestLines[index], ':', headerItem);
                this->header[headerItem[0]] = headerItem[1];
                ++index;
            }
        }

        std::string Method() const{
            return method;
        }

        std::string Path() const{
            return path;
        }

        std::map<std::string, std::string> Header() const{
            return header;
        }
};

void initializationRequest(std::string requestText){
    HttpRequest request(requestText);
    printf("------------------------------\n");
    printf("Method: %s; Path: %s;\n", request.Method().c_str(), request.Path().c_str());

    for(const auto& elements : request.Header()){
        printf("%s: %s\n", elements.first.c_str(), elements.second.c_str());
    }
    printf("------------------------------\n");
}

int main(){
    struct addrinfo hints, *res;
    int sockfd;
    
    // first, load up address structs with getaddrinfo():
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    
    getaddrinfo("192.168.4.1", "8080", &hints, &res);
    
    // make a socket:
    
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sockfd, res->ai_addr, res->ai_addrlen);

    char *request = "{\"method\":\"pingdffgkjghj\"}";

    send(sockfd, request, strlen(request), 0);

    // Приём ответа
    char buffer[2048];
    int bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Завершаем строку
        printf("Ответ от сервера: %s\n", buffer);
    } else {
        printf("Нет ответа или ошибка");
    }

    // Освобождение ресурсов
    freeaddrinfo(res);
    close(sockfd);
    
    // bind it to the port we passed in to getaddrinfo():
    
    // bind(sockfd, res->ai_addr, res->ai_addrlen);

    // // listening to the assigned socket
    // listen(sockfd, 5);
    // system("clear");

    // char host[NI_MAXHOST], service[NI_MAXSERV];
    // getnameinfo(res->ai_addr, res->ai_addrlen, 
    // host, sizeof host, 
    // service, sizeof service, 
    // NI_NUMERICHOST | NI_NUMERICSERV);
    // printf("IP: %s\n", host);
    // printf("Port: %s\n", service);      
    // printf("Listen localhost:8082...\n");

    // while (true)
    // {
    //     // accepting connection request
    //     int clientSocket
    //         = accept(sockfd, nullptr, nullptr);

    //     // recieving data
    //     char buffer[2048] = { 0 };
    //     recv(clientSocket, buffer, sizeof(buffer), 0);

    //     std::string s(buffer);
    //     initializationRequest(s);

    //     // printf("Message from client:\n%s", buffer);

    //     send(clientSocket, "Accept\n", 7, 0);
    // }

    // // closing the socket.
    // close(sockfd);

    return 0;
}