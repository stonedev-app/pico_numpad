/**
 * @file main.cpp
 * @brief メインプログラム
 */

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// ボタン入力に使うピン番号(GPIO15)
constexpr int BTN = 15;

/**
 * @brief Task1
 *
 * BTNの入力をシリアルログに出力するタスク
 *
 * @param pvParameters 未使用
 */
__attribute__((noreturn)) void Task1(void *pvParameters) {

    // GPIO15をプルアップして出力に設定
    pinMode(BTN, INPUT_PULLUP);

    // 1秒毎にGPIO15の値を読み取る
    while (true) {
        // GPIO15の値を読み取る
        const int value = digitalRead(BTN);
        // シリアルログに値を出力
        printf("%d\n", value);

        // タスクのスタック使用量を確認
        // UBaseType_t stackLeft = uxTaskGetStackHighWaterMark(nullptr);
        // printf("Stack Left(Task1) : %uword\n", stackLeft);

        // 1000ms待機
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief setup
 */
void setup() {
    // デバッグに指定しているシリアルポートを初期化
    DEBUG_RP2040_PORT.begin(115200);
    // Task1を開始
    xTaskCreate(Task1, "Task1", 512, nullptr, 1, nullptr);
}

/**
 * @brief loop
 */
void loop() {
    // 処理なし(タスクを使用)
}

/**
 * @brief vApplicationStackOverflowHook
 *
 * スタックオーバーフロー検知
 *
 * @param xTask タスク
 * @param pcTaskName タスク名
 */
__attribute__((noreturn)) void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {

    // スタックオーバーフローを起こしたタスク名をログ出力
    printf("Stack overflow detected in task %s\n", pcTaskName);

    // スタックオーバーフローを起こした場合は、以降安全のため処理を停止
    // 割り込みを停止
    taskDISABLE_INTERRUPTS();
    // 無限ループ(*以降何もしない)
    for(;;);
}