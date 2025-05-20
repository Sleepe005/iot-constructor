# iot-constructor
iot-constructor/
│
├── qt_client/                     # Десктопное приложение на Qt
│   ├── main.cpp                   # Точка входа
│   ├── mainwindow.ui              # Интерфейс (Qt Designer)
│   ├── device_manager.cpp/.h      # Управление устройствами, UDP-сниффер
│   ├── device_model.cpp/.h        # Структура данных по каждому устройству
│   ├── http_requester.cpp/.h      # Отправка HTTP-команд на ESP
│   └── ota_uploader.cpp/.h        # Обновление прошивки (OTA)
│
├── esp_firmware/                 # Код для ESP32/ESP8266 (Arduino Core)
│   ├── main.ino                   # Основная логика
│   ├── config.h                   # Настройки (SSID, OTA и т.п.)
│   ├── ota_handler.cpp/.h         # OTA-обновление (ESPhttpUpdate)
│   ├── http_server.cpp/.h         # HTTP-сервер с эндпоинтами управления
│   └── broadcaster.cpp/.h         # UDP-рассылка "я в сети"
│
├── ota_server/                   # Простейший HTTP сервер с прошивками
│   └── upload_server.py           # На Flask, отдаёт .bin файлы
│
├── firmware_images/              # Готовые прошивки
│   ├── light_v1.bin
│   └── light_v2.bin
│
├── README.md                     # Инструкции по сборке и запуску
└── docs/                         # Схемы, протокол, примеры взаимодействия
    ├── protocol.md
    └── architecture_diagram.png
