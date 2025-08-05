/**
 * @file main.cpp
 * @brief メインプログラム
 */

#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>

// ボタン入力に使うピン番号(GPIO15)
constexpr int BTN = 15;

// チャタリング用待機時間(50ms)
constexpr TickType_t DEBOUNCE_TIME = 50 / portTICK_PERIOD_MS;
// ボタン状態監視間隔(5ms)
constexpr TickType_t POLL_INTERVAL = 5 / portTICK_PERIOD_MS;

/**
 * @brief Task1
 *
 * ボタン状態をシリアルログに出力するタスク
 *
 * @param pvParameters 未使用
 */
__attribute__((noreturn)) void Task1(void *pvParameters) {

    // ボタン入力に使うピンをプルアップして出力に設定
    pinMode(BTN, INPUT_PULLUP);

    // 最新の安定したボタン状態
    int lastStableBtnState = digitalRead(BTN);
    // 最新のボタン状態
    int lastRawBtnState = lastStableBtnState;
    // 最新のボタン状態を更新した時間
    TickType_t lastBtnStateChangeTime = xTaskGetTickCount();

    // ボタン状態を指定時間間隔で監視
    while (true) {

        // 現在のボタン状態を取得する
        const int currentRawBtnState = digitalRead(BTN);
        // 現在のボタン状態と最新のボタン状態が異なる場合
        if (currentRawBtnState != lastRawBtnState) {
            // ボタン状態が変わったので時間をリセット
            lastBtnStateChangeTime = xTaskGetTickCount();
            // 最新のボタン状態を更新
            lastRawBtnState = currentRawBtnState;
        }

        // ボタン状態が変わって、かつ一定時間経過した場合
        if (currentRawBtnState != lastStableBtnState &&
            xTaskGetTickCount() - lastBtnStateChangeTime > DEBOUNCE_TIME) {
            // 最新の安定したボタン状態を更新
            lastStableBtnState = currentRawBtnState;
            printf(lastStableBtnState == LOW ? "Pressed\n" : "Released\n");
        }

        // 指定時間待機
        vTaskDelay(POLL_INTERVAL);
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