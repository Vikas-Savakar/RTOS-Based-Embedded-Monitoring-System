#include <Arduino.h>
#include <DHTesp.h>

// -----------------------------
// Pin configuration
// -----------------------------
constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t POT_PIN = 34;
constexpr uint8_t GREEN_LED = 2;
constexpr uint8_t RED_LED = 15;
constexpr uint8_t BUZZER = 27;

// -----------------------------
// Application settings
// -----------------------------
constexpr float TEMPERATURE_LIMIT_C = 32.0f;
constexpr int PROCESS_VALUE_LIMIT = 3000;
constexpr uint32_t SENSOR_PERIOD_MS = 2000;
constexpr uint32_t CONTROL_PERIOD_MS = 200;
constexpr uint32_t MONITOR_PERIOD_MS = 1000;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint16_t TASK_STACK_SIZE = 2048;

DHTesp dht;
QueueHandle_t sensorQueue = nullptr;

struct SensorData {
  float temperature;
  float humidity;
  int processValue;
};

void setAlarmOutputs(bool alarmActive) {
  digitalWrite(GREEN_LED, alarmActive ? LOW : HIGH);
  digitalWrite(RED_LED, alarmActive ? HIGH : LOW);
  digitalWrite(BUZZER, alarmActive ? HIGH : LOW);
}

void sensorTask(void *parameter) {
  (void)parameter;
  SensorData data{};

  while (true) {
    const TempAndHumidity reading = dht.getTempAndHumidity();

    if (dht.getStatus() != 0) {
      data.temperature = NAN;
      data.humidity = NAN;
    } else {
      data.temperature = reading.temperature;
      data.humidity = reading.humidity;
    }

    data.processValue = analogRead(POT_PIN);

    // Queue length is one because the application only needs the latest sample.
    xQueueOverwrite(sensorQueue, &data);
    vTaskDelay(pdMS_TO_TICKS(SENSOR_PERIOD_MS));
  }
}

void controlTask(void *parameter) {
  (void)parameter;
  SensorData data{};

  while (true) {
    if (xQueuePeek(sensorQueue, &data, 0) == pdTRUE) {
      const bool sensorFault = isnan(data.temperature) || isnan(data.humidity);
      const bool thresholdAlarm =
          data.temperature > TEMPERATURE_LIMIT_C ||
          data.processValue > PROCESS_VALUE_LIMIT;
      const bool alarmActive = sensorFault || thresholdAlarm;

      setAlarmOutputs(alarmActive);
    }

    vTaskDelay(pdMS_TO_TICKS(CONTROL_PERIOD_MS));
  }
}

void monitorTask(void *parameter) {
  (void)parameter;
  SensorData data{};

  while (true) {
    if (xQueuePeek(sensorQueue, &data, pdMS_TO_TICKS(10)) == pdTRUE) {
      const bool sensorFault = isnan(data.temperature) || isnan(data.humidity);
      const bool thresholdAlarm =
          data.temperature > TEMPERATURE_LIMIT_C ||
          data.processValue > PROCESS_VALUE_LIMIT;

      Serial.print("Temperature: ");
      if (isnan(data.temperature)) {
        Serial.print("nan");
      } else {
        Serial.print(data.temperature, 1);
      }

      Serial.print(" C | Humidity: ");
      if (isnan(data.humidity)) {
        Serial.print("nan");
      } else {
        Serial.print(data.humidity, 1);
      }

      Serial.print(" % | Process Value: ");
      Serial.print(data.processValue);
      Serial.print(" | Status: ");

      if (sensorFault) {
        Serial.println("SENSOR FAULT");
      } else if (thresholdAlarm) {
        Serial.println("ALARM");
      } else {
        Serial.println("NORMAL");
      }
    }

    vTaskDelay(pdMS_TO_TICKS(MONITOR_PERIOD_MS));
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(500);

  Serial.println("========================================");
  Serial.println("RTOS Embedded Monitoring System");
  Serial.println("ESP32 + FreeRTOS + DHT22 + ADC");
  Serial.println("========================================");

  dht.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  setAlarmOutputs(false);

  sensorQueue = xQueueCreate(1, sizeof(SensorData));
  if (sensorQueue == nullptr) {
    Serial.println("ERROR: Queue creation failed");
    while (true) {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  const BaseType_t sensorTaskCreated = xTaskCreate(
      sensorTask, "Sensor Acquisition", TASK_STACK_SIZE, nullptr, 1, nullptr);
  const BaseType_t controlTaskCreated = xTaskCreate(
      controlTask, "Control Logic", TASK_STACK_SIZE, nullptr, 2, nullptr);
  const BaseType_t monitorTaskCreated = xTaskCreate(
      monitorTask, "UART Monitor", TASK_STACK_SIZE, nullptr, 1, nullptr);

  if (sensorTaskCreated == pdPASS &&
      controlTaskCreated == pdPASS &&
      monitorTaskCreated == pdPASS) {
    Serial.println("RTOS tasks created successfully");
    Serial.println("Monitoring started");
  } else {
    Serial.println("ERROR: RTOS task creation failed");
  }
}

void loop() {
  // Application work is handled by the FreeRTOS tasks above.
  vTaskDelay(pdMS_TO_TICKS(1000));
}
