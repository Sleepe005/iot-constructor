#pragma once
#include "singleLinkList.h"

// Бинарный протокол обмена данными
// Структура протокола
// Стартовый байт - 1 байт
// Тип запроса	- 1 байт
// Тип данных - 1 байт
// Длина данных - 2 байта
// Данные - N байт
// Контрольная сумма - 1 байт

// Стартовый байт - 0xAA

// Тип запроса:
// - ping 0х01
// - authorize 0x02
// - post settings 0х03

// Тип данных 
// - Text 0x01
// - Key-value 0x02

// Serializer, deserializer

// struct KeyValueType{
//     char key[64];
//     char value[64];
// };

// void XTo8()

// void KeyValueToBytes(KeyValueType data, uint8_t *bytes_data){
    
// }

struct protocolRequest{
    uint8_t startByte = 0xAA;
    uint8_t requestType;
    uint8_t payloadType;
    uint16_t payloadSize;
    uint8_t *payload;
    //TODO: uint8_t controlSum 
};

// object to bytes
void serializerData(singleList<uint8_t> *&data, uint8_t *bytes_data){
    singleList<uint8_t> *buffer = data;
    while(buffer->next != NULL){
        *bytes_data++ = buffer->data;
        buffer = buffer->next;
    }
    *bytes_data++ = buffer->data;
    *bytes_data = 0x00;
}

void serializerData(protocolRequest &request, uint8_t *bytes_request){
    *bytes_request++ = request.startByte;
    *bytes_request++ = request.requestType;
    *bytes_request++ = request.payloadType;
    *bytes_request++ = request.payloadSize & 0xFF;
    *bytes_request++ = (request.payloadSize >> 8) & 0xFF;
    for(size_t i = 0; i < request.payloadSize; i++){
        *bytes_request++ = request.payload[i];
    }
}

void deserializerData(uint8_t *bytes_data, singleList<uint8_t> *&data){
    for(size_t i = 0; bytes_data[i] != 0x00; i++){
        pushBack(data, bytes_data[i]);
    }
}

void deserializerData(uint8_t *bytes_request, protocolRequest &request){
    request.startByte = bytes_request[0];
    request.requestType = bytes_request[1];
    request.payloadType = bytes_request[2];
    request.payloadSize = (uint16_t)bytes_request[3] | ((uint16_t)bytes_request[4] << 8);
    request.payload = new uint8_t[request.payloadSize];
    for(size_t i = 0; i < request.payloadSize; i++){
        request.payload[i] = bytes_request[5+i];
    }
}

void createRequest(uint8_t request_type, uint8_t payload_type, singleList<uint8_t> *&payload, singleList<uint8_t> *&request){
    // Стартовый байт
    pushBack(request, (uint8_t)0xAA);
    // Тип запроса
    pushBack(request, request_type);
    // Тип данных
    pushBack(request, payload_type);
    // Длина данных
    pushBack(request, uint8_t(lengthList(payload) & 0xFF));
    pushBack(request, (uint8_t)((lengthList(payload) >> 8) & 0xFF));
    // Данные
    pushBackList(request, payload);
    // TODO: Контрольная сумма
}

// int main(){
//     singleList<uint8_t> *list = nullptr;
//     pushBack(list, (uint8_t)0xAB);
//     pushBack(list, (uint8_t)0xAC);
//     pushBack(list, (uint8_t)0xAD);
//     pushBack(list, (uint8_t)0xAE);

//     // uint8_t bytes_data[2048] = {0};

//     // serializerData(list, bytes_data);

//     // printf("Serializer:\n");
//     // for(size_t i = 0; bytes_data[i] != 0xFF; i++){
//     //     printf("%X\n", bytes_data[i]);
//     // }

//     // singleList<uint8_t> *dlist = nullptr;
//     // deserializerData(bytes_data, dlist);
//     // printf("Deserializer\n");
//     // printList(dlist);

//     singleList<uint8_t> *request = nullptr;
//     createRequest((uint8_t)0x01, (uint8_t)0x01, list, request);

//     uint8_t b[2048] = {0};
//     serializerData(request, b);

//     protocolRequest pt;
//     deserializerData(b, pt);

//     uint8_t bt[2048] = {0};
//     serializerData(pt, bt);

//     deleteList(list);
//     deleteList(request);
// }
