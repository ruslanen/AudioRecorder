Концептуальное описание устройства
=====

TODO: Схема

### Компоненты
1. Модуль микрофона с усилителем MAX9814
2. Микроконтроллер Arduino Nano или Pro (?)
3. Wi-Fi модуль ESP8266
4. Переключатель
5. Модуль SD-карты
6. Модуль часов реального времени HW-084 или HW-111 (?)
7. Модуль беспроводной связи GPRS GSM GA6
8. Li-Ion аккумуляторы

### Требования к устройству
1. Запись аудиофайлов с заданным интервалом и заданным промежутком
2. Real-time прослушка

### Конфигурирование
Конфигурирование через веб-интерфейс, задание следующих параметров внучную:
1. Текущее время (при рабочем модуле беспроводной связи GA6 допустима синхронизация времени из сети)
2. Интервал записи файлов (в минутах, часах)
3. Промежуток записи файлов (в крон выражении?)
4. Адрес принимающего информацию сервиса

### Вопросы к анализу
1. Энергопотребление устройства
2. Если будет использоваться ESP8266 то скорее всего придется собирать для нее свою прошивку
3. Качество записи
4. Максимальный радиус действия
5. Помехи
6. Размер файлов
7. Каким образом организовать real-time прослушку пока не ясно
8. Энергопотребление модуля часов реального времени (на сколько хватит батарейки?)