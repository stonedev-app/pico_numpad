#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// GPIO15
constexpr int BTN = 15;

__attribute__((noreturn)) void Task1(void *pvParameters) {
    pinMode(BTN, INPUT_PULLUP);
    int value = 0;
    while (true) {
        value = digitalRead(BTN);
        printf("%d\n", value);

        // 自分のスタック使用量を確認
        // UBaseType_t stackLeft = uxTaskGetStackHighWaterMark(nullptr);
        // printf("Stack Left(Task1) : %uword\n", stackLeft);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    DEBUG_RP2040_PORT.begin(115200);
    xTaskCreate(Task1, "Task1", 512, nullptr, 1, nullptr);
}

void loop() {
    // 何もしなくてOK
}

// スタックオーバーフロー検知
__attribute__((noreturn)) void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    // ここにエラー処理やデバッグ用の処理を書く
    // 例：シリアル出力や無限ループで停止
    printf("Stack overflow detected in task %s\n", pcTaskName);
    taskDISABLE_INTERRUPTS();
    for(;;);
}