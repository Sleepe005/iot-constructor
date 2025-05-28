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

// object to bytes
void serializerData(singleList<uint8_t> *&data, uint8_t *bytes_data){
    singleList<uint8_t> *buffer = data;
    while(data->next != NULL){
        *bytes_data++ = data->data;
        data = data->next;
    }
    *bytes_data++ = data->data;
    *bytes_data = 0x00;
}

void deserializerData(uint8_t *bytes_data, singleList<uint8_t> *&data){
    for(size_t i = 0; bytes_data[i] != 0x00; i++){
        pushBack(data, bytes_data[i]);
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