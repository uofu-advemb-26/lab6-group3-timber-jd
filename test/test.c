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

void busy_busy()
{
    for (int i = 0; ; i++);
}

void busy_yield()
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

void print_task_times(uint64_t t1_time, uint64_t t2_time) {
    printf("Task 1 time: %llu\n", t1_time);
    printf("Task 2 time: %llu\n", t2_time);
}

void both_busy_busy(void) {
    uint64_t t1_time = 0, t2_time = 0;
    timeTasks(busy_busy, LOW_PRIORITY, &t1_time, busy_busy, LOW_PRIORITY, &t2_time, 0);
    print_task_times(t1_time, t2_time);
}

void both_busy_yield(void) {
    uint64_t t1_time = 0, t2_time = 0;
    timeTasks(busy_yield, LOW_PRIORITY, &t1_time, busy_yield, LOW_PRIORITY, &t2_time, 0);
    print_task_times(t1_time, t2_time);
}

void t1_busy_t2_yield(void) {
    uint64_t t1_time = 0, t2_time = 0;
    timeTasks(busy_busy, LOW_PRIORITY, &t1_time, busy_yield, LOW_PRIORITY, &t2_time, 0);
    print_task_times(t1_time, t2_time);
}

void high_priority_first(void) {
    uint64_t t1_time = 0, t2_time = 0;
    timeTasks(busy_busy, HIGH_PRIORITY, &t1_time, busy_busy, LOW_PRIORITY, &t2_time, 100);
    print_task_times(t1_time, t2_time);
}

void low_priority_first(void) {
    uint64_t t1_time = 0, t2_time = 0;
    timeTasks(busy_busy, LOW_PRIORITY, &t1_time, busy_busy, HIGH_PRIORITY, &t2_time, 100);
    print_task_times(t1_time, t2_time);
}

void busy_yield_different_priorities(void) {
    uint64_t t1_time = 0, t2_time = 0;
    timeTasks(busy_yield, HIGH_PRIORITY, &t1_time, busy_yield, LOW_PRIORITY, &t2_time, 0);
    print_task_times(t1_time, t2_time);
}



void setUp(void) {}

void tearDown(void) {}

// void test_variable_assignment()
// {
//     int x = 1;
//     TEST_ASSERT_TRUE_MESSAGE(x == 1,"Variable assignment failed.");
// }

// void test_multiplication(void)
// {
//     int x = 30;
//     int y = 6;
//     int z = x / y;
//     TEST_ASSERT_TRUE_MESSAGE(z == 5, "Multiplication of two integers returned incorrect value.");
// }

void test_supervisor(__unused void *params) {   
    while(1) {
        sleep_ms(5000); 
        printf("Start tests\n");
        UNITY_BEGIN(); 
        RUN_TEST(both_busy_busy);
        RUN_TEST(both_busy_yield);
        RUN_TEST(t1_busy_t2_yield);
        RUN_TEST(high_priority_first);
        RUN_TEST(low_priority_first);
        RUN_TEST(busy_yield_different_priorities);
        UNITY_END();
    }
}

int main (void)
{
    stdio_init_all();
    
    xTaskCreate(test_supervisor, "sup", configMINIMAL_STACK_SIZE, NULL, SUPERVISOR_PRIORITY, NULL);
    vTaskStartScheduler();

    
}
