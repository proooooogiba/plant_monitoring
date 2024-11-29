# Задаем переменную хоста
HOST="vm4481772.25ssd.had.wf:1883"

# Определяем JSON данные
DATA='{\"temperature\":90.25,\"humidity\":46.37, \"linkquality\":1.0}'

# Определяем топик
TOPIC="esp32/plant_monitoring/metrics"

# Формируем команду mosquitto_pub
COMMAND="mosquitto_pub -L mqtt://$HOST/$TOPIC -m \"$DATA\""

# Выполняем команду
eval "$COMMAND"