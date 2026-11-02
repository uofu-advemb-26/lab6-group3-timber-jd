#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"

#include <FreeRTOS.h>
#include "task.h"
#include <pico/time.h>

// Priorities used from reference solution.
#define LOW_PRIORITY ( tskIDLE_PRIORITY + 1UL )
#define HIGH_PRIORITY ( tskIDLE_PRIORITY + 2UL )
#define SUPERVISOR_PRIORITY ( tskIDLE_PRIORITY + 3UL )

void busy_busy(void)
{
    for (int i = 0; ; i++);
}

void busy_yield(void)
{
    for (int i = 0; ; i++) {
        taskYIELD();
    }
}

void timeTasks(TaskFunction_t task1, UBaseType_t task1Priority, uint64_t *task1Time,
            TaskFunction_t task2, UBaseType_t task2Priority,  uint64_t *task2Time,
            uint64_t task12_delayMs ) {
    
    TaskHandle_t task1Handle, task2Handle;

    xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE, NULL, task1Priority, &task1Handle);
    vTaskDelay(pdMS_TO_TICKS(task12_delayMs));
    xTaskCreate(task2, "Task2", configMINIMAL_STACK_SIZE, NULL, task2Priority, &task2Handle);
    
    vTaskDelay(pdMS_TO_TICKS(1000));

    *task1Time = ulTaskGetRunTimeCounter(task1Handle);
    *task2Time = ulTaskGetRunTimeCounter(task2Handle);

    vTaskDelete(task1Handle);
    vTaskDelete(task2Handle);
}

void setUp(void) {}

void tearDown(void) {}

void test_variable_assignment()
{
    int x = 1;
    TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
}

void test_multiplication(void)
{
    int x = 30;
    int y = 6;
    int z = x / y;
    TEST_ASSERT_TRUE_MESSAGE(z == 5, "Multiplication of two integers returned incorrect value.");
}

int main (void)
{
    stdio_init_all();
    while (1) {
        sleep_ms(5000); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_variable_assignment);
        RUN_TEST(test_multiplication);
        sleep_ms(5000);
        UNITY_END();
    }
}
