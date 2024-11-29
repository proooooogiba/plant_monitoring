# Задаем путь проекта
# shellcheck disable=SC2088
PROJECT_PATH="~/esp/plant_monitoring_project"
FRAMEWORK_PATH="~/esp/esp-idf"

# Формируем команду mosquitto_pub
COMMAND="cd $FRAMEWORK_PATH && . ./export.sh && $PROJECT_PATH && idf.py monitor"

# Выполняем команду
eval "$COMMAND"