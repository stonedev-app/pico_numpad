#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// GPIO15
constexpr int BTN = 15;

void Task1(void *pvParameters) {
    pinMode(BTN, INPUT_PULLUP);
    int value = 0;
    while (true) {
        value = digitalRead(BTN);
        printf("%d\n", value);

        // 自分のスタック使用量を確認
        // UBaseType_t stackLeft = uxTaskGetStackHighWaterMark(NULL);
        // printf("Stack Left(Task1) : %uword\n", stackLeft);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    DEBUG_RP2040_PORT.begin(115200);
    xTaskCreate(Task1, "Task1", 512, NULL, 1, NULL);
}

void loop() {
    // 何もしなくてOK
}

// スタックオーバーフロー検知
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // ここにエラー処理やデバッグ用の処理を書く
    // 例：シリアル出力や無限ループで停止
    printf("Stack overflow detected in task %s\n", pcTaskName);
    taskDISABLE_INTERRUPTS();
    for(;;);
}