# Система мониторинга за комнатным растением

Для разработки проекта использовался фреймворк idf-esp.
Настройка проекта и среды для разработки в CLion:
- [Дока esp-idf](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-start-project.html)
- [Введение в ESP-IDF(RUS)](https://www.youtube.com/watch?v=CDBqYCwWD0c&t=1133s)
- [Введение в ESP-IDF(ENG)](https://www.youtube.com/watch?v=1o9zASOxZaQ)

![process_of_developmnet](imgs/what-you-need.png)

При разработке в CLion локальные конфиги Kconfig.projbuild - не импортируются в настройки автоматически. 
Процесс разработки идёт через sdkconfig и их нужно переносить самостоятельно.

Команда для запуска отладки:
```shell
cd ~/esp/esp-idf && . ./export.sh && cd ~/esp/plant_monitoring_project && idf.py monitor
```

```shell
mosquitto_pub -L mqtt://vm4481772.25ssd.had.wf:1883/esp32/plant_monitoring/metrics -m "{\"temperature\":26.25,\"humydity\":46.37}"
```
Station mode(STA) - режим работы, при котором устройство ESP32 подключается к существующей сети Wi-Fi, 
функционируя как клиент. В этом режиме ESP32 не создает свою собственную сеть, 
а вместо этого использует доступ к уже имеющейся точке доступа (например, роутеру) для 
обмена данными с другими устройствами в сети.

idf.py -p 1220 flash monitor - загрузка + отладка порт можно увидеть подлючив плату esp и написав команду.

Фреймворк esp-idf установлен в пути: `~/esp`

Проект расположен в пути: `~/esp/plant_monitoring_project`

Выйти из режима отладки можно с помощью комбинации `CTRL` + `]`

### Библиотеки для работы с перефирией

[Библиотеки](https://github.com/UncleRus/esp-idf-lib) esp-idf

- [Настройка](https://github.com/UncleRus/esp-idf-lib/tree/master/examples/dht/default) dht-драйвера
- 

| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# _Sample project_

(See the README.md file in the upper level 'examples' directory for more information about examples.)

This is the simplest buildable example. The example is used by command `idf.py create-project`
that copies the project to user specified path and set it's name. For more information follow the [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project)



## How to use example
We encourage the users to use the example as a template for the new projects.
A recommended way is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
