#pragma once
#include <string.h>
#include <stdio.h>
#include "singleLinkList.h"

struct ProtocolRequest{
    uint8_t startByte = 0xAA;
    uint8_t requestType;
    uint8_t payloadType;
    uint16_t payloadSize;
    uint8_t *payload;
    //TODO: uint8_t controlSum 
};

uint16_t readUint16(const uint8_t *data, size_t offset) {
    return static_cast<uint16_t>(data[offset]) | (static_cast<uint16_t>(data[offset + 1]) << 8);
}

void addPayload(ProtocolRequest &request, char **values, size_t elementsCount){
    if(request.payloadType == 0x01){
        size_t totalSize = strlen(*values);

        request.payload = new uint8_t[totalSize];
        memcpy(request.payload, *values, totalSize);

        request.payloadSize = totalSize;
    }else if(request.payloadType == 0x02){
        // Содержимое payload
        // values count - 2 байти
        // n values sizes - 2 байта каждый
        // n values - 1 байт каждый

        // Выделяем память
        size_t totalSize = 2 + elementsCount*2;
        for(size_t i = 0; i < elementsCount; i++){
            totalSize += strlen(values[i])+1;
        }

        request.payload = new uint8_t[totalSize];
        request.payloadSize = totalSize;

        // Записываем данные
        size_t offset = 0;

        request.payload[offset++] = elementsCount & 0xFF;
        request.payload[offset++] = (elementsCount >> 8) & 0xFF;

        for(size_t i = 0; i < elementsCount; i++){
            request.payload[offset++] = (strlen(values[i])+1) & 0xFF;
            request.payload[offset++] = ((strlen(values[i])+1) >> 8) & 0xFF;
        }

    
        for(size_t i = 0; i < elementsCount; i++){
            for(size_t j = 0; j <= strlen(values[i]); j++){
                request.payload[offset++] = values[i][j];
            }
        }
    }
}

int getPayloadValues(ProtocolRequest &request, char **&values){
    if(request.payloadType == 0x01){
        // values = new char[1][request.payloadSize];
        memcpy(*values, request.payload, request.payloadSize);
        return 1;
    }else if(request.payloadType == 0x02){
        size_t offset = 0;

        size_t elementsCount = readUint16(request.payload, offset);
        offset+=2;

        uint16_t *sizes = new uint16_t[elementsCount*2];
        for(size_t i = 0; i < elementsCount; i++){
            sizes[i] = readUint16(request.payload, offset);
            offset+=2;
        }

        values = new char*[elementsCount];

        for (size_t i = 0; i < elementsCount; ++i) {
            values[i] = new char[sizes[i]];
            memcpy(values[i], request.payload + offset, sizes[i]);
            offset += sizes[i];
        }

        delete sizes;

        return elementsCount;
    }
}

void printPayload(const ProtocolRequest &request) {
    printf("Payload (%d bytes):\n", request.payloadSize);
    for (size_t i = 0; i < request.payloadSize; i++) {
        printf("  [%02zu] 0x%02X\n", i, request.payload[i]);
    }
}

uint8_t* protocolSerializer(ProtocolRequest &request){
    uint8_t *bytes_request = new uint8_t[5+request.payloadSize];
    
    bytes_request[0] = request.startByte;
    bytes_request[1] = request.requestType;
    bytes_request[2] = request.payloadType;
    bytes_request[3] = request.payloadSize & 0xFF;
    bytes_request[4] = (request.payloadSize >> 8) & 0xFF;
    for(size_t i = 0; i < request.payloadSize; i++){
        bytes_request[5+i] = request.payload[i];
    }

    return bytes_request;
}

ProtocolRequest protocolDeserializer(uint8_t *bytes_request){
    size_t offset = 0;

    ProtocolRequest request;
    request.startByte = bytes_request[offset++];
    request.requestType = bytes_request[offset++];
    request.payloadType = bytes_request[offset++];
    request.payloadSize = readUint16(bytes_request, offset);
    offset+=2;
    request.payload = new uint8_t[request.payloadSize];
    for(size_t i = 0; i < request.payloadSize; i++){
        request.payload[i] = bytes_request[offset++];
    }

    return request;
}