Команда для запуска отладки - мониторинга:
cd ~/esp/esp-idf && . ./export.sh && cd ~/esp/plant_monitoring_project && idf.py monitor

#### Примечания

# Задаем путь проекта
PROJECT_PATH="~/esp/plant_monitoring_project"
FRAMEWORK_PATH="~/esp"

# Формируем команду mosquitto_pub
COMMAND="mosquitto_pub -L mqtt://$HOST/$TOPIC -m \"$DATA\""

# Выполняем команду
eval "$COMMAND"