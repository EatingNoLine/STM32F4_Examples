#include "stm32f4xx_hal.h"

#define LED0_PIN                                GPIO_PIN_9
#define LED0_GPIO_PORT                          GPIOF
#define LED0_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOF_CLK_ENABLE()

#define LED1_PIN                                GPIO_PIN_10
#define LED1_GPIO_PORT                          GPIOF
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOF_CLK_ENABLE()

void LED_Init();

int main(void) {
    HAL_Init();
    LED_Init();

    while (1) {
        HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_PIN);
        HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
        HAL_Delay(1000);
    }
}

void LED_Init() {
    // Init LED0
    LED0_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef LED0_InitStruct;
    LED0_InitStruct.Pin = LED0_PIN;
    LED0_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    LED0_InitStruct.Pull = GPIO_PULLUP;
    LED0_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED0_GPIO_PORT, &LED0_InitStruct);

    // Init LED1
    LED1_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef LED1_InitStruct;
    LED1_InitStruct.Pin = LED1_PIN;
    LED1_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    LED1_InitStruct.Pull = GPIO_PULLUP;
    LED1_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED1_GPIO_PORT, &LED1_InitStruct);
}
