#include <Arduino.h>
#include <DHTesp.h>

#define DHT_PIN 4
#define POT_PIN 34
#define GREEN_LED 2
#define RED_LED 15
#define BUZZER 27

DHTesp dht;
QueueHandle_t sensorQueue;

struct SensorData {
  float temperature;
  float humidity;
  int processValue;
};

void sensorTask(void *parameter) {
  SensorData data;
  while (true) {
    TempAndHumidity reading = dht.getTempAndHumidity();

    if (dht.getStatus() != 0) {
      data.temperature = NAN;
      data.humidity = NAN;
    } else {
      data.temperature = reading.temperature;
      data.humidity = reading.humidity;
    }

    data.processValue = analogRead(POT_PIN);
    xQueueOverwrite(sensorQueue, &data);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void controlTask(void *parameter) {
  SensorData data;
  while (true) {
    if (xQueuePeek(sensorQueue, &data, 0) == pdTRUE) {
      bool sensorFault = isnan(data.temperature) || isnan(data.humidity);
      bool alarm = sensorFault || data.temperature > 32.0 || data.processValue > 3000;

      digitalWrite(GREEN_LED, alarm ? LOW : HIGH);
      digitalWrite(RED_LED, alarm ? HIGH : LOW);
      digitalWrite(BUZZER, alarm ? HIGH : LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void monitorTask(void *parameter) {
  SensorData data;
  while (true) {
    if (xQueuePeek(sensorQueue, &data, 10) == pdTRUE) {
      Serial.print("Temperature: ");
      if (isnan(data.temperature)) Serial.print("nan");
      else Serial.print(data.temperature, 1);
      Serial.print(" C | Humidity: ");
      if (isnan(data.humidity)) Serial.print("nan");
      else Serial.print(data.humidity, 1);
      Serial.print(" % | Process Value: ");
      Serial.print(data.processValue);
      Serial.print(" | Status: ");

      if (isnan(data.temperature) || isnan(data.humidity)) {
        Serial.println("SENSOR FAULT");
      } else if (data.temperature > 32.0 || data.processValue > 3000) {
        Serial.println("ALARM");
      } else {
        Serial.println("NORMAL");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("================================");
  Serial.println("RTOS Monitoring System Started - BUILD9");
  Serial.println("================================");

  dht.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  sensorQueue = xQueueCreate(1, sizeof(SensorData));
  if (sensorQueue == NULL) {
    Serial.println("ERROR: Queue creation failed");
    while (true) vTaskDelay(pdMS_TO_TICKS(1000));
  }

  BaseType_t t1 = xTaskCreate(sensorTask, "Sensor Acquisition", 2048, NULL, 1, NULL);
  BaseType_t t2 = xTaskCreate(controlTask, "Control Logic", 2048, NULL, 2, NULL);
  BaseType_t t3 = xTaskCreate(monitorTask, "UART Monitor", 2048, NULL, 1, NULL);

  if (t1 == pdPASS && t2 == pdPASS && t3 == pdPASS) {
    Serial.println("RTOS tasks created successfully");
  } else {
    Serial.println("ERROR: RTOS task creation failed");
  }
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
