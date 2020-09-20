/*
 * Осуществляет запись n-секундных аудиофайлов после включения.
 * Непонятно почему, но на Arduino Uno питания в 3.3V недостаточно, в отличии от ESP8266, поэтому нужно использовать 5V для max9814.
 */
#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>

// Номер пина для MAX9814
#define REC_PIN A0
// Номер пина для SD-карты
#define CS_PIN 10

TMRpcm audio;
// Наименование файла метаданных
const String METADATA_FILE_NAME = "metadata";
// Расширение записываемого файла
const String REC_FILE_EXTENSION = ".mp3";
// Интервал записи фрагмента
const unsigned long REC_TIME_IN_MILLIS = 1000UL * 60UL * 10UL;
// Интервал задержки между записываемыми фрагментами
const unsigned long DELAY_TIME_IN_MILLIS = 1000;

// Флаг успешной инициализации устройства
bool isInitializedSuccess = true;
// Флаг завершения записи
bool isWriteCompleted = false;
// Флаг начала записи
bool isWriteStarted = false;

// Порядковый номер файла, который был записан с момента включения устройства
unsigned long uptimeFileNum = 1;

// Время с момента запуска устройства до состояния готовности к работе (время инициализации)
unsigned long initMillis = 0 ;

void setup() {
  Serial.begin(9600);
  pinMode(REC_PIN, INPUT);

  if (!SD.begin(CS_PIN)) {
    isInitializedSuccess = false;
    Serial.println("An error occurred while initializing SD-card");
    return;
  }

  audio.CSPin = CS_PIN;
  
  //ensureMetadataFileExists();
  Serial.println("Initialization done");
}

void loop() {
  if (!isInitializedSuccess) {
    Serial.println("Critical error");
    delay(60000);
    return;
  }

  if (initMillis == 0) {
    initMillis = millis();
    Serial.println("Init milliseconds: " + String(initMillis));
  }

  // Лимит времени для записи в текущей итерации
  unsigned long recMillisLimit = uptimeFileNum * REC_TIME_IN_MILLIS;
  unsigned long uptimeMillis = millis() + initMillis;  
  if (recMillisLimit > uptimeMillis && !isWriteStarted) {
    String fileName = String(uptimeFileNum) + REC_FILE_EXTENSION;
    Serial.println("Recording to " + fileName);

    char charBufFileName[10]; // fileName.length()
    fileName.toCharArray(charBufFileName, 10);
    
    audio.startRecording(charBufFileName, 16000, REC_PIN);
    isWriteStarted = true;
  } else if (recMillisLimit < uptimeMillis) {
    //Serial.println(recMillisLimit);
    //Serial.println(uptimeMillis);
    isWriteStarted = false;
    String fileName = String(uptimeFileNum) + REC_FILE_EXTENSION;
    Serial.println("Recording to " + fileName + " completed");

    char charBufFileName[10];
    fileName.toCharArray(charBufFileName, 10);
    
    audio.stopRecording(charBufFileName);
    uptimeFileNum++;
  }
}


void ensureMetadataFileExists() {
  if (!SD.exists(METADATA_FILE_NAME)) {
    // Если файла метаданных не существует, то считаем последним номер 1
    // В этом случае существовавшие ранне файлы могут быть перезаписаны
    // TODO: Определение номера файла по содержимому папки
    uptimeFileNum = 1;
    Serial.println("Metadata file is doesn't exists. Creating..");
    // Особенность библиотеки SD - нельзя создать файл без имени (например: .metadata)
    // TODO: Другой способ реализации записи скрытого файла
    File metadataFile = SD.open(METADATA_FILE_NAME, FILE_WRITE);
    metadataFile.print(uptimeFileNum);
    metadataFile.close();
  } else {
    File metadataFile = SD.open(METADATA_FILE_NAME, FILE_READ);
    String fileContent = "";
    while (metadataFile.available()) {
      fileContent += (char)metadataFile.read();
    }
    
    metadataFile.close();
    uptimeFileNum = fileContent.toInt();
    Serial.println("The last file number from metadata is: " + fileContent);
  }
}
