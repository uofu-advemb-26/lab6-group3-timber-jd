/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include <semphr.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

int count = 0;
bool on = false;
SemaphoreHandle_t semaphore;

#define SUPERVISOR_PRIORITY (tskIDLE_PRIORITY + 4UL)
#define HIGH_PRIORITY       (tskIDLE_PRIORITY + 3UL)
#define MEDIUM_PRIORITY     (tskIDLE_PRIORITY + 2UL)
#define LOW_PRIORITY        (tskIDLE_PRIORITY + 1UL)
#define STACK_SIZE          configMINIMAL_STACK_SIZE

void medium_priority_task(__unused void *params)
{
    printf("Medium: starving the system.\n");
    while (1) {}
}

void low_priority_task(__unused void *params)
{
    printf("Low: attempting to take semaphore.\n");
    if (xSemaphoreTake(semaphore, 10) == pdTRUE) {
        printf("Low: got semaphore, holding for 1s.\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        xSemaphoreGive(semaphore);
        printf("Low: released semaphore.\n");
    }
    while (1) vTaskDelay(1);
}

void high_priority_task(__unused void *params)
{
    printf("High: attempting to take semaphore.\n");
    if (xSemaphoreTake(semaphore, pdMS_TO_TICKS(5000)) == pdTRUE) {
        printf("High: GOT semaphore.\n");
        xSemaphoreGive(semaphore);
    } else {
        printf("High: TIMED OUT so it didn't take the semaphore\n");
    }
    while (1) vTaskDelay(1);
}

void supervisor_task(__unused void *params)
{
    xTaskCreate(low_priority_task, "low", STACK_SIZE, NULL,
                LOW_PRIORITY, NULL);
    vTaskDelay(pdMS_TO_TICKS(500));
    xTaskCreate(medium_priority_task, "medium", STACK_SIZE, NULL,
                MEDIUM_PRIORITY, NULL);
    xTaskCreate(high_priority_task, "high", STACK_SIZE, NULL,
                HIGH_PRIORITY, NULL);
    while (1) vTaskDelay(pdMS_TO_TICKS(1000));
}

int main( void )
{
    stdio_init_all();
    sleep_ms(5000); 
    semaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(semaphore);
    
    xTaskCreate(supervisor_task, "supervisor",
                STACK_SIZE, NULL, SUPERVISOR_PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}
